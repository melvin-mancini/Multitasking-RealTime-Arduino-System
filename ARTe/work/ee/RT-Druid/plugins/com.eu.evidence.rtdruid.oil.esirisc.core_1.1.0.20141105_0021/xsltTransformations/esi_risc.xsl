<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.1">
 <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  

    <!-- ============================================================================================================ -->
    <!--                              OS    (eecfg.c, stack definition)                                               -->
    <!-- ============================================================================================================ -->
<xsl:template match="object[@Type='OS']/parameter[@Name='CPU_DATA']/enumerator[@Name='ESI_RISC']/parameter[@Name='MULTI_STACK']/enumerator[@Name='TRUE']" mode="build_eecfg_c">
#include "ee.h"

// STACK configuration
<xsl:if test="parameter[@Name='IRQ_STACK']/enumerator[@Name='TRUE']/parameter[@Name='SYS_SIZE']">
#define STACK_IRQ_SIZE <xsl:value-of select="parameter[@Name='IRQ_STACK']/enumerator[@Name='TRUE']/parameter[@Name='SYS_SIZE']/@CurrValue"/><xsl:text> </xsl:text>
int EE_stack_IRQ[STACK_IRQ_SIZE];

/* stack used only by IRQ handlers */
struct EE_TOS EE_std_IRQ_tos = {
    (EE_ADDR)(&amp;EE_stack_IRQ[STACK_IRQ_SIZE - 1])
};

</xsl:if>

<xsl:for-each select="../../../../../object[@Type='TASK']/parameter[@Name='STACK']/enumerator[@Name='PRIVATE']/parameter[@Name='SYS_SIZE']">
#define STACK_<xsl:value-of select="../../../@Name"/>_ID <xsl:value-of select="position()" />U
#define STACK_<xsl:value-of select="../../../@Name"/>_SIZE <xsl:value-of select="@CurrValue"/>
int EE_stack_<xsl:value-of select="../../../@Name"/>[STACK_<xsl:value-of select="../../../@Name"/>_SIZE];
</xsl:for-each>


const EE_UREG EE_std_thread_tos[EE_MAX_TASK+1] = {
        0U, /* dummy */<xsl:for-each select="../../../../../object[@Type='TASK']"><xsl:choose><xsl:when test="parameter[@Name='STACK']/enumerator[@Name='PRIVATE']/parameter[@Name='SYS_SIZE']">
        STACK_<xsl:value-of select="@Name"/>_ID,</xsl:when><xsl:otherwise>
        0U,</xsl:otherwise></xsl:choose>
</xsl:for-each>
};


struct EE_TOS EE_std_system_tos[<xsl:value-of select="count(../../../../../object[@Type='TASK']/parameter[@Name='STACK']/enumerator[@Name='PRIVATE']/parameter[@Name='SYS_SIZE'])+1"/>] = {
        {0}, /* shared */<xsl:for-each select="../../../../../object[@Type='TASK']/parameter[@Name='STACK']/enumerator[@Name='PRIVATE']/parameter[@Name='SYS_SIZE']">
        {(EE_ADDR)(&amp;EE_stack_<xsl:value-of select="../../../@Name"/>[STACK_<xsl:value-of select="../../../@Name"/>_SIZE - 1])},</xsl:for-each>
};


EE_UREG EE_std_active_tos = 0U; /* dummy */
</xsl:template> 
    
    
    <!-- ============================================================================================================ -->
    <!--                              OS    (eecfg.h)                                                                 -->
    <!-- ============================================================================================================ -->
    
    
<xsl:template match="object[@Type='OS']/parameter[@Name='CPU_DATA']/enumerator[@Name='ESI_RISC']" mode="build_eecfg_h">
</xsl:template>

<xsl:template match="object[@Type='OS']/parameter[@Name='CPU_DATA']/enumerator[@Name='ESI_RISC']" mode="build_makefile">
# Configuration for <xsl:value-of select="@Name"/>
EEOPT += __ESI_RISC__

</xsl:template>
    
    <!-- ============================================================================================================ -->
    <!--                              MCU         (makefie, add a define)                                             -->
    <!-- ============================================================================================================ -->
<xsl:template match="object[@Type='OS']/parameter[@Name='MCU_DATA']/enumerator[@Name='ESI_RISC']/parameter[@Name='MODEL']/enumerator[@Name='CUSTOM']" mode="build_makefile">
<xsl:if test="parameter[@Name='MODEL']">
CFLAGS  += <xsl:value-of select="parameter[@Name='MODEL']/@CurrValue"/>
ASFLAGS += <xsl:value-of select="parameter[@Name='MODEL']/@CurrValue"/>
LDFLAGS += <xsl:value-of select="parameter[@Name='MODEL']/@CurrValue"/>
</xsl:if>
#
</xsl:template>
    

    <!-- ============================================================================================================ -->
    <!--                             STARTING POINT                                                                   -->
    <!-- ============================================================================================================ -->

<xsl:template match="application">
<CODE_GENERATION>
<OUTPUT_BUFFER name="eecfg.h">
		<xsl:apply-templates select="/" mode="build_eecfg_h"/> 
</OUTPUT_BUFFER>
<OUTPUT_BUFFER name="eecfg.c">
        <xsl:apply-templates select="/" mode="build_eecfg_c"/>
</OUTPUT_BUFFER>
<OUTPUT_BUFFER name="makefile">
        <xsl:apply-templates select="/" mode="build_makefile"/>
</OUTPUT_BUFFER>
</CODE_GENERATION>
</xsl:template>

</xsl:stylesheet>
