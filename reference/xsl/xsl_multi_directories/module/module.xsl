<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet  version="1.1"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
  
<xsl:param name="rootdir">/default</xsl:param>  
  <xsl:template name="module_menu">
          <style type="text/css">
          @import url(module.css);
        </style>   
  </xsl:template>
   	<xsl:template match="rootdirshow">
    rootdir= <xsl:value-of select="$rootdir"/>
    </xsl:template>
  
</xsl:stylesheet>
