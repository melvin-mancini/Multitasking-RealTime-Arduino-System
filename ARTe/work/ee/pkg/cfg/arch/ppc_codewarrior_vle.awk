#!/usr/bin/gawk -f

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

# Script to convert assembly files to C with inline assembly for the CodeWarrior
# compiler (used for automatic VLE translation)
## Author: 2011 Bernardo  Dal Seno


# `state' values:
# 0 = normal (copy)
# 1 = function header (ignore)
# 2 = function body (copy)
# 3 = ignore this line and then goto normal
# 4 = variable declaration marker (ignore)
# 5 = variable declaration in C (copy)
# 6 = variable declaration in assembly (ignore)

BEGIN {
    state = 0
    print "#define __ASM_CONVERTED_TO_C__ 1"
    print "#include <ee_internal.h>"
}

# Function header marker
/^[[:space:]]*\/\* CW-FUNC-DEFINITION: .+ \*\/[[:space:]]*/ {
    match($0, "[[:space:]]*/\\* CW-FUNC-DEFINITION: +(.+) \\*/", m);
    printf("__asm %s\n{\n\tnofralloc\n", m[1]);
    state = 1;
}

# size directive: end of a function or a variable declaration
/^[[:space:]]*\.size[[:space:]]+[0-9a-zA-Z_]+[[:space:]]*,.+/ {
    if (state == 2) {
	printf("}\n");
	state = 3;
    } else if (state == 6) {
	state = 3;
    }
}

# Variable declaration marker
/^[[:space:]]*\/\* CW-VAR-DEFINITION:/ {
    match($0, "[[:space:]]*/\\* CW-VAR-DEFINITION: +([^*]+|\\*+[^*/])+", m);
    printf("%s\n", m[1]);
    state = 4;
}

# End of variable declaration
/\*\// {
    if (state == 5) {
	# Print last line
	p = match($0, "(.*[^[:space:]*])[[:space:]]*\\*+/", m);
	if (p > 0)
	    printf("%s\n", m[1]);
    }
    if (state == 4 || state == 5)
	state = 6;
}

/^[[:space:]]\.(text|bss|extern)/ {
    if (state == 0)
	state = 3;
}

/^[[:space:]](\.section[[:space:]]|text_section\()[[:space:]]*([.0-9a-zA-Z_]+)/ {
    if (state == 0) {
	match($0, "^[[:space:]](\\.section[[:space:]]|text_section\\()[[:space:]]*([.0-9a-zA-Z_]+)", m)
	printf("#pragma push\n#pragma section code_type \"%s\"\n#pragma pop\n", m[2]);
	state = 3;
    }
}

# Normal lines: copy or ignore them
{
    if (state == 0 || state == 2 || state == 5)
	print $0;
    else if (state == 3)
	state = 0;
    else if (state == 4)
	state = 5;
}

# Label: beginning of a function
/^[0-9a-zA-Z_]+:/ {
    if (state == 1)
	state = 2;
}
