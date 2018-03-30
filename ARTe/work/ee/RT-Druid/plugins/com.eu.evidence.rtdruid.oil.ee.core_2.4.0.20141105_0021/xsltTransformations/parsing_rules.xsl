<?xml version="1.0" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<!-- xsl:output method="text" encoding="ASCII" /-->
<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>

    
    <!-- Architecture specific templates -->
    <!-- * CPUs                          -->
    <xsl:include href="arch/pic32.xsl"/>
    <!-- * MCUs                          -->
    <xsl:include href="arch/pic32MX360F512L.xsl"/>  
    <!-- * Boards                        -->
    <!-- xsl:include href="arch/ipermob_mb.xsl"/--> 

    <!-- Output file specific rules -->
    <xsl:include href="rules_eecfg_h.xsl"/> 
    <xsl:include href="rules_eecfg_c.xsl"/> 
    <xsl:include href="rules_makefile.xsl"/> 
    <xsl:template match="/">
<CODE_GENERATION>
        <xsl:apply-templates select="/" mode="build_eecfg_h"/> 
        <xsl:apply-templates select="/" mode="build_eecfg_c"/> 
        <xsl:apply-templates select="/" mode="build_makefile"/> 
</CODE_GENERATION>
    </xsl:template>
</xsl:stylesheet>
