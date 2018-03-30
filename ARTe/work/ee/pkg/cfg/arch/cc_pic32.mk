ifeq ($(call iseeopt, __PIC32__), yes)

PIC32_EXTENSION := elf
ifeq ($(wildcard $(PIC32_GCCDIR)/bin/xc32*),)
PIC32_GCCPREFIX := pic32-
else
PIC32_GCCPREFIX := xc32-
endif

BINDIR_CYG := /usr/bin
BINDIR_C32   := $(PIC32_GCCDIR)/bin

BINDIR_ASM      := $(BINDIR_C32)
BINDIR_CC       := $(BINDIR_C32)
BINDIR_BINUTILS := $(BINDIR_C32)

BINDIR_DEP := $(BINDIR_C32)
PIC32_DEPPREFIX := $(PIC32_GCCPREFIX)

ifndef EE_LINK
EE_LINK:=$(BINDIR_BINUTILS)/$(PIC32_GCCPREFIX)gcc 
#EE_LINK:=$(BINDIR_BINUTILS)/$(PIC32_GCCPREFIX)ld
endif

ifndef EE_ASM
EE_ASM:=$(BINDIR_ASM)/$(PIC32_GCCPREFIX)as
endif

ifndef EE_CC
EE_CC:=$(BINDIR_CC)/$(PIC32_GCCPREFIX)gcc
endif

ifndef EE_DEP
EE_DEP:=$(BINDIR_DEP)/$(PIC32_DEPPREFIX)gcc
endif

ifndef EE_AR
EE_AR:=$(BINDIR_BINUTILS)/$(PIC32_GCCPREFIX)ar
endif

ifndef EE_NM
EE_NM:=$(BINDIR_BINUTILS)/$(PIC32_GCCPREFIX)nm
endif

ifndef EE_OBJCOPY
EE_OBJCOPY:=$(BINDIR_BINUTILS)/$(PIC32_GCCPREFIX)objcopy
endif

ifndef EE_OBJDUMP
EE_OBJDUMP:=$(BINDIR_BINUTILS)/$(PIC32_GCCPREFIX)objdump
endif

ifndef EE_BIN2HEX
EE_BIN2HEX:=$(BINDIR_BINUTILS)/$(PIC32_GCCPREFIX)bin2hex
endif

INTERNAL_PKGBASEDIR := -I$(call native_path,$(PKGBASE)) -I$(call native_path,$(APPBASE)) -I.
ALLINCPATH += $(INTERNAL_PKGBASEDIR)

## OPT_CC are the options for compiler invocation
OPT_CC = -O2 -Wall -Winline -x c
ifeq ($(call iseeopt, DEBUG), yes)
OPT_CC += -ggdb 
endif
# Specific option from the application makefile
OPT_CC += $(CFLAGS)

# target type, not used for dependencies if the Cygwin GCC is used
ifneq ($(PIC32_MODEL),)
OPT_CC += -mprocessor=$(PIC32_MODEL)
endif

# #OPT_ASM are the options for asm invocation
OPT_ASM = -Ifrommchp
#ifeq ($(call iseeopt, DEBUG), yes)
#OPT_ASM += --gstabs
#endif
# Specific option from the application makefile
OPT_ASM += $(ASFLAGS)

## OPT_LINK represents the options for armlink invocation
OPT_LINK = 
# Specific option from the application makefile
OPT_LINK += $(LDFLAGS)
ifeq ($(call iseeopt, DEBUG), yes)
OPT_LINK += -mdebugger
endif

# NOTE: if the model is specified, use the specific linker script 
#       NECESSARY to use more than 32K of ram in the new families (e.g 7xx)
# NOTE2: if the model is specified, it must be used passed also the specific
# processor value to the linker script in order to work properly
# FIXME: this generate some warning with the linux compiler (seems it has a
#        redaclaration - necessary - with respect to the GENERIC linker script)
ifneq ($(PIC32_MODEL),)
#OPT_LINK += --script $(PIC32_GCCDIR)/pic32mx/lib/proc/$(PIC32_MODEL)/procdefs.ld
OPT_LINK += -mprocessor=$(PIC32_MODEL)
endif

# OPT_AR: options for library generation
OPT_AR = rs


# Defining EEOPT Macros
# Each identifier that is listed in EEOPT is also inserted as a 
# command-line macro in the compiler...

#DEFS_ASM = $(addprefix -D, $(EEOPT) )
#DEFS_CC  = $(addprefix -D, $(EEOPT) )
# NOTE: the pic32 processors include files define the DEBUG symbol as register
#       name, thus we CANNOT pass -DDEBUG from the compiler command line
#       TODO: maybe we should do the same for all the architectures by simply
#             renaming all DEBUG in the ERIKA tree with EE_DEBUG and adapting
#             the RT-Druid. Do the same with VERBOSE?
DEFS_ASM = $(addprefix -D, $(patsubst DEBUG, EE_DEBUG, $(EEOPT)))
DEFS_CC  = $(addprefix -D, $(patsubst DEBUG, EE_DEBUG, $(EEOPT)))

ifeq ($(call iseeopt, __BIN_DISTR), yes) 
# Note: the defines used in EEOPT to compile the library
# are already added in the eecfg.h
DEFS_ASM += -D__CONFIG_$(EELIB)__
DEFS_CC  += -D__CONFIG_$(EELIB)__
endif

# Automatic dependency generation
ifeq ($(call iseeopt, NODEPS), yes)
DEPENDENCY_OPT = 
make-depend =
else # NODEPS
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Dependencies on Windows need path translation
DEPENDENCY_OPT = -MMD -MF $(call native_path,$(subst .o,.d_tmp,$@)) -MP -MT $@
make-depend = sed -e 's_\\\(.\)_/\1_g' -e 's_\<\([a-zA-Z]\):/_/cygdrive/\l\1/_g' < $3_tmp > $3 && rm $3_tmp
else # __RTD_CYGWIN__
DEPENDENCY_OPT = -MMD -MF $(subst .o,.d_tmp,$@) -MP -MT $@
make-depend = mv $3_tmp $3
endif # __RTD_CYGWIN__
endif # NODEPS

endif # __PIC32__
