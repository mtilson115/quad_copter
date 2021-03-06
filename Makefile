# Macros
MKDIR=mkdir -p
RM=rm -R -f

# quad controller project
PROJECT=quad_controller

# Device
DEVICE=32MX795F512L

# Debugger/hwtool
HWTOOL=ICD3

# current directory
CURRENT_DIR= $(shell pwd)

# output file locations
OUT:=$(CURRENT_DIR)/OUT
BIN:=$(CURRENT_DIR)/BIN

# Compiler
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CC_VERS=v2.20
	export CC=/opt/microchip/xc32/$(CC_VERS)/bin/xc32
	DEBUGGER=/opt/microchip/mplabx/v5.30/mplab_platform/bin/mdb.sh
	FLASHTOOL=/opt/microchip/mplabx/v5.30/mplab_platform/mplab_ipe/ipecmd.sh
endif

ifeq ($(UNAME_S),Darwin)
	CC_VERS=v2.40
	export CC=/Applications/microchip/xc32/$(CC_VERS)/bin/xc32
	DEBUGGER=/Applications/microchip/mplabx/v5.30/mplab_platform/bin/mdb.sh
	FLASHTOOL=/Applications/microchip/mplabx/v5.30/mplab_platform/mplab_ipe/bin/ipecmd.sh
endif

# Linker
LD=$(CC)-g++

# CC Compiler directives
CFLAGS=-g -std=c99 -G 0 -mprocessor=$(DEVICE) -nostartfiles -Wall -Wextra -Wmissing-prototypes -Wold-style-definition -Wold-style-declaration
CPPFLAGS=-g -G 0 -mprocessor=$(DEVICE) -nostartfiles -Wall -Wextra

LDFLAGS= -mprocessor=$(DEVICE) -G 0 -Wl,--defsym=_min_heap_size=0x1000 -Wl,-Map=$(BIN)/$(PROJECT).map -Wl,--report-mem -Wl,--script p32MX795F512L.ld -Wl, -nostartfiles

# Directories of the project
DIRS= bsp CPU app drivers uC-CPU uC-LIB uCOS-III algs comms
INC:=$(DIRS:%=-I./%/inc)
VSRC:=$(DIRS:%=./%/src)
VINC:=$(DIRS:%=./%/inc)

# file names
FILES=$(shell find $(VSRC) -iname "*.c" -o -iname "*.S" -o -iname "*.cpp" -o -iname "*.h")
CPP_FILES=$(realpath $(shell find $(VSRC) -iname "*.cpp" ))
C_FILES=$(realpath $(shell find $(VSRC) -iname "*.c" ))
S_FILES=$(realpath $(shell find $(VSRC) -iname "*.S" ))
C_OBJ_FILES=$(addprefix ./OUT/, $(notdir $(C_FILES:.c=.o)))
CPP_OBJ_FILES=$(addprefix ./OUT/, $(notdir $(CPP_FILES:.cpp=.o)))
S_OBJ_FILES=$(addprefix ./OUT/, $(notdir $(S_FILES:.S=.o)))
OBJ_FILES=$(addprefix ./OUT/, $(notdir $(FILES:.c=.o)))

# Update the search paths for files
VDIRS:=$(realpath $(VSRC))
VDIRS+=$(realpath $(VINC))
space :=
space +=
VPATH:=$(subst $(space),:,$(VDIRS))

all: OUT_DIR $(BIN)/$(PROJECT).hex $(BIN)/$(PROJECT).lst

# Rules for building various files
./OUT/%.o: %.c %.h
	$(CC)-g++ $(INC) $(CFLAGS) -x c -c -o $@ $<

./OUT/%.o: %.c
	$(CC)-g++ $(INC) $(CFLAGS) -x c -c -o $@ $<

./OUT/%.o: %.cpp %.h
	$(CC)-g++ $(CPPFLAGS) -x c++ -frtti -fexceptions -fno-check-new -fenforce-eh-specs $(INC) -c -o $@ $<

./OUT/%.o: %.cpp
	$(CC)-g++ $(CPPFLAGS) -x c++ -frtti -fexceptions -fno-check-new -fenforce-eh-specs $(INC) -c -o $@ $<

./OUT/%.o: %.S %.h
	$(CC)-g++ $(INC) -nostartfiles -c -o $@ $<

./OUT/%.o: %.S
	$(CC)-g++ $(INC) -nostartfiles -c -o $@ $<

#dependency generation
depend: .depend

.depend: $(S_FILES) $(CPP_FILES) $(C_FILES)
	$(CC)-g++ $(INC) $(CPPFLAGS) -MM $^ | sed -e 's/\(\w*.\.o\)/.\/OUT\/\1/' > $@

include .depend

#Hex file generation
$(BIN)/$(PROJECT).hex: $(BIN)/$(PROJECT).elf
	$(CC)-bin2hex $(BIN)/$(PROJECT).elf

#Binary file generation
$(BIN)/$(PROJECT).elf: $(S_OBJ_FILES) $(C_OBJ_FILES) $(CPP_OBJ_FILES)
	$(LD) -o $(BIN)/$(PROJECT).elf $(S_OBJ_FILES) $(C_OBJ_FILES) $(CPP_OBJ_FILES) $(LDFLAGS)

$(BIN)/$(PROJECT).lst: $(BIN)/$(PROJECT).elf
	$(CC)-objdump -d $(BIN)/$(PROJECT).elf > $(BIN)/$(PROJECT).lst

#output directories
OUT_DIR:
	$(MKDIR) ./OUT
	$(MKDIR) ./BIN

#cleaning
clean:
	$(RM) $(OUT)
	$(RM) $(BIN)

# program in debug mode with mdb.sh
debug:
ifneq ("$(wildcard $(BIN))","")
	@echo 'Creating install config'
	@echo 'device PIC'$(DEVICE) > install.txt
	@echo 'hwtool '$(HWTOOL) >> install.txt
	@echo 'programming in debug mode'
	@echo 'program '$(PROJECT).elf >> install.txt
	@echo 'reset MCLR' >> install.txt
	@echo 'Moving install config'
	mv install.txt $(BIN)
	@echo 'Programming...' $(BIN)/$(PROJECT).elf
	@echo $(DEBUGGER)
	cd $(BIN) && $(DEBUGGER) install.txt
else
	@echo 'No valid binaries.  Call make first?'
endif

# flash
flash:
ifneq ("$(wildcard $(BIN))","")
	@echo 'Flashing the .hex file'
	$(FLASHTOOL) -P$(DEVICE) -TP$(HWTOOL) -M -Y -F$(BIN)/$(PROJECT).hex
	$(RM) *xml*
	$(RM) log.*
else
	@echo 'No valid binaries.  Call make first?'
endif
