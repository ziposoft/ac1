<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet  version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:zs="http://ziposoft.com/ns"
  xmlns="http://www.w3.org/1999/xhtml"
  xmlns:html="http://www.w3.org/1999/xhtml"
>


 <xsl:output
  method="html"
omit-xml-declaration="yes"
	encoding="UTF-8"
	indent="yes" />   
  <xsl:template match="zs:testpage">
    <html xmlns="http://www.w3.org/1999/xhtml">
      <head>
        <title>test</title>
        <xsl:apply-templates select="/" mode="module"/>
      <xsl:apply-templates select="zs:script" mode="fixed"/>
      </head>
      <body onload="init()" >
        <xsl:apply-templates/>
      </body>
    </html>
  </xsl:template>
 
  <xsl:template match="/" mode="module">
		what the fuck
		</xsl:template>
  <xsl:template match="zs:script"/>
  <xsl:template match="zs:script" mode="fixed">
    <script  type="text/javascript"><xsl:copy-of select="@src"/><xsl:value-of select="."/></script>
  </xsl:template>

	
</xsl:stylesheet>