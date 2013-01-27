<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet  version="1.1"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>  	
  <xsl:include      href="xsl_include_other.xsl"/>

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
