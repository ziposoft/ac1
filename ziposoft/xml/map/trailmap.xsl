<?xml version='1.0' encoding='utf-8'?>

<xsl:stylesheet  version="1.0"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
 xmlns:svg="http://www.w3.org/2000/svg"
 xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"
 xmlns:zs="http://ziposoft.com/ns"
                 
>
	<xsl:param name="rootdir">..</xsl:param> 

  
  
  <xsl:output
  method="html"
omit-xml-declaration="yes"
	encoding="UTF-8"
	indent="yes" />
  
  
  
      <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>
  <!--
    <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>   -->
  
  <xsl:template match="svg:svg">
    <html >
      <head>
        <title>Umstead Trail Map</title>
        <xsl:call-template name="zs:module_menu_css" />

        <!--
			  <link rel="stylesheet" type="text/css" href="trailmap.css"/>    
        <style>
        <xsl:copy-of select="document('trailmap.css')"/>  </style>      -->
        <style   type="text/css">  @import url(trailmap.css); </style>
      </head>

      <body onload="init()" >
        <!-- status/menu bar-->
        
        <div id="header" style="background-color:#888">
         <xsl:apply-templates select="document('trailmap_menu.xml')"/>
   
          <table id="debug_table" visibility="hidden" rules="all" style="display: none;border: solid 1px black">
            <tr>
              <td>window=</td>
              <td id="stat_window">--</td>
              <td>svgrect=</td>
              <td id="stat_svgrect">--</td>
              <td>viewBox=</td>
              <td >
                <input onchange="onViewBoxSet()" id="stat_viewBox"  value="--" type="text"/>
              </td>
              <td>Mouse=</td>
              <td id="stat_mouse">--</td>
              <td>viewXY=</td>
              <td id="stat_viewXY">--</td>
              <td>scale=</td>
              <td id="stat_scale">--</td>
              <td>Zoom=</td>
              <td id="browser_info">--</td>

            </tr>
          </table>
        </div>



        <div>
          <!-- 
				<div style="display:inline; "   >
          <h1  style="display:inline;">Total:</h1>
          <h1  style="display:inline;" id="total_distance">0</h1>
    
					<h1 id="routename">routename</h1>
					<input onchange="routename_change(this)" id="routename_input" type="text" name="routename" />
					<table rules="all" style="border: solid 1px black">

						<tbody id="route">
							<tr>
								<td>trail</td>
								<td>section</td>
								<td>distance</td>
							</tr>
							<tr id="segtmp" style="display: none">
								<td>Company Mill</td>
								<td>CM1</td>
								<td>0</td>
							</tr>
						</tbody>


					</table> 
				</div>         -->


          <div style="position:fixed;top:30px;left:0px;width:500px" id="svgwindow" >

            <svg
                id="svgdoc"
                xmlns="http://www.w3.org/2000/svg"
                version="1.1"
                visibility="hidden"
                preserveAspectRatio="xMidYMid meet"
          >
              <xsl:copy-of select="@viewBox"/>
              <xsl:copy-of select="@height"/>
              <g id="layer_group">
                 <xsl:apply-templates  mode="layer_groups" />
              </g>
              <g id="path_mouse_group"/>
              <g id="path_group">
                <xsl:apply-templates  mode="path_groups" />
              </g>


            </svg>
          </div>
        </div>


        <div >
          <xsl:apply-templates select="//svg:g[@inkscape:label='Trails_BB']/svg:path" mode="html"/>
          <xsl:apply-templates select="//svg:g[@inkscape:label='Trails_ST']/svg:path" mode="html"/>
        </div>
      </body>
        <xsl:call-template name="zs:module_menu_script" />
       <script type="text/javascript" src="../util/BrowserDetect.js"/>
        <script type="text/javascript" src="trailmap.js" ></script>

      
    </html>
  </xsl:template>

  <xsl:template match="svg:path" mode="html">
    <xsl:variable name="segname" select="@id" />
    <xsl:variable name="seglength" select="document('Umstead.xml')//segment[@name=$segname]/@length" />
    <xsl:variable name="segdata" select="document('Umstead.xml')//segment[@name=$segname]" />
    <xsl:variable name="trailname" select="document('Umstead.xml')//trail[segment/@name=$segname]/@name" />
    <table class="info_table_hide" >
      <xsl:attribute name="id">info-<xsl:value-of select="@id"/>
      </xsl:attribute>
      <tr>
        <td>Trail Name:</td>
        <td>
          <xsl:value-of select="$trailname"/>
        </td>
      </tr>
      <tr>
        <td>Segment:</td>
        <td>
          <xsl:value-of select="@id"/>
        </td>
      </tr>
      <tr>
        <td>Length:</td>
        <td>
          <xsl:value-of select="$seglength"/>
        </td>
      </tr>
      <tr>
        <td>Desc:</td>
        <td>
          <xsl:copy-of select="$segdata"/>
        </td>
      </tr>
    </table>
  </xsl:template>

  <xsl:template match="svg:path" mode="svg" >
    <xsl:param name="trail_type" select="'trail_bb'"/>
    <xsl:variable name="segname" select="@id" />

    <path xmlns="http://www.w3.org/2000/svg"  class="{$trail_type} sel0" onclick="toggle1(this)" onmouseover="on_over(this,evt)" onmouseout="on_out(this,evt)" data-trail-state="0">
      <xsl:copy-of select="@id"/>
      <xsl:copy-of select="@transform"/>
      <xsl:copy-of select="@d"/>
    
      <xsl:attribute name="data-trail-length">
        <xsl:value-of select="document('Umstead.xml')//segment[@name=$segname]/@length"/>
      </xsl:attribute>
    </path>
  </xsl:template>

   <xsl:template match="svg:g[@inkscape:label]" ></xsl:template>   
   <xsl:template match="svg:style" ></xsl:template>   
   <xsl:template match="style" ></xsl:template>   
   <xsl:template match="*"  mode="path_groups" ></xsl:template>   
   <xsl:template match="*"  mode="layer_groups" ></xsl:template>   
  
  
  
  <xsl:template match="svg:g[@inkscape:label='Trails_BB']" mode="path_groups" >
    <xsl:apply-templates select="svg:path" mode="svg">
      <xsl:with-param name="trail_type" select="'trail_bb'"/>
    </xsl:apply-templates>
  </xsl:template>
  <xsl:template match="svg:g[@inkscape:label='Trails_ST']" mode="path_groups" >
    <xsl:apply-templates select="svg:path" mode="svg">
      <xsl:with-param name="trail_type" select="'trail_st'"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="svg:g[contains(@inkscape:label,'layer_')]"  mode="layer_groups"  >
    <g  xmlns="http://www.w3.org/2000/svg"   visibility="hidden">
			    
      <xsl:attribute name="id">
        <xsl:value-of select="@inkscape:label"/>
      </xsl:attribute>
      <xsl:apply-templates/>
    </g>
  </xsl:template>   
  <xsl:template match="svg:use">
    <xsl:copy-of select="."/>
  </xsl:template>
  
   <xsl:include href="../menu/zs_menu.xsl"/>

  <!--
  

-->
</xsl:stylesheet>
