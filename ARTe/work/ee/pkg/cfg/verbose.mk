# ###*B*###
# ERIKA Enterprise - a tiny RTOS for small microcontrollers
# 
# Copyright (C) 2002-2008  Evidence Srl
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

## Author: 2004 Paolo Gai
## CVS: $Id: verbose.mk,v 1.8 2007/02/01 09:04:11 nino Exp $


# do not display too much things...
ifdef VERBOSE
QUIET=
VERBOSE_DEVNULL=
else
QUIET=@
VERBOSE_DEVNULL=>/dev/null
endif


VERBOSE_PRINTDEP=$(QUIET)printf "DEP  $(EXPERIMENT) $(notdir $<)\\n";
VERBOSE_PRINTCC= $(QUIET)printf "CC   $(EXPERIMENT) $(notdir $<)\\n";
VERBOSE_PRINTTCC=$(QUIET)printf "TCC  $(EXPERIMENT) $(notdir $<)\\n";
VERBOSE_PRINTCPP=$(QUIET)printf "CPP  $(EXPERIMENT) $(notdir $<)\\n";
VERBOSE_PRINTASM=$(QUIET)printf "ASM  $(EXPERIMENT) $(notdir $<)\\n";
VERBOSE_PRINTPRE=$(QUIET)printf "PRE  $(EXPERIMENT) $(notdir $<)\\n";

VERBOSE_PRINTCXX=$(QUIET)printf "CXX  $(EXPERIMENT) $(notdir $<)\\n";
VERBOSE_PRINTLD= $(QUIET)printf "LD   $(EXPERIMENT) $(notdir $@)\\n";
VERBOSE_PRINTAR= $(QUIET)printf "AR   $(EXPERIMENT) $(notdir $@)\\n";

#
# ST_Janus related
#

VERBOSE_PRINTDEP0=$(QUIET)printf "0 DEP $(notdir $<)\\n";
VERBOSE_PRINTCC0 =$(QUIET)printf "0 CC  $(notdir $<)\\n";
VERBOSE_PRINTTCC0=$(QUIET)printf "0 TCC $(notdir $<)\\n";
VERBOSE_PRINTCPP0=$(QUIET)printf "0 CPP $(notdir $<)\\n";
VERBOSE_PRINTASM0=$(QUIET)printf "0 ASM $(notdir $<)\\n";
VERBOSE_PRINTPRE0=$(QUIET)printf "0 PRE $(notdir $<)\\n";

VERBOSE_PRINTDEP1=$(QUIET)printf "1 DEP $(notdir $<)\\n";
VERBOSE_PRINTCC1 =$(QUIET)printf "1 CC  $(notdir $<)\\n";
VERBOSE_PRINTTCC1=$(QUIET)printf "1 TCC $(notdir $<)\\n";
VERBOSE_PRINTCPP1=$(QUIET)printf "1 CPP $(notdir $<)\\n";
VERBOSE_PRINTASM1=$(QUIET)printf "1 ASM $(notdir $<)\\n";
VERBOSE_PRINTPRE1=$(QUIET)printf "1 PRE $(notdir $<)\\n";

#
# AVR8 related
#

VERBOSE_PRINTEEP= $(QUIET)printf "EEP  $(EXPERIMENT) $(notdir $<)\\n";
VERBOSE_PRINTHEX= $(QUIET)printf "HEX  $(EXPERIMENT) $(notdir $<)\\n";
VERBOSE_PRINTLSS= $(QUIET)printf "LSS  $(EXPERIMENT) $(notdir $<)\\n";
VERBOSE_PRINTSREC=$(QUIET)printf "SREC $(EXPERIMENT) $(notdir $<)\\n";
