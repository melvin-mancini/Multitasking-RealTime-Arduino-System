#!/bin/bash

# This script is used to print out information about the positioning of the
# Microchip PIC30 compiler, to avoid that the compilation will fail due to
# a wrongly specified compiler installation directory

# usage:
# cc_pic30_detect.sh ASM dirname
# cc_pic30_detect.sh GCC dirname


############################################################################
#
# Workaround to remove cygwin warning message about the usage of DOS-like paths
# that is produced the first time a DOS-like path is used:
#
# Note: CYGWIN=nodosfilewarning option has to be set before the first execution
#       of cygwin programs, than it cannot be set here
#
############################################################################
test -d "c:\\dummy\\path" 2>/dev/null


############################################################################
#
# Check user's path
#
############################################################################
if test -d "$2"; then
    cygpath `cygpath -ms "$2"`
    exit
fi

############################################################################
#
# Search ASM compiler inside standard paths
#
############################################################################
if [ $1 = "ASM" ]; then
    printf "Looking for the MPLAB ASM30 directory...\n" 1>&2

    if test -d "${PROGRAMFILES}\Microchip\MPLAB ASM30 Suite"; then
	printf "...Using ${PROGRAMFILES}\\Microchip\\MPLAB ASM30 Suite\n" 1>&2
	cygpath `cygpath -ms "${PROGRAMFILES}\Microchip\MPLAB ASM30 Suite"`
	exit
    fi

    if test -d "c:\Program Files\Microchip\MPLAB ASM30 Suite"; then
	printf "...Using c:\\Program Files\\Microchip\\MPLAB ASM30 Suite\n" 1>&2
	cygpath `cygpath -ms "c:\Program Files\Microchip\MPLAB ASM30 Suite"`
	exit
    fi

    if test -d "c:\Programmi\Microchip\MPLAB ASM30 Suite"; then
	printf "...Using c:\\Programmi\\Microchip\\MPLAB ASM30 Suite\n" 1>&2
	cygpath `cygpath -ms "c:\Programmi\Microchip\MPLAB ASM30 Suite"`
	exit
    fi

fi

############################################################################
#
# Search C compiler inside standard paths
#
############################################################################
if [ $1 = "GCC" ]; then
    printf "Looking for the MPLAB C30 directory...\n" 1>&2

    if test -d "${PROGRAMFILES}\Microchip\xc16\v1.00"; then
	echo "...Using ${PROGRAMFILES}\\Microchip\\xc16\\v1.00" 1>&2
	cygpath `cygpath -ms "${PROGRAMFILES}\Microchip\xc16\v1.00"`
	exit
    fi

    if test -d "${PROGRAMFILES}\Microchip\mplabc30\v3.31"; then
	echo "...Using ${PROGRAMFILES}\\Microchip\\mplabc30\\v3.31" 1>&2
	cygpath `cygpath -ms "${PROGRAMFILES}\Microchip\mplabc30\v3.31"`
	exit
    fi

    if test -d "${PROGRAMFILES}\Microchip\mplabc30\v3.30"; then
	echo "...Using ${PROGRAMFILES}\\Microchip\\mplabc30\\v3.30" 1>&2
	cygpath `cygpath -ms "${PROGRAMFILES}\Microchip\mplabc30\v3.30"`
	exit
    fi

    if test -d "${PROGRAMFILES}\Microchip\MPLAB C30 Suite"; then
	printf "...Using ${PROGRAMFILES}\\Microchip\\MPLAB C30 Suite\n" 1>&2
	cygpath `cygpath -ms "${PROGRAMFILES}\Microchip\MPLAB C30 Suite"`
	exit
    fi

    if test -d "c:\Program Files\Microchip\MPLAB C30"; then
	printf "...Using c:\\Program Files\\Microchip\\MPLAB C30\n" 1>&2
	cygpath `cygpath -ms "c:\Program Files\Microchip\MPLAB C30"`
	exit
    fi

    if test -d "c:\Programmi\Microchip\MPLAB C30"; then
	printf "...Using c:\\Programmi\\Microchip\\MPLAB C30\n" 1>&2
	cygpath `cygpath -ms "c:\Programmi\Microchip\MPLAB C30"`
	exit
    fi

fi;

