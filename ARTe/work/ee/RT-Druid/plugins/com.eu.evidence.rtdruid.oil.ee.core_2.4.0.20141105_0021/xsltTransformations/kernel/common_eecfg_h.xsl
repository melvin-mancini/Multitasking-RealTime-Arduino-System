<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.1">
<!--                xmlns:retis_xslt='retis_xslt_python/retis_xslt' xsl:exclude-result-prefixes='retis_xslt'-->
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  
        <!-- xsl:variable name="output_main" select="retis_xslt:register_output_file(concat($base_dir, '/', $main_filename))"/ -->

    <!-- ============================================================================================================ -->
    <!--                                    Common XSLT rules for 'eecfg.h'                                           -->
    <!-- TODO: say more!                                                                                              -->
    <!-- ============================================================================================================ -->
    <xsl:template match="/" mode="common_build_eecfg_h">
        
        <xsl:apply-templates select="cpu" mode="common_build_eecfg_h"/> 

/*******************************************************************************
 *   Common Defines (All CPUs)
 ******************************************************************************/
/* CPUs */
#define EE_MAX_CPU <xsl:value-of select="count(cpu)"/>
<!-- FIXME change this hard-coded value for EE_CURRENTCPU -->
#define EE_CURRENTCPU 0 /* FIXME: This was hard-coded by the kernel/common_eecfg_h.xsl template */

        <xsl:apply-templates select="/" mode="arch_build_eecfg_h"/> 
<xsl:text>
</xsl:text>
        
    </xsl:template>

    <xsl:template match="cpu" mode="common_build_eecfg_h">
/*******************************************************************************
 *   Common Defines (CPU <xsl:value-of select="count(../cpu) - 1 - count(following-sibling::*)"/>)
 ******************************************************************************/
/* TASKs declaration */
#define EE_MAX_TASK <xsl:text ></xsl:text><xsl:value-of select="count(task)"/><xsl:text> </xsl:text>
        <xsl:for-each select="task">
#define <xsl:value-of select="@name"/><xsl:text> </xsl:text><xsl:value-of select="position() - 1"/><xsl:text> </xsl:text>
         </xsl:for-each>

/* RESOURCEs declaration */
#define EE_MAX_RESOURCE <xsl:text ></xsl:text><xsl:value-of select="count(resource)"/><xsl:text> </xsl:text>
        <xsl:for-each select="resource">
#define <xsl:value-of select="@name"/><xsl:text> </xsl:text><xsl:value-of select="position() - 1"/><xsl:text> </xsl:text>
         </xsl:for-each>

/* ALARMs declaration */
#define EE_MAX_ALARM <xsl:text ></xsl:text><xsl:value-of select="count(alarm)"/><xsl:text> </xsl:text>
        <xsl:for-each select="alarm">
#define <xsl:value-of select="@name"/><xsl:text> </xsl:text><xsl:value-of select="position() - 1"/><xsl:text> </xsl:text>
         </xsl:for-each>

/* COUNTERs declaration */
#define EE_MAX_COUNTER <xsl:text ></xsl:text><xsl:value-of select="count(counter)"/><xsl:text> </xsl:text>
        <xsl:for-each select="counter">
#define <xsl:value-of select="@name"/><xsl:text> </xsl:text><xsl:value-of select="position() - 1"/><xsl:text> </xsl:text>
         </xsl:for-each>

/* APPMODEs declaration */
#define EE_MAX_APPMODE <xsl:text ></xsl:text><xsl:value-of select="count(appmode)"/><xsl:text> </xsl:text>
        <xsl:for-each select="appmode">
#define <xsl:value-of select="@name"/><xsl:text> </xsl:text><xsl:value-of select="position() - 1"/><xsl:text> </xsl:text>
        </xsl:for-each>
<xsl:text>
</xsl:text>
    </xsl:template>

</xsl:stylesheet>
