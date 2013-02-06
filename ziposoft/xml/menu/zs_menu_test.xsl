<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet  version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:zs="http://ziposoft.com/ns"
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:html="http://www.w3.org/1999/xhtml"
>
  
 <xsl:import href="zs_menu.xsl"/>
  
<xsl:param name="rootdir">..</xsl:param> 
  
 <xsl:output
  method="html"
omit-xml-declaration="yes"
	encoding="UTF-8"
	indent="yes" /> 


  
  <xsl:template match="zs:testpage">
    <html>
      <head>
        <title>test</title>
        <xsl:call-template name="zs:module_menu_css" />

      </head>
      <body onload="init()" id="elm1">
        <xsl:apply-templates/>
      </body>
        <xsl:call-template name="zs:module_menu_script" />
       <script  type="text/javascript" src="zs_menu_test.js"></script>
    </html>
  </xsl:template>
 

  
</xsl:stylesheet>