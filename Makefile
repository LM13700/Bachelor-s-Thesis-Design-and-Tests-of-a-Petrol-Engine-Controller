TARGET := ECU
BUILD_DIR := build

DEBUG = 1
VERBOSE := 0

# Echo suspend
ifeq ($(VERBOSE),1)
  NO_ECHO :=
else
  NO_ECHO := @
endif

MK := mkdir
RM := del /q


#######################################
# Sources
#######################################
# C sources
C_SOURCES =  \
Core/Src/system_stm32f4xx.c \
Core/Src/engine_constants.c \
Core/Src/engine_sensors.c \
Core/Src/ignition_driver.c \
Core/Src/injection_driver.c \
Core/Src/main.c \
Core/Src/speed_density.c \
Core/Src/swo.c \
Core/Src/tables.c \
Core/Src/trigger_decoder.c \
Core/Src/utils.c \

# C includes
C_INCLUDES = \
-ICore/Inc \
-IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
-IDrivers/CMSIS/Include

# ASM sources
ASM_SOURCES =  \
startup_stm32f411xe.s

# AS includes
AS_INCLUDES = 


#######################################
# Binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


#######################################
# CFLAGS
#######################################
# cpu
CPU := -mcpu=cortex-m4

# fpu
FPU := -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI := -mfloat-abi=hard

# mcu
MCU := $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS :=

# C defines
C_DEFS := \
-DSTM32F411xE

# General flags
GFLAGS += -fdata-sections
GFLAGS += -ffunction-sections

# Warning flags
WFLAGS += -Wall
WFLAGS += -Wextra
WFLAGS += -Wduplicated-branches
WFLAGS += -Wduplicated-cond
WFLAGS += -Wlogical-op
#CFLAGS += -Wmissing-declarations
WFLAGS += -Wmissing-prototypes
WFLAGS += -Wno-expansion-to-defined
WFLAGS += -Wno-unused-parameter
WFLAGS += -Wno-enum-conversion
WFLAGS += -Wpedantic
WFLAGS += -Wshadow
WFLAGS += -Wstack-usage=256
WFLAGS += -Wuninitialized
WFLAGS += -Wunsafe-loop-optimizations

# compile gcc flags
ASFLAGS += $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) $(WFLAGS) $(GFLAGS)

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(WFLAGS) $(GFLAGS) -fdiagnostics-color=auto -std=c99

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -gdwarf-2 -DDEBUG
OPT += -Og
else
OPT += -Ofast
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F411CEUx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# Build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR):
	$(NO_ECHO)$(MK) $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo Compiling file: $@
	$(NO_ECHO)$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo Assembling file: $@
	$(NO_ECHO)$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo Linking target: $@
	$(NO_ECHO)$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(NO_ECHO)$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo Preparing: $@
	$(NO_ECHO)$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo Preparing: $@
	$(NO_ECHO)$(BIN) $< $@


#######################################
# Clean up
#######################################
clean:
	@echo Cleaning directory: $(BUILD_DIR)
	$(NO_ECHO)$(RM) $(BUILD_DIR)\*

#######################################
# Flash
#######################################
flash: all
	openocd -f stm32f4x_hardware_reset.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify; reset; exit"


#######################################
# Erase
#######################################
erase:
	openocd -f stm32f4x_hardware_reset.cfg -c "init; reset halt; flash erase_sector 0 0 last; exit"

#######################################
# Dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
