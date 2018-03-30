# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers

# Copyright (C) 2002-2012  Evidence Srl

# This file is part of ERIKA Enterprise.

# ERIKA Enterprise is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation,
# (with a special exception described below).

# Linking this code statically or dynamically with other modules is
# making a combined work based on this code.  Thus, the terms and
# conditions of the GNU General Public License cover the whole
# combination.

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

# ERIKA Enterprise is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License version 2 for more details.

# You should have received a copy of the GNU General Public License
# version 2 along with ERIKA Enterprise; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
# ###*E*###

## Author: 2012 Errico Guidieri

#Used by start scripts
T32TMP ?= C:/Temp

ifneq ($(call iseeopt, EE_BUILD_SINGLE_ELF), yes)
#Used to export symbols to slave cpus
GLOBAL_LINKSCRIPT := shared_sym.lsl
else  # !EE_BUILD_SINGLE_ELF
#Used to strip redundat symbols in Lautherbach windows
T32_STRIPSECTIONS_SCRIPT := stripCPUSections.cmm
endif # !EE_BUILD_SINGLE_ELF

#Slave CPU List
SLAVE_CPUS := $(filter-out CPU_MASTER, $(CPU_LIST))
#Master ELF Target
MASTER_ELF_PATH := $(CPU_MASTER_DIR)/$(CPU_MASTER_ELF)

#Slave ELFs PATH
ifdef CPU1_ELF
CPU1_ELF_PATH := $(CPU1_DIR)/$(CPU1_ELF)
endif #CPU1_ELF
ifdef CPU2_ELF
CPU2_ELF_PATH := $(CPU2_DIR)/$(CPU2_ELF)
endif #CPU2_ELF

#ORTI Support
ifneq ($(wildcard $(CPU_MASTER_DIR)/system.orti),)
MASTER_ORTI_CD      := \&core0 cd $(CPU_MASTER_DIR)
MASTER_ORTI_CMD     := \&core0 do orti.cmm\n\&core0 do markers.cmm
MASTER_ORTI_CD_BACK := \&core0 cd ..
ifdef CPU1_ELF
CPU1_ORTI_CD        := \&core1 cd $(CPU1_DIR)
CPU1_ORTI_CMD       := \&core1 do orti.cmm\n\&core1 do markers.cmm
CPU1_ORTI_CD_BACK   := \&core1 cd ..
endif # CPU1_ELF
ifdef CPU2_ELF
CPU2_ORTI_CD        := \&core2 cd $(CPU2_DIR)
CPU2_ORTI_CMD       := \&core2 do orti.cmm\n\&core2 do markers.cmm
CPU2_ORTI_CD_BACK   := \&core2 cd ..
endif # CPU2_ELF

endif # system.orti

#Preprocessing command
T32_SED_COMMAND = 's-\#MASTER_ELF\#-$(MASTER_ELF_PATH)-g' 's-\#CPU1_ELF\#-$(CPU1_ELF_PATH)-g' 's-\#CPU2_ELF\#-$(CPU2_ELF_PATH)-g'

T32_SED_COMMAND := $(addprefix -e , $(T32_SED_COMMAND)) -e 's-\#ORTICDMASTER\#-$(MASTER_ORTI_CD)-g'\
 -e 's-\#ORTICDCPU1\#-$(CPU1_ORTI_CD)-g' -e 's-\#ORTICDCPU2\#-$(CPU2_ORTI_CD)-g'\
 -e 's-\#ORTICMDMASTER\#-$(MASTER_ORTI_CMD)-g' -e 's-\#ORTICMDCPU1\#-$(CPU1_ORTI_CMD)-g' -e 's-\#ORTICMDCPU2\#-$(CPU2_ORTI_CMD)-g'\
 -e 's-\#ORTICDBACKMASTER\#-$(MASTER_ORTI_CD_BACK)-g' -e 's-\#ORTICDBACKCPU1\#-$(CPU1_ORTI_CD_BACK)-g'\
 -e 's-\#ORTICDBACKCPU2\#-$(CPU2_ORTI_CD_BACK)-g'

ifeq ($(call iseeopt, EE_EXECUTE_FROM_RAM), yes)
T32SCRIPT  := t32_$(TRICORE_MODEL)_mc_ram.cmm
EE_SCRIPTS := config_$(TRICORE_MODEL)_mc.t32 $(TRICORE_MODEL)_mc_start.bat $(TRICORE_MODEL)_mc_start.sh $(T32_STRIPSECTIONS_SCRIPT)
else # EE_EXECUTE_FROM_RAM
T32SCRIPT  := t32_$(TRICORE_MODEL)_mc.cmm
EE_SCRIPTS := config_$(TRICORE_MODEL)_mc.t32 $(TRICORE_MODEL)_mc_start.bat $(TRICORE_MODEL)_mc_start.sh $(TRICORE_MODEL)_mc_flash.bat\
  $(TRICORE_MODEL)_mc_flash.sh t32_$(TRICORE_MODEL)_mc_flash.cmm $(T32_STRIPSECTIONS_SCRIPT)
endif # EE_EXECUTE_FROM_RAM

ifeq ($(call iseeopt, EE_BUILD_SINGLE_ELF),yes)
EE_T32_SCRIPTS_SOURCE_FOLDER = $(PKGBASE)/mcu/infineon_$(TRICORE_MODEL)/cfg/multicore_single_elf
else # EE_BUILD_SINGLE_ELF
EE_T32_SCRIPTS_SOURCE_FOLDER = $(PKGBASE)/mcu/infineon_$(TRICORE_MODEL)/cfg/multicore
endif # EE_BUILD_SINGLE_ELF

.PHONY: all clean

#List of CPU directories
EE_CORE_DIRS = $(CPU_MASTER_DIR)/ $(CPU1_DIR)/
ifneq ($(CPU2_DIR),)
EE_CORE_DIRS += $(CPU2_DIR)/
endif # CPU2_DIR not empty

# FIXME: Add some compiler awareness in master makefile
ifeq ($(call iseeopt, EE_BUILD_SINGLE_ELF),yes)
#XXX: Supported only by gcc compiler (undefined behaviour with other compilers)
# Define the TARGET
TARGET_ELF_NAME ?= $(TRICORE_MODEL)_multicore.elf
.DEFAULT_GOAL := $(TARGET_ELF_NAME)

EEOPT += EE_GNU__

include $(EEBASE)/pkg/cfg/dir.mk
include $(PKGBASE)/cfg/verbose.mk
include $(PKGBASE)/cfg/compiler.mk

# Select input filename format
SOURCEFILE = $(call native_path,$<)
TARGETFILE = $(call native_path,$@)

$(TARGET_ELF_NAME): all $(EE_GLOBAL_OBJS) $(LINKDEP)
	@echo "GLOBAL LD $@";
	$(QUIET)$(EE_LINK) $(OPT_LINK)  $(call target_ld_file,$(TARGETFILE)) $(EE_GLOBAL_OBJS) $(MASTER_ELF_PATH) $(CPU1_ELF_PATH) $(CPU2_ELF_PATH)
	@echo "************************************"
	@echo "Global Compilation terminated successfully!"
else # EE_BUILD_SINGLE_ELF
.DEFAULT_GOAL := all
endif # EE_BUILD_SINGLE_ELF

all: $(foreach c, $(CPU_LIST), $(c)-all) $(EE_SCRIPTS) $(T32SCRIPT)

clean: $(foreach c, $(CPU_LIST), $(c)-clean)

# Create a pattern rule for each CPU; one pattern for `all', and one for `clean'
define all-clean-template
 .PHONY: $(1)-all $(1)-clean
 $(1)-all $(1)-clean: $(1)-%:
	$(MAKE) -C $($(1)_DIR) TARGET_NAME=$(basename $($(1)_ELF)) CPU_NUMID=$($(1)_ID) GLOBAL_LINKSCRIPT=../$(GLOBAL_LINKSCRIPT) $$*
endef

$(foreach c, $(CPU_LIST), $(eval $(call all-clean-template,$c)))

#Targets Dependencies
$(foreach s, $(SLAVE_CPUS), $(s)-all): $(GLOBAL_LINKSCRIPT)

config_$(TRICORE_MODEL)_mc.t32: $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/config_tc2Yx_mc.t32
	@echo "CP $@ from $<"
	$(QUIET) cp $< $@

$(TRICORE_MODEL)_mc_start.bat: $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/tc2Yx_mc_start.bat
	@echo "GEN $@ from $<"
	$(QUIET) sed -e 's-#T32SYS#-$(T32SYS)-g'	\
		-e 's-#T32TMP#-$(T32TMP)-g'				\
		-e 's-#T32SCRIPT#-$(T32SCRIPT)-g'		\
		-e 's-#T32ARCH#-$(T32ARCH)-g'			\
		-e 's-#TC2YX#-$(TRICORE_MODEL)-g'	\
		$< > $@
	$(QUIET) chmod 777 $@

$(TRICORE_MODEL)_mc_start.sh: $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/tc2Yx_mc_start.sh
	@echo "GEN $@ from $<"
	$(QUIET) sed -e 's-#T32SYS#-$(T32SYS)-g'	\
		-e 's-#T32TMP#-$(T32TMP)-g'				\
		-e 's-#T32SCRIPT#-$(T32SCRIPT)-g'		\
		-e 's-#T32ARCH#-$(T32ARCH)-g'			\
		-e 's-#TC2YX#-$(TRICORE_MODEL)-g'	\
		$< > $@
	$(QUIET) chmod 777 $@

$(TRICORE_MODEL)_mc_flash.bat:
	@echo GEN $@
	@echo "@ECHO OFF" > $@
	@echo "REM script to flash TriCore". >> $@
	@echo $(T32SYS)/bin/$(T32ARCH)/t32mtc -s t32_$(TRICORE_MODEL)_mc_flash.cmm >> $@
	$(QUIET) chmod 777 $@

#	@echo GEN $@
#	@echo "@ECHO OFF" > $@
#	@echo "REM script to flash TriCore". >> $@
#ifdef CPU1_DIR
#	@echo cd $(CPU1_DIR) >> $@
#	@echo $(T32SYS)/bin/windows/t32mtc >> $@
#	@echo cd .. >> $@
#endif
#ifdef CPU2_DIR
#	@echo cd $(CPU2_DIR) >> $@
#	@echo $(T32SYS)/bin/windows/t32mtc >> $@
#	@echo cd .. >> $@
#endif
#	@echo cd $(CPU_MASTER_DIR) >> $@
#	@echo $(T32SYS)/bin/windows/t32mtc >> $@
#	@echo cd .. >> $@

$(TRICORE_MODEL)_mc_flash.sh:
	@echo "GEN $@"
	@echo "#! /bin/bash" > $@
	@echo $(T32SYS)/bin/$(T32ARCH)/t32mtc -s t32_$(TRICORE_MODEL)_mc_flash.cmm >> $@
	$(QUIET) chmod 777 $@

$(T32SCRIPT) t32_$(TRICORE_MODEL)_mc_flash.cmm: %: $(EE_T32_SCRIPTS_SOURCE_FOLDER)/%
	@echo "GEN $@ from $<"
	$(QUIET) sed $(T32_SED_COMMAND) $< > $@

stripCPUSections.cmm: $(PKGBASE)/mcu/infineon_common_tc2Yx/cfg/multicore/stripCPUSections.cmm
	@echo "CP $@ from $<"
	$(QUIET) cp $< $@

$(MASTER_ELF_PATH): CPU_MASTER-all

ifneq ($(call iseeopt, EE_BUILD_SINGLE_ELF), yes)
# FIXME: Add some compiler awareness in master makefile
# Generator command for AWK
# XXX: WORKAROUND TO CATCH THAT WE ARE COMPILING WITH HIGHTEC GNU COMPILER
ifneq ($(TRICORE_TASKINGDIR),)
AWK_GEN_CMD = printf("\"%s\" := 0x%s;\n", m[2], m[1])
else
AWK_GEN_CMD = printf("%s = 0x%s;\n", m[2], m[1])
#else ifeq ($(call iseeopt, EE_DIAB__), yes)
#AWK_GEN_CMD = printf("%s = 0x%s;\n", m[2], m[1])
# else #EE_GNU__ I need just the symbol list
# AWK_GEN_CMD = printf("%s\n", m[2])
endif

# I will use objdump always because: the target is an ELF file and,even though TASKING copiler has is own,
# obj utility but I don't know how to get the right output
$(GLOBAL_LINKSCRIPT): $(MASTER_ELF_PATH)
	@echo Building shared symbol table $@
	$(QUIET) objdump -t -w -j ee_mcglobalc -j ee_mcglobald		\
                -j ee_mcglobalu -j ee_fast_mcglobalc			\
                -j ee_fast_mcglobald -j ee_fast_mcglobalu		\
                -j ee_mcglobalt -j ee_kernel_start $<	|		\
		awk '/^[0-9a-fA-F]+ ......[O,F]/ {						\
			match($$0, "^([0-9a-fA-F]+) .+ ([^ ]+)$$", m);		\
			$(AWK_GEN_CMD)	}' > $@
endif # !EE_BUILD_SINGLE_ELF

