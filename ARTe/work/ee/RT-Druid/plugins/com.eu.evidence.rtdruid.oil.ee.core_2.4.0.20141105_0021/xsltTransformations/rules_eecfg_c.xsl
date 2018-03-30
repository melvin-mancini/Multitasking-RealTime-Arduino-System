<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  

    <!-- Architecture independent templates -->
    <xsl:include href="kernel/common_eecfg_c.xsl"/> 

    <xsl:template match="/" mode="build_eecfg_c">
    <OUTPUT_BUFFER>
        <xsl:attribute name="name">eecfg.c</xsl:attribute>
        <xsl:apply-templates select="/" mode="common_build_eecfg_c"/> 
    </OUTPUT_BUFFER>
    </xsl:template>

</xsl:stylesheet>
