<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet  version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
 xmlns="http://www.w3.org/1999/xhtml"
> 
 <xsl:output
  method="html"
omit-xml-declaration="yes"
	encoding="UTF-8"
	indent="yes" /> 	
	
<xsl:template match="simple">
 <p>as simple as it gets</p>
  </xsl:template>	
	<xsl:template match="testpage">
		<html >
			<head>
				<title>test</title>
			</head>
			<body onload="init()" >
        <xsl:apply-templates/>
			</body>
		</html>
	</xsl:template>

      <xsl:template match="*"/>
</xsl:stylesheet>
