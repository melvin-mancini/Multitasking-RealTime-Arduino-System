<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  

    <!-- Architecture independent templates -->
    <xsl:include href="kernel/common_makefile.xsl"/> 

    <xsl:template match="/" mode="build_makefile">
    <OUTPUT_BUFFER>
        <xsl:attribute name="name">makefile</xsl:attribute>
        <xsl:apply-templates select="/" mode="common_build_makefile"/> 
    </OUTPUT_BUFFER>
    </xsl:template>
</xsl:stylesheet>
