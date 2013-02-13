<?xml version='1.0' encoding='utf-8'?>

<xsl:stylesheet  version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:zs="http://ziposoft.com/ns"
>


  
  <xsl:template name="zs:module_menu_css" >
    <style>@import url(<xsl:value-of select="$rootdir"/>/menu/zs_menu.css);</style>
  </xsl:template>
  <xsl:template name="zs:module_menu_script" >
    <script type="text/javascript" src="{$rootdir}/menu/zs_menu.js"/>
    <script type="text/javascript" src="{$rootdir}/util/util.js"/>
  </xsl:template>
  
  <xsl:template match="zs:menu_bar_text">
    <td class="zs_menutop" >
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates/>
    </td>
  </xsl:template>
  <xsl:template match="zs:menu_bar_button">
    <td class="zs_menutop" >
      <button class="menu_bar_item" >
        <xsl:copy-of select="@*"/>
        <xsl:apply-templates/>
      </button>
    </td>
  </xsl:template>
  <xsl:template match="zs:menu_bool">
    <tr>
      <td class="zs_menu_link" >
          <a class="zs_menu_link" data-mi-type="bool" >
             <xsl:copy-of select="@*"/>
            <xsl:attribute name="onclick">
              zs.menu.onclk_bool(this)
            </xsl:attribute>
        <xsl:value-of select="."/><span style="display:none">&#x2713;</span>
        </a>     
      </td>
    </tr>
  </xsl:template>
  <xsl:template match="zs:menu_opt">
    <tr>
      <td class="zs_menu_link" >
          <a class="zs_menu_link" data-mi-type="opt" >
             <xsl:attribute name="data-mi-val">
              <xsl:value-of select="@val"/>
            </xsl:attribute> 
            <xsl:attribute name="onclick">
              zs.menu.onclk_opt(this)
            </xsl:attribute>
        <xsl:value-of select="."/><span style="display:none">&#x2713;</span>
        </a>     
      </td>
    </tr>
  </xsl:template>	
	  <xsl:template match="zs:menu_opt_sel">
    <tr>
      <td  data-mi-type="opt_sel" class="zs_menu_item_sub" onmouseover="zs.menu.show_sub(this,event);" onmouseout="zs.menu.hide_sub(this,event);">
        <xsl:copy-of select="@id"/>
				<xsl:value-of select="@title"/>:<span />&#9658;<table class="zs_menu_sub" cellspacing="0" cellpadding="0">
          <xsl:apply-templates/>
        </table>
      </td>
    </tr>
  </xsl:template>		
  <xsl:template match="zs:menu_link">
    <tr>
      <td class="zs_menu_link" >
        <a class="zs_menu_link"  >
          <xsl:copy-of select="@*"/>
          <xsl:apply-templates/>
        </a>
      </td>
    </tr>
  </xsl:template>
  <xsl:template match="zs:menu_bar">
    <table class="zs_menu_bar" cellspacing="0" cellpadding="0">
      <tr>
        <xsl:apply-templates/>
      </tr>
    </table>
  </xsl:template>

  <xsl:template match="zs:menu_sub">
    <tr>
      <td class="zs_menu_item_sub" onmouseover="zs.menu.show_sub(this,event);" onmouseout="zs.menu.hide_sub(this,event);">
        <xsl:value-of select="@title"/>&#9658;<table class="zs_menu_sub" cellspacing="0" cellpadding="0">
          <xsl:apply-templates/>
        </table>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="zs:menu">
    <td class="zs_menutop" align="left" onmouseover="zs.menu.show(this,event);" onmouseout="zs.menu.hide(this,event);">
      <xsl:value-of select="@title"/>
      <div >
        <table class="zs_menu" cellspacing="0" cellpadding="0" border="0">
          <xsl:apply-templates/>
        </table>
      </div>
    </td>
  </xsl:template>
</xsl:stylesheet>

