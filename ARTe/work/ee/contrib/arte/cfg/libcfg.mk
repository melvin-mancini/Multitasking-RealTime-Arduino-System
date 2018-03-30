## ARTE - TEST
##
## Author: 2015,  Marco Pagani

## Check the Console library selection
ifeq ($(findstring __LIB_ARTE__,$(LIB_OPT)) , __LIB_ARTE__)
INCLUDE_ARTE = YES
endif

## Check if it's selected all libs inclusion
ifeq ($(call iseeopt, __BUILD_ALL_LIBS__), yes)
INCLUDE_ARTE = YES
endif

##
## If the library is required
##
ifeq ($(INCLUDE_ARTE), YES)

##
## Library code
##

## Add the inc path to the include pathlist
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes) 
ALLINCPATH += -I"$(shell cygpath -w $(EEBASE)/contrib/arte/inc)"
else
ALLINCPATH += -I$(EEBASE)/contrib/arte/inc
endif
## New include mechanism
INCLUDE_PATH += $(EEBASE)/contrib/arte/inc

endif
