# Macros
MKDIR=mkdir -p
RM=rm -R -f

# quad controller project
PROJECT=quad_controller

# current directory
CURRENT_DIR= $(shell pwd)

# output file locations
OUT:=$(CURRENT_DIR)/OUT
BIN:=$(CURRENT_DIR)/BIN

# Compiler
export CC=/opt/microchip/xc32/v1.40/bin/xc32

# Linker
LD=$(CC)-g++

# CC Compiler directives
#CFLAGS=-g -std=c99 -mprocessor=32MX795F512L -nostartfiles
CFLAGS=-g -mprocessor=32MX795F512L -nostartfiles
CPPFLAGS=-g -mprocessor=32MX795F512L -nostartfiles

# LD flags --defsym=_min_heap_size=1024
LDFLAGS=-mprocessor=32MX795F512L -nostartfiles -Wl,--defsym=_min_heap_size=0x400 -Wl,-Map=$(BIN)/$(PROJECT).map

# Directories of the project
DIRS= BSP CPU app drivers uC-CPU uC-LIB uCOS-III
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

# final executable
all: OUT_DIR $(BIN)/$(PROJECT).hex

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
	rm -f ./.depend
	$(CC)-g++ $(INC) $(CPPFLAGS) -MM $^ | sed -e 's/\(\w*.\.o\)/.\/OUT\/\1/' > $@

include .depend

#Hex file generation
$(BIN)/$(PROJECT).hex: $(BIN)/$(PROJECT).bin
	$(CC)-bin2hex $(BIN)/$(PROJECT).bin

#Binary file generation
$(BIN)/$(PROJECT).bin: $(S_OBJ_FILES) $(C_OBJ_FILES) $(CPP_OBJ_FILES)
	$(LD) $(LDFLAGS) $(S_OBJ_FILES) $(C_OBJ_FILES) $(CPP_OBJ_FILES) -o $(BIN)/$(PROJECT).bin

#output directories
OUT_DIR:
	$(MKDIR) ./OUT
	$(MKDIR) ./BIN

#cleaning
clean:
	$(RM) $(OUT)
	$(RM) $(BIN)
	$(RM) .depend