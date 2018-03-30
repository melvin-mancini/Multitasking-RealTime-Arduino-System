<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.1">
<!--                xmlns:retis_xslt='retis_xslt_python/retis_xslt' xsl:exclude-result-prefixes='retis_xslt'-->
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" omit-xml-declaration="no"/>  
        <!-- xsl:variable name="output_main" select="retis_xslt:register_output_file(concat($base_dir, '/', $main_filename))"/ -->

    <!-- ============================================================================================================ -->
    <!--                                             OIL ROOT NODE: 'CPU'                                             -->
    <!-- CPU <name> {                                                                                                 -->
    <!--     ...                                                                                                      -->
    <!-- };                                                                                                           -->
    <!-- ============================================================================================================ -->
    <xsl:template match="application">
        <cpu>
            <xsl:attribute name="name"> 
                 <xsl:value-of select="@Name"/> 
            </xsl:attribute>
            <xsl:apply-templates/>
        </cpu>
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                                           OIL 'OS' OBJECT in CPU                                             -->
    <!-- OS <name> {                                                                                                  -->
    <!--     ...                                                                                                      -->
    <!-- };                                                                                                           -->
    <!-- ============================================================================================================ -->
    <xsl:template match="object[@Type='OS']">
        <os>
            <xsl:attribute name="name"> 
                 <xsl:value-of select="@Name"/> 
            </xsl:attribute>
            <xsl:apply-templates/>
        </os>
    </xsl:template>
    
    <xsl:template match="parameter[@Name='EE_OPT']">
        <ee_opt>
            <xsl:attribute name="value"> 
                 <xsl:value-of select="@CurrValue"/> 
            </xsl:attribute>
        </ee_opt>
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                                       OIL 'CPU_DATA' SECTION in OS                                           -->
    <!-- CPU_DATA <cpu_type> {                                                                                        -->
    <!--     ...                                                                                                      -->
    <!-- };                                                                                                           -->
    <!-- ============================================================================================================ -->
    <xsl:template match="parameter[@Name='CPU_DATA']">
        <cpu_data>
            <xsl:attribute name="cpu_type"> 
                 <xsl:value-of select="enumerator/@Name"/> 
            </xsl:attribute>
            <xsl:if test="enumerator/parameter[@Name='MULTI_STACK']">
                <xsl:attribute name="multi_stack"> 
                    <xsl:value-of select="enumerator/parameter[@Name='MULTI_STACK']/enumerator/@Name"/> 
                </xsl:attribute>
            </xsl:if>
            <xsl:apply-templates/>
            <xsl:copy-of select="./enumerator/descendant::*[position() = 1 and @Name!='MULTI_STACK' and @Name!='APP_SRC']"/> 
            <xsl:copy-of select="./enumerator/descendant::*[position() = 1]/following-sibling::*[@Name!='MULTI_STACK' and @Name!='APP_SRC']"/> 
        </cpu_data>
    </xsl:template>

    <xsl:template match="parameter[@Name='APP_SRC']">
        <app_src>
            <xsl:attribute name="file_name"> 
                 <xsl:value-of select="@CurrValue"/> 
            </xsl:attribute>
        </app_src>
    </xsl:template>

    <xsl:template match="parameter[@Name='LIB']">
        <lib>
            <xsl:if test="enumerator[@Name='ENABLE']">
                <xsl:attribute name="name"> 
                    <xsl:value-of select="enumerator[@Name='ENABLE']/parameter[@Name='NAME']/@CurrValue"/> 
                </xsl:attribute>
                <xsl:attribute name="enabled">true</xsl:attribute>
            </xsl:if>
            <!--xsl:copy-of select="./enumerator//*"/--> 
            <xsl:copy-of select="./enumerator/descendant::*[position() = 1]"/> 
            <xsl:copy-of select="./enumerator/descendant::*[position() = 1]/following-sibling::*"/> 
        </lib>
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                                       OIL 'MCU_DATA' SECTION in OS                                           -->
    <!-- MCU_DATA <mcu_type> {                                                                                        -->
    <!--     ...                                                                                                      -->
    <!-- };                                                                                                           -->
    <!-- ============================================================================================================ -->
    <xsl:template match="parameter[@Name='MCU_DATA']">
        <mcu_data>
            <xsl:attribute name="mcu_type"> 
                 <xsl:value-of select="enumerator/@Name"/> 
            </xsl:attribute>
            <xsl:if test="enumerator/parameter[@Name='MODEL']">
                <xsl:attribute name="model"> 
                    <xsl:value-of select="enumerator/parameter[@Name='MODEL']/enumerator/@Name"/> 
                </xsl:attribute>
            </xsl:if>
        </mcu_data>
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                                       OIL 'BOARD_DATA' SECTION in OS                                         -->
    <!-- BOARD_DATA <board_type> {                                                                                    -->
    <!--     ...                                                                                                      -->
    <!-- };                                                                                                           -->
    <!-- ============================================================================================================ -->
    <xsl:template match="parameter[@Name='BOARD_DATA']">
        <board_data>
            <xsl:attribute name="board_type"> 
                 <xsl:value-of select="enumerator/@Name"/> 
            </xsl:attribute>
            <xsl:copy-of select="./enumerator/descendant::*[position() = 1]"/> 
            <xsl:copy-of select="./enumerator/descendant::*[position() = 1]/following-sibling::*"/> 
        </board_data>
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                                      OIL 'KERNEL_TYPE' SECTION in OS                                         -->
    <!-- KERNEL_TYPE <kernel_name> {                                                                                  -->
    <!--     ...                                                                                                      -->
    <!-- };                                                                                                           -->
    <!-- ============================================================================================================ -->
    <xsl:template match="parameter[@Name='KERNEL_TYPE']">
        <kernel_type>
            <xsl:attribute name="kernel_name"> 
                 <xsl:value-of select="enumerator/@Name"/> 
            </xsl:attribute>
            <xsl:if test="enumerator/parameter[@Name='NESTED_IRQ']">
                <xsl:attribute name="nested_irq"> 
                    <xsl:value-of select="enumerator/parameter[@Name='NESTED_IRQ']/enumerator/@Name"/> 
                </xsl:attribute>
            </xsl:if>
            <xsl:copy-of select="./enumerator/descendant::*[position() = 1 and @Name!='NESTED_IRQ']"/> 
            <xsl:copy-of select="./enumerator/descendant::*[position() = 1]/following-sibling::*[@Name!='NESTED_IRQ']"/> 
        </kernel_type>
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                                           OIL 'TASK' OBJECT in CPU                                           -->
    <!-- TASK <name> {                                                                                                -->
    <!--     ...                                                                                                      -->
    <!-- };                                                                                                           -->
    <!-- ============================================================================================================ -->
    <xsl:template match="object[@Type='TASK']">
        <task>
            <xsl:attribute name="name"> 
                 <xsl:value-of select="@Name"/> 
            </xsl:attribute>
            <xsl:if test="parameter[@Name='PRIORITY']">
                <xsl:attribute name="priority"> 
                    <xsl:value-of select="parameter[@Name='PRIORITY']/@CurrValue"/> 
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="parameter[@Name='REL_DEADLINE']">
                <xsl:attribute name="rel_deadline"> 
                    <xsl:value-of select="parameter[@Name='REL_DEADLINE']/@CurrValue"/> 
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="parameter[@Name='STACK']">
                <xsl:attribute name="stack"> 
                    <xsl:value-of select="parameter[@Name='STACK']/enumerator/@Name"/> 
                </xsl:attribute>
            </xsl:if>
            <xsl:if test="parameter[@Name='SCHEDULE']">
                <xsl:attribute name="schedule"> 
                    <xsl:value-of select="parameter[@Name='SCHEDULE']/enumerator/@Name"/> 
                </xsl:attribute>
            </xsl:if>
        </task>
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                                         OIL 'COUNTER' OBJECT in CPU                                          -->
    <!-- COUNTER <name> {                                                                                             -->
    <!--     ...                                                                                                      -->
    <!-- };                                                                                                           -->
    <!-- ============================================================================================================ -->
    <xsl:template match="object[@Type='COUNTER']">
        <counter>
            <xsl:attribute name="name"> 
                 <xsl:value-of select="@Name"/> 
            </xsl:attribute>
        </counter>
    </xsl:template>

    <!-- ============================================================================================================ -->
    <!--                                          OIL 'ALARM' OBJECT in CPU                                           -->
    <!-- ALARM <name> {                                                                                               -->
    <!--     ...                                                                                                      -->
    <!-- };                                                                                                           -->
    <!-- ============================================================================================================ -->
    <xsl:template match="object[@Type='ALARM']">
        <alarm>
            <xsl:attribute name="name"> 
                 <xsl:value-of select="@Name"/> 
            </xsl:attribute>
            <xsl:if test="parameter[@Name='COUNTER']">
                <xsl:attribute name="counter_name"> 
                    <xsl:value-of select="parameter[@Name='COUNTER']/@CurrValue"/> 
                </xsl:attribute>
            </xsl:if>
            <xsl:apply-templates/>
        </alarm>
    </xsl:template>
    
    <xsl:template match="parameter[@Name='ACTION']/enumerator[@Name='ACTIVATETASK']">
        <activate_task>
            <xsl:if test="parameter[@Name='TASK']">
                <xsl:attribute name="task_name"> 
                    <xsl:value-of select="parameter[@Name='TASK']/@CurrValue"/> 
                </xsl:attribute>
            </xsl:if>
        </activate_task>
    </xsl:template>

</xsl:stylesheet>
