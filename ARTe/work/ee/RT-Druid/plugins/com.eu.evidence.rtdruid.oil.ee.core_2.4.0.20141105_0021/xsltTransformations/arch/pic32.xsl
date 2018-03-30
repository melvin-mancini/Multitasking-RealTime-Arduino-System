<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.1">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  

    <!-- ============================================================================================================ -->
    <!--                              Microchip PIC32 XSLT rules for 'eecfg.h'                                        -->
    <!-- TODO: say more!                                                                                              -->
    <!-- ============================================================================================================ -->
    <xsl:template match="cpu[./os/cpu_data/@cpu_type='PIC32']" mode="arch_build_eecfg_h">
/*******************************************************************************
 *   Architecture Specific Defines: Microchip PIC32 (CPU <xsl:value-of select="count(../cpu) - 1 - count(following-sibling::*)"/>)
 ******************************************************************************/
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                              Microchip PIC32 XSLT rules for 'eecfg.c'                                        -->
    <!-- TODO: say more!                                                                                              -->
    <!-- ============================================================================================================ -->
    <xsl:template match="cpu[./os/cpu_data/@cpu_type='PIC32']" mode="arch_build_eecfg_c">
/*******************************************************************************
 *   Architecture Specific: Microchip PIC32 (CPU <xsl:value-of select="count(../cpu) - 1 - count(following-sibling::*)"/>)
 ******************************************************************************/
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                              Microchip PIC32 XSLT rules for 'makefile'                                       -->
    <!-- TODO: say more!                                                                                              -->
    <!-- ============================================================================================================ -->
    <xsl:template match="cpu[./os/cpu_data/@cpu_type='PIC32']" mode="arch_build_makefile_opt_auto_cpu">
###<xsl:text> </xsl:text>
### CPU: Microchip PIC32<xsl:text> </xsl:text>
###<xsl:text> </xsl:text>
EEOPT += __PIC32__<xsl:text> </xsl:text>
        <xsl:if test="os/cpu_data/parameter[@Name='ICD2']/enumerator[@Name='TRUE']">
EEOPT += __PIC32_ICD2__<xsl:text> </xsl:text>
        </xsl:if>
    </xsl:template>

    <xsl:template match="cpu[./os/cpu_data/@cpu_type='PIC32']" mode="arch_build_makefile_toolchain">
ifndef PIC32_ASMDIR
PIC32_ASMDIR := $(shell cygpath `cygpath -ms "c:\Programmi\Microchip\MPLAB ASM32 Suite"`)
endif
ifndef PIC32_GCCDIR
PIC32_GCCDIR := $(shell cygpath `cygpath -ms "c:\Programmi\Microchip\MPLAB C32"`)
endif
    </xsl:template>

</xsl:stylesheet>
