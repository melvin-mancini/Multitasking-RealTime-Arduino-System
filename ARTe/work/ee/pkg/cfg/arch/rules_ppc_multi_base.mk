# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers

# Copyright (C) 2002-2011  Evidence Srl

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

## Author: 2011 Bernardo  Dal Seno

export EEBASE
GLOBAL_LINKSCRIPT = shared_sym.ld
SLAVE_CPUS := $(filter-out CPU_MASTER, $(CPU_LIST))

ifeq ($(call iseeopt, __E200ZX_EXECUTE_FROM_RAM__), yes)
START_SH = start.sh
MULTI_FLASH_T32_SCRIPT = 
else
START_SH = start_flash.sh
MULTI_FLASH_T32_SCRIPT = rom_multi.cmm
endif

.PHONY: all clean

.DEFAULT_GOAL := all
all: $(foreach c, $(CPU_LIST), $(c)-all) $(START_SH) config-mc.t32 start-mc.cmm $(MULTI_FLASH_T32_SCRIPT) exec_lauterbach.sh

clean: $(foreach c, $(CPU_LIST), $(c)-clean)

# Create a pattern rule for each CPU; one pattern for `all', and one for `clean'
define all-clean-template
 .PHONY: $(1)-all $(1)-clean
 $(1)-all $(1)-clean: $(1)-%:
	$(MAKE) -C $($(1)_DIR) ELFNAME=$($(1)_ELF)  		\
		CPU_NUMID=$($(1)_ID) 				\
		GLOBAL_LINKSCRIPT=../$(GLOBAL_LINKSCRIPT) $$*
endef
$(foreach c, $(CPU_LIST), $(eval $(call all-clean-template,$c)))

$(foreach s, $(SLAVE_CPUS), $(s)-all): $(GLOBAL_LINKSCRIPT)

$(START_SH): \
 %: $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/multicore/%
	@echo CP $@
	$(QUIET) cp $< $@

config-mc.t32: \
 %: $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/multicore/%
	@echo GEN $@
	$(QUIET) sed -e 's:#T32SYS#:$(T32SYS):g'  $< > $@

ifeq ($(call iseeopt, __E200ZX_EXECUTE_FROM_RAM__), yes)
# This rule works on the assumption that there is one slave CPU
start-mc.cmm: \
 %: $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/multicore/% $(MAKEFILE_LIST)
	@echo GEN $@
	$(QUIET)sed -e 's:#MASTER_DIR#:$(CPU_MASTER_DIR):g'		\
		$(foreach s, $(SLAVE_CPUS),				\
			-e 's:#CPU1_DIR#:$($(s)_DIR):g'			\
			-e 's:#CPU1_EXE_NAME#:$($(s)_ELF):g'		\
			-e 's:#EXECUTE_FROM_FLASH#:0:g'			\
		) $< > $@
else
# This rule works on the assumption that there is one slave CPU
start-mc.cmm: \
 %: $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/multicore/% $(MAKEFILE_LIST)
	@echo GEN $@
	$(QUIET)sed -e 's:#MASTER_DIR#:$(CPU_MASTER_DIR):g'		\
		$(foreach s, $(SLAVE_CPUS),				\
			-e 's:#CPU1_DIR#:$($(s)_DIR):g'			\
			-e 's:#CPU1_EXE_NAME#:$($(s)_ELF):g'		\
			-e 's:#EXECUTE_FROM_FLASH#:1:g'			\
		) $< > $@

rom_multi.cmm: \
 %: $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/multicore/% $(MAKEFILE_LIST)
	@echo GEN $@
	$(QUIET) sed -e 's:#MASTER_ELF#:$(MASTER_ELF):g'  		\
		$(foreach s, $(SLAVE_CPUS),				\
			-e 's:#CPU1_DIR#:$($(s)_DIR):g'			\
			-e 's:#CPU1_EXE_NAME#:$($(s)_ELF):g'		\
		) $< > $@

endif

exec_lauterbach.sh:  %: $(PKGBASE)/mcu/freescale_$(PPC_MCU_MODEL)/cfg/%
	@echo
	@echo "Copying exec_lauterbach.sh..."
	@echo
	$(QUIET)cp $< $@

MASTER_ELF = $(CPU_MASTER_DIR)/$(CPU_MASTER_ELF)

$(MASTER_ELF): CPU_MASTER-all

$(GLOBAL_LINKSCRIPT): $(MASTER_ELF)
	@echo Building shared symbol table
	$(QUIET)objdump -t -w -j ee_mcglobalc -j ee_mcglobald -j ee_mcglobalu \
                -j ee_fast_mcglobalc -j ee_fast_mcglobald \
                -j ee_fast_mcglobalu -j ee_start $< \
		| awk '/^[0-9a-fA-F]+ ......O/ {			\
			match($$0, "^([0-9a-fA-F]+) .+ ([^ ]+)$$", m);	\
			printf("%s = 0x%s;\n", m[2], m[1]) }' > $@

