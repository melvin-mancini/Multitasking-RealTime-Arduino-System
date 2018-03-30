## ARTE - TEST
##
## Author: 2015,  Marco Pagani

## Check the arte library selection
ifeq ($(findstring __LIB_ARTE__,$(LIB_OPT)),__LIB_ARTE__)

INCLUDE_PATH += $(EEBASE)/contrib/arte/inc

##
## Application files
##

EE_SRCS += contrib/arte/src/arte_tools.c

endif
