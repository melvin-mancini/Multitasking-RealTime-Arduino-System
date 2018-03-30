<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.1">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  

    <!-- ============================================================================================================ -->
    <!--                                    Common XSLT rules for 'eecfg.c'                                           -->
    <!-- TODO: say more!                                                                                              -->
    <!-- ============================================================================================================ -->
    <xsl:template match="/" mode="common_build_eecfg_c">
        
#include "ee.h"
        <xsl:apply-templates select="cpu" mode="common_build_eecfg_c"/> 
        <xsl:apply-templates select="/" mode="arch_build_eecfg_c"/> 
<xsl:text>
</xsl:text>
        
    </xsl:template>

    <xsl:template match="cpu" mode="common_build_eecfg_c">
/*******************************************************************************
 *   Kernel (CPU <xsl:value-of select="count(../cpu) - 1 - count(following-sibling::*)"/>)
 ******************************************************************************/
/* Definition of TASK's body */
        <xsl:for-each select="task">
DeclareTask(<xsl:value-of select="@name"/>);<xsl:text> </xsl:text>
        </xsl:for-each>
/* Thread body look-up table */
const EE_FADDR EE_hal_thread_body[EE_MAX_TASK] = {
        <xsl:for-each select="task">
	(EE_FADDR) Func<xsl:value-of select="@name"/>, /* thread <xsl:value-of select="@name"/> */<xsl:text> </xsl:text>
        </xsl:for-each>
};

/* Ready priority */
<!-- FIXME What is the right way to produce it? -->
/* FIXME: This was created by the 'kernel/common_eecfg_c.xsl' template. What is the right way to produce it? */
const EE_TYPEPRIO EE_th_ready_prio[EE_MAX_TASK] = {
        <xsl:for-each select="task">
	<xsl:value-of select="@priority"/>, /* thread <xsl:value-of select="@name"/> */<xsl:text> </xsl:text>
        </xsl:for-each>
};

/* Dispatch priority */
<!-- FIXME What is the right way to produce it? -->
/* FIXME: This was created by the 'kernel/common_eecfg_c.xsl' template. What is the right way to produce it? */
const EE_TYPEPRIO EE_th_dispatch_prio[EE_MAX_TASK] = {
        <xsl:for-each select="task">
	<xsl:value-of select="@priority"/>, /* thread <xsl:value-of select="@name"/> */<xsl:text> </xsl:text>
        </xsl:for-each>
};

/* Thread status */
<!-- FIXME What is the right way to produce it? -->
/* FIXME: This was created by the 'kernel/common_eecfg_c.xsl' template. What is the right way to produce it? */
#if defined(__MULTI__) || defined(__WITH_STATUS__)
EE_TYPESTATUS EE_th_status[EE_MAX_TASK] = {
        <xsl:for-each select="task">
	EE_READY, /* thread <xsl:value-of select="@name"/> */<xsl:text> </xsl:text>
        </xsl:for-each>
};
#endif

/* Next thread */
EE_TID EE_th_next[EE_MAX_TASK] = {
        <xsl:for-each select="task">
	EE_READY, /* thread <xsl:value-of select="@name"/> */<xsl:text> </xsl:text>
        </xsl:for-each>
};

<!-- FIXME What is the right way to produce it? -->
/* FIXME: These were created by the 'kernel/common_eecfg_c.xsl' template. What is the right way to produce them? */
EE_TYPEPRIO EE_th_nact[EE_MAX_TASK];
EE_TID EE_stkfirst = EE_NIL; /* The first stacked task */
EE_TID EE_rqfirst  = EE_NIL; /* The first task into the ready queue */
EE_TYPEPRIO EE_sys_ceiling= 0x0000; /* system ceiling */

<!-- TODO CREATE THE REL_DEADLINE PART -->
/* TODO: CREATE THE REL_DEADLINE PART */

/*******************************************************************************
 *   Counters (CPU <xsl:value-of select="count(../cpu) - 1 - count(following-sibling::*)"/>)
 ******************************************************************************/
<!-- FIXME What is the right way to produce it? -->
/* FIXME: This was created by the 'kernel/common_eecfg_c.xsl' template. What is the right way to produce it? */
EE_counter_RAM_type EE_counter_RAM[EE_MAX_COUNTER] = {
        <xsl:for-each select="counter">
	{0, 1}, /* counter <xsl:value-of select="@name"/> */<xsl:text> </xsl:text>
        </xsl:for-each>
};

/*******************************************************************************
 *   Alarms (CPU <xsl:value-of select="count(../cpu) - 1 - count(following-sibling::*)"/>)
 ******************************************************************************/
<!-- FIXME What is the right way to produce it? -->
/* FIXME: This was created by the 'kernel/common_eecfg_c.xsl' template. What is the right way to produce it? */
<!-- TODO: why the EE_MAX_ALARM is not used in this case? -->
/* TODO: why the EE_MAX_ALARM is not used in this case? */
const EE_alarm_ROM_type EE_alarm_ROM[] = {
        <xsl:for-each select="alarm">
            <xsl:if test="./activate_task">
	{0, EE_ALARM_ACTION_TASK, <xsl:value-of select="activate_task/@task_name"/>, NULL}<xsl:text> </xsl:text>
            </xsl:if>
        </xsl:for-each>
};

EE_alarm_RAM_type EE_alarm_RAM[EE_MAX_ALARM];
    </xsl:template>

</xsl:stylesheet>
