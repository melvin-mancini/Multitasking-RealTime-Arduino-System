### fake-druid makefile
#
# Author: 2011  Giuseppe Serano

### Setting Erika's variables:
# ERIKA_FILES= ../../..
# APPBASE = .
# 
# EEOPT =


RTDRUID_CONFIGURATOR_NUMBER:=1277


############################################################################
#
# User options
#
############################################################################
EEOPT += DEBUG
EEOPT += __ASSERT__
EEOPT += __USE_SYSTICK__
EEOPT += __ADD_LIBS__
EEOPT += __USE_ATMEL_CMSIS__
EEOPT += __USE_ATMEL_SAM_DEV__
EEOPT += __USE_ATMEL_SAM_DEV_ALL__

############################################################################
#
# Automatic options
#
############################################################################
EEOPT += __RTD_CYGWIN__
EEOPT += __CORTEX_MX__
EEOPT += __CORTEX_M3__
EEOPT += __ATMEL__
EEOPT += __SAM3__
EEOPT += __SAM3X8E__
EEOPT += __GNU__
EEOPT += __FP__
EEOPT += __MONO__
EEOPT += __ALARMS__
EEOPT += __ALLOW_NESTED_IRQ__
EEOPT += __DISABLE_EEOPT_DEFINES__

############################################################################
#
# Erika base directory
#
############################################################################
ifdef ERIKA_FILES
ifdef EEBASE
    $(warning EEBASE is set, but it has been overridden by ERIKA_FILES)
endif
EEBASE := $(shell cygpath `cygpath -ms '${ERIKA_FILES}'`)

else # ERIKA_FILES

ifndef EEBASE
        EEBASE := $(shell cygpath `cygpath -ms '%%EE_SRC%%'`)
else
        $(warning The usage of EEBASE is deprecated. Please use ERIKA_FILES)
endif
endif # ERIKA_FILES
# ERIKA_FILES has fulfilled its role. Make sure it's not used inside Erika makefiles
ERIKA_FILES :=
$(info Using erika files in $(EEBASE))

############################################################################
#
# CORTEX M3
#
############################################################################
include $(EEBASE)/pkg/cfg/path_helper.mk
APPBASE := 
OUTBASE := Debug

$(eval $(call check_and_set_cygwin_compiler_path,GNU_ARM_ROOT,%%COMPILER%%))
#$(eval $(call check_and_set_linux_compiler_path,GNU_ARM_ROOT,/opt/arm/gcc-arm-none-eabi-4_7-2013q1))

############################################################################
#
# Init MCU for CORTEX MX (LM4F232xxxx)
#
############################################################################
ATMEL_MODEL	:= SAM3X8E


############################################################################
#
# Flags
#
############################################################################
CFLAGS  +=
ASFLAGS +=
LDFLAGS +=
LDDEPS  +=
LIBS    +=

LIB_OPT  += __LIB_ATMEL_CMSIS__
LIB_OPT  += __LIB_ATMEL_SAM_DEV__

############################################################################
#
# Sources
#
############################################################################
APP_SRCS += eecfg.c


############################################################################
#
# end
#
############################################################################
include $(EEBASE)/pkg/cfg/rules.mk
