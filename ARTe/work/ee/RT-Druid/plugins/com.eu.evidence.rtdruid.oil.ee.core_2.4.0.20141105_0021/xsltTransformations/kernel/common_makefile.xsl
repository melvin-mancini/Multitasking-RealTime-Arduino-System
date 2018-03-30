<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.1">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  

    <!-- ============================================================================================================ -->
    <!--                                   Common XSLT rules for 'makefile'                                           -->
    <!-- TODO: say more!                                                                                              -->
    <!-- ============================================================================================================ -->
    <xsl:template match="/" mode="common_build_makefile">
        
##                ##
## Erika Makefile ##
##                ##

EEOPT = 
################################################################################
#    User options
################################################################################
        <xsl:apply-templates select="/" mode="common_build_makefile_opt_user"/> 
################################################################################
#    Automatic options
################################################################################
        <xsl:apply-templates select="/" mode="common_build_makefile_opt_auto"/> 
        <xsl:apply-templates select="/" mode="arch_build_makefile_opt_auto_cpu"/> 
        <xsl:apply-templates select="/" mode="arch_build_makefile_opt_auto_mcu"/> 
        <xsl:apply-templates select="/" mode="arch_build_makefile_opt_auto_board"/> 
################################################################################
#    Tool-chain
################################################################################
<!-- TODO: take this from the XML ouptut (produced by the druid)! -->
ifndef EEBASE
EEBASE := $(shell cygpath `cygpath -ms "D:\work\Evidence\RT-Druid\1_5_0\trunk\src\open_ee\ee_base"`)
endif
APPBASE := ..
OUTBASE := Debug
        <xsl:apply-templates select="/" mode="arch_build_makefile_toolchain"/> 

################################################################################
#    Architecture specific
################################################################################
        <xsl:apply-templates select="/" mode="arch_build_makefile_extra_cpu"/> 
        <xsl:apply-templates select="/" mode="arch_build_makefile_extra_mcu"/> 
        <xsl:apply-templates select="/" mode="arch_build_makefile_extra_board"/> 

################################################################################
#    Flags
################################################################################
<!-- TODO: take this from the XML ouptut (produced by the druid)! -->
CFLAGS  +=
ASFLAGS +=
LDFLAGS +=
LDDEPS  +=
LIBS    +=

################################################################################
#    Sources
################################################################################
APP_SRCS += eecfg.c<xsl:text> </xsl:text>
        <xsl:apply-templates select="/" mode="common_build_makefile_src"/> 

################################################################################
#    Iclude erika makefiles
################################################################################
include $(EEBASE)/pkg/cfg/rules.mk
<xsl:text>
</xsl:text>
        
    </xsl:template>

    <xsl:template match="os" mode="common_build_makefile_opt_user">
        <xsl:for-each select="./ee_opt">
EEOPT += <xsl:value-of select="@value"/><xsl:text> </xsl:text>
        </xsl:for-each>
    </xsl:template>

    <xsl:template match="cpu" mode="common_build_makefile_opt_auto">
###<xsl:text> </xsl:text>
### Architecture idependent<xsl:text> </xsl:text>
###<xsl:text> </xsl:text>
<!-- TODO: Where can I read this OPT? -->
EEOPT += __RTD_CYGWIN__<xsl:text> </xsl:text>
EEOPT += __<xsl:value-of select="os/kernel_type/@kernel_name"/>__<xsl:text> </xsl:text>
        <xsl:choose>
            <xsl:when test="os/cpu_data[@multi_stack ='TRUE']">
EEOPT += __MULTI__<xsl:text> </xsl:text>
            </xsl:when>
            <xsl:when test="os/cpu_data[@multi_stack ='FALSE']">
EEOPT += __MONO__<xsl:text> </xsl:text>
            </xsl:when>
        </xsl:choose>
        <xsl:if test="os/kernel_type[@nested_irq='TRUE']">
EEOPT += __ALLOW_NESTED_IRQ__<xsl:text> </xsl:text>
        </xsl:if>
        <xsl:if test="alarm[1]">
EEOPT += __ALARM__<xsl:text> </xsl:text>
        </xsl:if>
    </xsl:template>

    <xsl:template match="cpu_data" mode="common_build_makefile_src">
        <xsl:for-each select="./app_src">
APP_SRCS += <xsl:value-of select="@file_name"/><xsl:text> </xsl:text>
        </xsl:for-each>
    </xsl:template>

</xsl:stylesheet>
