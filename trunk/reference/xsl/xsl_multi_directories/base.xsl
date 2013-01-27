<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet  version="1.1"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>  	

	<xsl:template match="testpage">
		<html >
			<head>
				<title>test</title>
          <style type="text/css">
          @import url(base.css);
        </style>      
			</head>
			<body onload="" >
        <xsl:apply-templates/>
			</body>
		</html>
	</xsl:template>
  <xsl:template match="@*|node()">
      <xsl:copy>
        <xsl:apply-templates select="@*|node()"/>
      </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
