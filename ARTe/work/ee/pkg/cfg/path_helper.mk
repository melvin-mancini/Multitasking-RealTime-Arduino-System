# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2009  Evidence Srl
# 
# This file is part of ERIKA Enterprise.
# 
# ERIKA Enterprise is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation, 
# (with a special exception described below).
# 
# Linking this code statically or dynamically with other modules is
# making a combined work based on this code.  Thus, the terms and
# conditions of the GNU General Public License cover the whole
# combination.
# 
# As a special exception, the copyright holders of this library give you
# permission to link this code with independent modules to produce an
# executable, regardless of the license terms of these independent
# modules, and to copy and distribute the resulting executable under
# terms of your choice, provided that you also meet, for each linked
# independent module, the terms and conditions of the license of that
# module.  An independent module is a module which is not derived from
# or based on this library.  If you modify this code, you may extend
# this exception to your version of the code, but you are not
# obligated to do so.  If you do not wish to do so, delete this
# exception statement from your version.
# 
# ERIKA Enterprise is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License version 2 for more details.
# 
# You should have received a copy of the GNU General Public License
# version 2 along with ERIKA Enterprise; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
# ###*E*###

## Authors:
## 2012 Nicola Serreli
## 2013 Errico Guidieri


############################################################################
#
# Workaround to remove cygwin warning messages about the usage of DOS-like paths
# that is produced the first time a DOS-like path is used:
#  - run as soon as possible a shell command that uses a path and drop 
#    the error message.
# Here is an example, using 'test' command
#  es:  $(shell test -d "c:\\dummy\\path" 2>/dev/null)
#
# Usually, check_and_set_xxx_compiler_path is called in first stages of makefile
# and it contains a test command that already drop the error message, than there
# is no need to explicitly invoke a shell command to drop the message.
#
# Note: CYGWIN=nodosfilewarning option has to be set before the first execution
#       of cygwin programs, than it cannot be set here
#
############################################################################
#$(shell test -d "c:\\dummy\\path" 2>/dev/null)


############################################################################
#
# Compiler checks
#
############################################################################


#
# Checks if a path exists and then sets the variable required by Erika makefiles.
# The resulted path is transformed using cygwin functions
#
# 1-> container name (es. PIC30_GCC). Cannot be empty.
# 2-> native compiler path (es c:/Program Files). Can be empty.
#
# es -> $(eval $(call check_and_set_cygwin_compiler_path,PIC30_GCC,C:\\Program Files))
#
define check_and_set_cygwin_compiler_path

ifndef $(1)

    ifeq ($$(strip $(2)),)
        # usually this happens if RT-Druid was not able to find the default value.
        # Even if is deprecated, for some cases it is possible to search the compiler
        # using the PATH.

        # list here all special cases (es. a b c)
        ifneq (,$$(findstring $$(strip $(1)),PPC_DIAB_BASEDIR))
            $$(warning The environment variable $(1) is unset.)
            $$(warning The compiler will be searched using the PATH)
        else
            $$(error Please set the compiler path into $(1) variable before run the makefile)
        endif
    else
        CHECK_COMPILER_PATH := $$(shell test -d "$(2)" 2>/dev/null && echo "0")
        ifneq ($$(CHECK_COMPILER_PATH),0)
            $$(error Compiler not found. Provided path is '$(2)')
        endif
        CHECK_COMPILER_PATH :=
        $(1) := $$(shell cygpath `cygpath -ms '$(2)'`)
    endif
    
else
    CHECK_COMPILER_PATH := $$(shell test -d "$$($(1))" 2>/dev/null && echo "0")
    ifneq ($$(CHECK_COMPILER_PATH),0)
        $$(error Compiler not found. Provided path is '$$($(1))')
    endif
    CHECK_COMPILER_PATH :=
endif

endef # check_and_set_cygwin_compiler_path

#
# Checks if a path exists and then sets the variable required by Erika makefiles
#
# 1-> container name (es. PIC30_GCC). Cannot be empty
# 2-> native compiler path (es /opt/Program Files). Can be empty.
#
# es -> $(eval $(call check_and_set_linux_compiler_path,PIC30_GCC,/opt/Program Files))
#
define check_and_set_linux_compiler_path

ifndef $(1)

    ifeq ($$(strip $(2)),)
        # usually this happens if RT-Druid was not able to find the default value.
        # Even if is deprecated, for some cases it is possible to search the compiler
        # using the PATH.

        # list here all special cases (es. a b c)
        ifneq (,$$(findstring $$(strip $(1)),PPC_DIAB_BASEDIR))
            $$(warning The environment variable $(1) is unset.)
            $$(warning The compiler will be searched using the PATH)
        else
            $$(error Please set the compiler path into $(1) variable before run the makefile)
        endif
    else
        CHECK_COMPILER_PATH := $$(shell test -d "$(2)" 2>/dev/null && echo "0")
        ifneq ($$(CHECK_COMPILER_PATH),0)
            $$(error Compiler not found. Provided path is '$(2)')
        endif
        CHECK_COMPILER_PATH :=
        $(1) := $(2)
    endif
    
else
    CHECK_COMPILER_PATH := $$(shell test -d "$$($(1))" 2>/dev/null && echo "0")
    ifneq ($$(CHECK_COMPILER_PATH),0)
        $$(error Compiler not found. Provided path is '$$($(1))')
    endif
    CHECK_COMPILER_PATH :=
endif

endef # check_and_set_linux_compiler_path


############################################################################
#
# Mico32 Platform's path
#
############################################################################

#
# Checks if the platform path exists as it is written, otherwise tries to move to the parent folder and check again.
# This is required to be compatible with the "old way" to compile Erika mico32 projects, that uses the project path
# as "current working directory", while the "new way" runs from the RT-Druid output folder. 
#
# 1-> container name (es. PLATFORM_LIB_PATH). Cannot be empty
# 2-> platfrom path (es /opt/Program Files). Can be empty.
#
# es -> $(eval $(call check_and_set_mico32_platform_path,PLATFORM_LIB_PATH,../../platforms/fpg_eye_diamond/library))
#
define check_and_set_mico32_platform_path

ifndef $(1)
    ifeq ($$(strip $(2)),)
        $$(error Please set the platform path into $(1) variable before run the makefile)
    else
        CHECK_PLATFORM_PATH := $$(shell test -d "$(2)" 2>/dev/null && echo "0")
        ifneq ($$(CHECK_PLATFORM_PATH),0)
            CHECK_PLATFORM_PATH := $$(shell test -d "../$(2)" 2>/dev/null && echo "0")
        	ifneq ($$(CHECK_PLATFORM_PATH),0)
                $$(error Platform not found. Provided path is '$(2)')
            else
                $(1) := ../$(2)
            endif
        else
            $(1) := $(2)
        endif
        CHECK_PLATFORM_PATH :=
    endif
    
else
    CHECK_PLATFORM_PATH := $$(shell test -d "$$($(1))" 2>/dev/null && echo "0")
    ifneq ($$(CHECK_PLATFORM_PATH),0)
        CHECK_PLATFORM_PATH := $$(shell test -d "../$$($(1))" 2>/dev/null && echo "0")
        ifneq ($$(CHECK_PLATFORM_PATH),0)
            $$(error Platform not found. Provided path is '$$($(1))')
        else
            $(1) := ../$$($(1))
        endif
    endif
    CHECK_PLATFORM_PATH :=
endif

endef # check_and_set_mico32_platform_path

# Functions to test EEOPT and LIB_OPT, useful in conditionals and in or/and expressions
# Examples:
#   ifeq ($(call iseeopt,__MULTI__), yes)
#   ifeq ($(and $(call iseeopt,__MULTI__), $(call iseeopt,__IRQ_STACK_NEEDED__)), yes)
#   ifneq ($(call iseeopt,__MULTI__), yes)
iseeopt = $(if $(filter $1,$(EEOPT)),yes,)

islibopt = $(if $(filter $1,$(LIB_OPT)),yes,)

# `native_path' is used to convert Unix-style names to native names
ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
# Sed is used to remove trailing backslash and to double internal backslashes
native_path = "$(shell cygpath -w '$(strip $1)' | sed -e 's/\\$$//' -e 's/\\/\\\\/g')"
else

# native_path is supposed to return a path string; `strip' removes leading or trailing white chars
native_path = $(strip $1)

endif

ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
#unix_path = $(shell cygpath -u -a '$1' | sed -e 's/ /\\ /g')
unix_path = $(shell cygpath -u -a '$1')
else
unix_path = $1
endif

ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
unix_relpath = $(shell cygpath -u '$1')
else
unix_relpath = $1
endif

ifeq ($(call iseeopt, __RTD_CYGWIN__), yes)
short_native_path = $(shell cygpath -w -s '$(strip $1)' | sed -e 's/\\$$//' -e 's/\\/\\\\/g')
else
short_native_path = $(strip $1)
endif

