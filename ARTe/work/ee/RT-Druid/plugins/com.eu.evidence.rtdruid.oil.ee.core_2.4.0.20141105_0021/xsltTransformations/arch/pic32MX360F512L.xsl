<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.1">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  

    <!-- ============================================================================================================ -->
    <!--                              Microchip PIC32 XSLT rules for 'eecfg.h'                                        -->
    <!-- TODO: say more!                                                                                              -->
    <!-- ============================================================================================================ -->
<!--
    <xsl:template match="cpu[./os/cpu_data/@cpu_type='PIC32']" mode="arch_build_eecfg_h">
/*******************************************************************************
 *   Architecture Specific Defines: Microchip PIC32 (CPU <xsl:value-of select="count(../cpu) - 1 - count(following-sibling::*)"/>)
 ******************************************************************************/
    </xsl:template>
-->

    <!-- ============================================================================================================ -->
    <!--                              Microchip PIC32 XSLT rules for 'eecfg.c'                                        -->
    <!-- TODO: say more!                                                                                              -->
    <!-- ============================================================================================================ -->
<!--
    <xsl:template match="cpu[./os/cpu_data/@cpu_type='PIC32']" mode="arch_build_eecfg_c">
/*******************************************************************************
 *   Architecture Specific: Microchip PIC32 (CPU <xsl:value-of select="count(../cpu) - 1 - count(following-sibling::*)"/>)
 ******************************************************************************/
    </xsl:template>
-->

    <!-- ============================================================================================================ -->
    <!--                       Microchip PIC32MX360F512L XSLT rules for 'makefile'                                    -->
    <!-- TODO: say more!                                                                                              -->
    <!-- ============================================================================================================ -->
    <xsl:template match="cpu[./os/mcu_data/@model='PIC32MX360F512L' and ./os/mcu_data/@mcu_type='PIC32']" mode="arch_build_makefile_opt_auto_mcu">
###<xsl:text> </xsl:text>
### MCU: Microchip PIC32MX360F512L<xsl:text> </xsl:text>
###<xsl:text> </xsl:text>
EEOPT += __MICROCHIP_PIC32__<xsl:text> </xsl:text>
EEOPT += __32MX360F512L__<xsl:text> </xsl:text>
    </xsl:template>

    <xsl:template match="cpu[./os/mcu_data/@model='PIC32MX360F512L' and ./os/mcu_data/@mcu_type='PIC32']" mode="arch_build_makefile_extra_mcu">
###<xsl:text> </xsl:text>
### MCU: Microchip PIC32MX360F512L<xsl:text> </xsl:text>
###<xsl:text> </xsl:text>
PIC32_MODEL      := 32MX360F512L<xsl:text> </xsl:text>
PIC32_DEV_LIB    := libmchp_peripheral_32MX360F512L.a<xsl:text> </xsl:text> 
PIC32_INCLUDE_C  := p32xxxx.h<xsl:text> </xsl:text>
    </xsl:template>

</xsl:stylesheet>
