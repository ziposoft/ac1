<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet  version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:zs="http://ziposoft.com/ns"
>
 <xsl:include href="../zs_menu.xsl"/>
   	<xsl:output
  method="html"
omit-xml-declaration="yes"
	encoding="UTF-8"
	indent="yes" />   
  <xsl:template match="zs:testpage">
    <html >
      <head>
        <title>test</title>
        <xsl:apply-templates select="zs:module" mode="head"/>
      </head>
      <body onload="" >
        <xsl:apply-templates/>
      </body>
    </html>
  </xsl:template>
  <xsl:template match="*"/>
</xsl:stylesheet>