<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.1">
 <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  

    <!-- ============================================================================================================ -->
    <!--                              OS    (eecfg.c, stack definition)                                               -->
    <!-- ============================================================================================================ -->
<xsl:template match="object[@Type='OS']/parameter[@Name='CPU_DATA']/enumerator[@Name='MSP430']/parameter[@Name='MULTI_STACK']/enumerator[@Name='TRUE']" mode="build_eecfg_c">
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
    
    
<xsl:template match="object[@Type='OS']/parameter[@Name='CPU_DATA']/enumerator[@Name='MSP430']" mode="build_eecfg_h">
</xsl:template>

<xsl:template match="object[@Type='OS']/parameter[@Name='CPU_DATA']/enumerator[@Name='MSP430']" mode="build_makefile">
# Configuration for <xsl:value-of select="@Name"/>
EEOPT += __MSP430__

APPBASE := ..
OUTPUT_DIR := .

<!-- Define __RTD_MSP430__ so that the build script knows we are using a real RT-Druid -->
EEOPT += __RTD_MSP430__

</xsl:template>
    
    <!-- ============================================================================================================ -->
    <!--                              MCU         (makefie, add a define)                                             -->
    <!-- ============================================================================================================ -->
<xsl:template match="object[@Type='OS']/parameter[@Name='MCU_DATA']/enumerator[@Name='MSP430']/parameter[@Name='MODEL']/enumerator[@Name='CUSTOM']" mode="build_makefile">
<xsl:if test="parameter[@Name='MODEL']">
CFLAGS  += <xsl:value-of select="parameter[@Name='MODEL']/@CurrValue"/>
ASFLAGS += <xsl:value-of select="parameter[@Name='MODEL']/@CurrValue"/>
LDFLAGS += <xsl:value-of select="parameter[@Name='MODEL']/@CurrValue"/>
</xsl:if>
#
</xsl:template>

<xsl:template match="object[@Type='OS']/parameter[@Name='MCU_DATA']/enumerator[@Name='MSP430']/parameter[@Name='MODEL']/enumerator[@Name='MSP430_1611']" mode="build_makefile">
EEOPT  += __MSP430_1611__
EEOPT  += __MSP430x16__
<xsl:if test="parameter[@Name='USE_UART']/enumerator[@Name='TRUE']">
<xsl:if      test="parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0'] or not(parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1'])">EEOPT  += __USE_EE_UART_0__
</xsl:if>
<xsl:if      test="parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1']">EEOPT  += __USE_EE_UART_1__
</xsl:if>
</xsl:if>
<xsl:if test="parameter[@Name='USE_SPI']/enumerator[@Name='TRUE']">
<xsl:if      test="parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0'] or not(parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1'])">EEOPT  += __USE_EE_SPI_0__
</xsl:if>
<xsl:if      test="parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1']">EEOPT  += __USE_EE_SPI_1__
</xsl:if>
</xsl:if>
<xsl:if test="parameter[@Name='USE_I2C']/enumerator[@Name='TRUE']">
<xsl:if      test="parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0'] or not(parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1'])">EEOPT  += __USE_EE_I2C__
</xsl:if>
<xsl:if      test="parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1'] and not(parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0'])">$(warning i2c disabled because it requires port0)
</xsl:if>
</xsl:if>


<xsl:if test="parameter[@Name='ENABLE_INTERRUPTS']/enumerator[@Name='TRUE']">
<xsl:if   test="parameter[@Name='USE_UART']/enumerator[@Name='TRUE'] or parameter[@Name='USE_SPI']/enumerator[@Name='TRUE']">
<xsl:if      test="parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0'] or not(parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1'])">EEOPT  += __EE_USART_PORT_0_ISR_ENABLE__
</xsl:if>
<xsl:if      test="parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1']">EEOPT  += __EE_USART_PORT_1_ISR_ENABLE__
</xsl:if>
</xsl:if>
<xsl:if   test="parameter[@Name='USE_I2C']/enumerator[@Name='TRUE']">
<xsl:if      test="parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0'] or not(parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1'])">EEOPT  += __EE_I2C_PORT_0_ISR_ENABLE__
</xsl:if>
</xsl:if>
</xsl:if>
#
</xsl:template>

<!--
EEOPT  += __EE_USART_PORT_0_ISR_ENABLE__
</xsl:if>
<xsl:if         test="parameter[@Name='ENABLE_INTERRUPTS']/enumerator[@Name='TRUE']">EEOPT  += __EE_USART_PORT_1_ISR_ENABLE__</xsl:if>


<xsl:if test="parameter[@Name='USE_I2C']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0']">
EEOPT  += __USE_EE_I2C__</xsl:if>
<xsl:if test="parameter[@Name='USE_I2C']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1']">
SEGNALARE UN ERRORE !!!!</xsl:if>
<xsl:if test="parameter[@Name='USE_SPI']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0']">
EEOPT  += __USE_EE_SPI_0_</xsl:if>
<xsl:if test="parameter[@Name='USE_SPI']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1']">
EEOPT  += __USE_EE_SPI_1__</xsl:if>
<xsl:if test="parameter[@Name='USE_TIMER']/enumerator[@Name='TRUE']">
EEOPT  += __USE_EE_TIMER__</xsl:if>

<xsl:if test="parameter[@Name='USE_UART']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0'] and parameter[@Name='ENABLE_INTERRUPTS']/enumerator[@Name='TRUE']">
EEOPT  += __EE_USART_PORT_0_ISR_ENABLE__</xsl:if>
<xsl:if test="parameter[@Name='USE_UART']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1'] and parameter[@Name='ENABLE_INTERRUPTS']/enumerator[@Name='TRUE']">
EEOPT  += __EE_USART_PORT_1_ISR_ENABLE__</xsl:if>
<xsl:if test="parameter[@Name='USE_I2C']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0'] and parameter[@Name='ENABLE_INTERRUPTS']/enumerator[@Name='TRUE']">
EEOPT  += __EE_I2C_PORT_0_ISR_ENABLE__</xsl:if>
<xsl:if test="parameter[@Name='USE_I2C']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1'] and parameter[@Name='ENABLE_INTERRUPTS']/enumerator[@Name='TRUE']">
SEGNALARE UN ERRORE !!!!</xsl:if>
<xsl:if test="parameter[@Name='USE_SPI']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT0'] and parameter[@Name='ENABLE_INTERRUPTS']/enumerator[@Name='TRUE']">
EEOPT  += __EE_USART_PORT_0_ISR_ENABLE__</xsl:if>
<xsl:if test="parameter[@Name='USE_SPI']/enumerator[@Name='TRUE'] and parameter[@Name='ENABLE_PORTS']/enumerator[@Name='PORT1'] and parameter[@Name='ENABLE_INTERRUPTS']/enumerator[@Name='TRUE']">
EEOPT  += __EE_USART_PORT_1_ISR_ENABLE__</xsl:if>
 	-->

    

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
