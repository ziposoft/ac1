<?xml version='1.0' encoding='utf-8'?>
				
<xsl:stylesheet  version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:zs="http://ziposoft.com/ns"
>
 	  <xsl:template name="zs:module_menu">
      <script type="text/javascript" src="zs_menu.js"/>
      <style>

        .zs_menu_bar
        {
	          background-color: #888;
            cursor: pointer;
	        color: White;
        }
        td.zs_menutop
        {
	        padding: 6px;
	        border-right: 1px groove White;
        }
        .zs_menu, .zs_menu_sub 
        {
        
            font-family: verdana,sans-serif;
            font-size: small;
	          border-collapse: separate;
	          -moz-border-radius: 15px;
	          clear: both;
	          background-color: #888;
	          color: White;
	          border-radius: 15px;
	          padding: 6px;
            margin:0;

	          border-width: 3px;
	          border-color: #000080;
	          border-style: solid;
	          vertical-align: text-bottom;
	          cursor: pointer;
	          position: absolute;
	          display: none; /* width:100%; */
	          z-index:1;
        }
        .zs_menu_item_sub,.zs_menu_item_sub_hi
        {
        
	        border-top: 1px solid #888;
	        border-left: 1px solid #888;
	        cursor: pointer;
	        padding: 4px;
        }
        .zs_menu_item_sub_hi
        {
	        color: blue;
	        background-color: #E5E5E5;
        }
        
        td.zs_menu_item
        {
	        margin: 0px;
	        padding-right: 4px;
        }
        a.zs_menu_item
        {
          font-family: verdana,sans-serif;
          font-size: small;
	        cursor: pointer;
	        margin: 0px;
          border: solid #888;
	        padding: 0px;
	        width: 100%;
	        height: 100%;
          text-decoration: none;
	        display: inline-block;
	        overflow: visible;
	        white-space: nowrap;
        }
        .menu_bar_item
        {
        }
    
        a.zs_menu_item:hover,a.menu_bar_item:hover 
        {
	        color: Blue;
	        background-color: #E5E5E5;
           border:outset White; 
        }
         a.zs_menu_item:active,a.menu_bar_item:active 
        {
	          border:inset White; 
        }                   
    </style>
  </xsl:template>

  
   <xsl:template match="zs:menu_bar_item">
      <td class="zs_menutop" >
        <a class="menu_bar_item" >
       <xsl:copy-of select="@*"/>
      <xsl:apply-templates/>
        </a>
      </td>
  </xsl:template> 
  <xsl:template match="zs:menu_item">
    <tr>
      <td class="zs_menu_item" >
        <a class="zs_menu_item"  >
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

      