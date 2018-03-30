# Enable verbose output from EE_OPT
ifeq ($(call iseeopt, VERBOSE), yes)
VERBOSE = 1
endif

# Initial definition of target all as predefined
all::

include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

MCHP_DATA_DIR := $(PIC32_GCCDIR)/pic32mx
PIC32_LIB_DIR := $(MCHP_DATA_DIR)/lib
PIC32_INCLUDE_DIR := $(MCHP_DATA_DIR)/include

MCHP_SUPPORT_DIR := $(MCHP_DATA_DIR)
PIC32_H_DIR := $(MCHP_SUPPORT_DIR)/include
PIC32_LINKER_DIR := $(MCHP_SUPPORT_DIR)/lib/ldscripts

ifneq ($(ONLY_LIBS), TRUE)
ifneq ($(call iseeopt, __BIN_DISTR), yes) 
OPT_LIBS += -lee -L .
LIBDEP += libee.a
else
OPT_LIBS += -l$(EELIB) -L $(EEBASE)/lib
LIBDEP += $(EEBASE)/lib/lib$(EELIB).a
endif
endif

LIBDEP += $(ALL_LIBS)
LIBDEP += $(LDDEPS)
## Libraries from MC
OPT_LIBS += -lm -lc -ldsp
OPT_LIBS += -l$(subst .a,,$(subst lib,,$(PIC32_DEV_LIB)))

OPT_LIBS += -L $(call native_path,$(PIC32_LIB_DIR))
# check if PIC32_LIBD_DIR is empty
ifneq ($(PIC32_LIBD_DIR),)
OPT_LIBS += -L $(call native_path,$(PIC32_LIBD_DIR))
endif

# #Includes from MC
INTERNAL_GCCINCLUDEDIR := -I$(call native_path,$(PIC32_INCLUDE_DIR))

ALLINCPATH += $(INTERNAL_GCCINCLUDEDIR)



################### PIC32-related directories
# we should look if these need to be moved inside dir.mk

#if PIC32_GCCDIR is defined
ifneq ($(PIC32_GCCDIR),)
DEFS_CC += -D__PIC32_INCLUDE_REGS__
MPLAB_INCLUDE_REGS=__PIC32_INCLUDE_REGS__
endif

# Add application file to dependencies
ifneq ($(ONLY_LIBS), TRUE)
TARGET := pic32.objdump pic32.hex
endif

# When building for MPLAB IDE, we do not have to include the source
# code of the application, but we have to include the library
# containing the EE code. 
# note that:
# - libee.a is not included in ALL_LIBS
# - when GENERATE_MPLABIDE_LIBS is defined, ONLY_LIBS is NOT defined
ifeq ($(GENERATE_MPLABIDE_LIBS), TRUE)
TARGET:=libee.a generate_eeopt

# we reset the SRCS variable (no application has to be compiled), and
# we put the eecfg.c inside the library
SRCS:=
LIBEESRCS+= $(OUTBASE)/eecfg.c
endif

include $(PKGBASE)/cfg/cfg.mk


#
# --------------------------------------------------------------------------
#

##
## Source files and paths
##

LIBEESRCS += $(EE_SRCS)
LIBEEOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBEESRCS))))

LIBEESRCS += $(LIB_SRCS)
LIBOBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(LIBSRCS))))

SRCS += $(foreach d,$(APP_SRCS),$(call unix_relpath,$d))
OBJS := $(addprefix $(OBJDIR)/, $(patsubst %.c,%.o,$(patsubst %.S,%.o, $(SRCS))))

ALLOBJS = $(LIBEEOBJS) $(LIBOBJS) $(OBJS)

OBJDIRS=$(sort $(dir $(ALLOBJS))) frommchp


vpath %.c $(EE_VPATH) $(APPBASE)
vpath %.S $(EE_VPATH) $(APPBASE)


## Compute common variables ##

COMPUTED_ALLINCPATH := $(ALLINCPATH)
COMPUTED_OPT_LINK := $(OPT_LINK)
COMPUTED_OPT_ASM := $(OPT_ASM)
COMPUTED_OPT_CC := $(OPT_CC)
COMPUTED_OPT_AR := $(OPT_AR)

## Select input filename format ##
SOURCEFILE = $(call native_path,$<)
TARGETFILE = $(call native_path,$@)

###########################
###########################
## Main rules: all clean ##
.PHONY: all clean

all:: $(ALL_LIBS) $(TARGET) 
	@printf "************************************\n\n"
	@printf "Compilation terminated successfully!\n"

clean::
	@-rm -rf *.a *.ld *.map *.$(PIC32_EXTENSION) *.objdump *.hex deps deps.pre obj ee_pic32regs.h
# to support "make clean all"
ifeq ($(findstring all,$(MAKECMDGOALS)),all)
	@printf "CLEAN (also \"all\" specified, frommchip directory not removed)\n"
else
	@printf "CLEAN\n";
	@-rm -rf frommchp
endif

pic32.objdump: pic32.$(PIC32_EXTENSION)
	@printf "OBJDUMP\n";
	$(QUIET)$(EE_OBJDUMP) -d pic32.$(PIC32_EXTENSION) > pic32.objdump

pic32.hex: pic32.$(PIC32_EXTENSION)
	@printf "BIN2HEX\n";
	$(QUIET)$(EE_BIN2HEX) pic32.$(PIC32_EXTENSION)

## Object file creation ###
pic32.$(PIC32_EXTENSION): $(OBJS) $(LINKDEP) $(LIBDEP) 
	@printf "LD\n";
	$(QUIET)$(EE_LINK) $(COMPUTED_OPT_LINK) \
		-o $(TARGETFILE) $(OBJS) \
		$(OPT_LIBS) \
		-Wl,-Map="pic32.map"
#--start-group $(OPT_LIBS) --end-group 

# produce the object file from assembly code in a single step
# chris: I found no *.inc file in the c32 distribution
#$(OBJDIR)/%.o: %.S ee_pic32regs.inc
$(OBJDIR)/%.o: %.S
	$(VERBOSE_PRINTCPP) $(EE_CC) $(COMPUTED_ALLINCPATH) $(DEFS_ASM) $(DEPENDENCY_OPT) -c $(SOURCEFILE) -o $(TARGETFILE)
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))

# produce the object file from C code in a single step
$(OBJDIR)/%.o: %.c ee_pic32regs.h
	$(VERBOSE_PRINTCPP) $(EE_CC) $(COMPUTED_OPT_CC) $(COMPUTED_ALLINCPATH) $(DEFS_CC) $(DEPENDENCY_OPT) -c $(SOURCEFILE) -o $(TARGETFILE)
	$(QUIET) $(call make-depend, $<, $@, $(subst .o,.d,$@))


##
## Microchip C32 files
##

# Check if the MCU model has been defined
ifneq ($(PIC32_MODEL),)

ee_pic32regs.h: frommchp/$(PIC32_INCLUDE_C)
	@printf "GEN ee_pic32regs.h\n"
	@printf "/* Automatically generated from Makefile */\n" > ee_pic32regs.h
	@printf "#include \"frommchp/$(PIC32_INCLUDE_C)\"\n" >> ee_pic32regs.h

# chris: I found no *.inc file in the c32 distribution
#ee_pic32regs.inc: frommchp/$(PIC32_INCLUDE_S)
#	@printf "GEN ee_pic32regs.inc\n"
#	@printf "; Automatically generated from Makefile\n" > ee_pic32regs.inc
#	@printf "	.equ __$(PIC32_MODEL), 1 \n" >> ee_pic32regs.inc
#	@printf "	.include \"frommchp/$(PIC32_INCLUDE_S)\" \n" >> ee_pic32regs.inc

frommchp/$(PIC32_INCLUDE_C): $(PIC32_H_DIR)/$(PIC32_INCLUDE_C) frommchp
	@printf "CP $(PIC32_INCLUDE_C)\n"; cp $(SOURCEFILE) $(TARGETFILE)

# chris: I found no *.inc file in the c32 distribution
#frommchp/$(PIC32_INCLUDE_S): $(PIC32_INC_DIR)/$(PIC32_INCLUDE_S)
#	@printf "CP $(PIC32_INCLUDE_S)\n"; cp $(SOURCEFILE) $(TARGETFILE)

else

# no MCU model defined
ee_pic32regs.h: frommchp/$(PIC32_INCLUDE_C)
	@printf "GEN ee_pic32regs.h\n"
	@printf "/* Automatically generated from Makefile */\n" > ee_pic32regs.h
	@printf "/* WARNING! No MCU model selected! */\n" >> ee_pic32regs.h

# chris: I found no *.inc file in the c32 distribution
#ee_pic32regs.inc: frommchp/$(PIC32_INCLUDE_S)
#	@printf "GEN ee_pic32regs.inc\n"
#	@printf "; Automatically generated from Makefile \n" > ee_pic32regs.inc
#	@printf "; WARNING! No MCU model selected! \n" >> ee_pic32regs.inc
endif


##
## Locator files
##

#if PIC32_GCCDIR is defined
#loc_gnu.ld: $(PIC32_LINKERDIR)/$(PIC32_LINKERSCRIPT)
#	@printf "LOC\n" ; cp $(PIC32_LINKERDIR)/$(PIC32_LINKERSCRIPT) loc_gnu.ld
loc_gnu.ld: $(PIC32_LINKER_DIR)/$(PIC32_LINKERSCRIPT)
	@printf "LOC\n" ; cp $(PIC32_LINKER_DIR)/$(PIC32_LINKERSCRIPT) loc_gnu.ld

##
## EE Library
##

libee.a: $(LIBEEOBJS)
	@printf "AR  libee.a\n" ;
	$(QUIET)$(EE_AR) rs libee.a $(LIBEEOBJS)


##
## Automatic Generation of dependencies
##

dependencies=$(subst .o,.d,$(ALLOBJS))
ifneq ($(call iseeopt, NODEPS), yes) 
ifneq ($(MAKECMDGOALS),clean)
-include $(dependencies)
endif
endif


##
## Directories
##

.PHONY: make_directories

# Objects depend on directories, but they are not remade if directories change
$(ALLOBJS): | make_directories

make_directories: $(OBJDIRS)

# Directories are (re)created only when some of them don't exist already
$(OBJDIRS):
	@echo "MAKE_DIRECTORIES"
	$(QUIET) mkdir -p $(OBJDIRS)


# the eeopt file is generated when dealing with MPLAB IDE!
#
# this is a phony because the source code does not depend on this file
# and its content higly depends on the EEOPT variables...
#
.PHONY: generate_eeopt
generate_eeopt:
	@printf "MPLAB - eeopt.h\n"
	@echo // This part of the include file includes all the options > eeopt.h
	@echo // which are typically inserted with >> eeopt.h
	@echo // the -D compiler directive. >> eeopt.h
	@echo // This part is automatically generated by the makefile only for MPLABIDE. >> eeopt.h
	@for x in $(EEOPT) $(MPLAB_INCLUDE_REGS); do \
		echo \#ifndef $${x}      >> eeopt.h; \
		echo \#define $${x}      >> eeopt.h; \
		echo \#endif             >> eeopt.h; \
	done;
