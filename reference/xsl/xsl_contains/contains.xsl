<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet  version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>  	
      <xsl:template match="*"/> 
	<xsl:template match="testpage">
		<html >
			<head>
				<title>test</title>
			</head>
			<body onload="init()" >
				<h1>testing:</h1>
        <xsl:apply-templates/>
			</body>
		</html>
	</xsl:template>
     <xsl:template match="simple">
 <p><xsl:value-of select="@name"/></p>
  </xsl:template>		
     <xsl:template match="simple[contains(@name,'layer_')]">
 <h1>LAYER=<xsl:value-of select="substring-after(@name, 'layer_')"/></h1>
  </xsl:template>	

</xsl:stylesheet>
