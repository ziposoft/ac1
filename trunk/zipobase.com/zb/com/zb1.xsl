<?xml version='1.0' encoding='utf-8'?>
<!--
Copyright 2007, ZipoSoft, Inc. (Acme Device Drivers, Inc.)

This file is part of ZipoBase, from ZipoSoft. See ZipoBase.com for more infomation

THIS CODE IS PROVIDED "AS IS". ZIPOSOFT MAKES NO WARRANTIES, EXPRESSED,
IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.
-->

<!DOCTYPE xsl:stylesheet [
  <!ENTITY loz "&#9674;">
  <!ENTITY empty "&#664;">
  <!ENTITY times "&#215;">
  <!ENTITY Xi "&#926;">
  <!ENTITY space "&#160;">
  <!ENTITY bull "&#9679;">
  <!ENTITY arw_left   "&#8592;">
  <!ENTITY arw_right  "&#8594;">
  <!ENTITY arw_down   "&#8595;">
  <!ENTITY arw_up     "&#8593;">
  <!ENTITY wdg_left   "&#9668;">
  <!ENTITY wdg_right  "&#9658;">
  <!ENTITY wdg_down   "&#9660;">
  <!ENTITY wdg_up     "&#9650;">
  <!ENTITY branch     "&#9500;">
]>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output  doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" doctype-system="http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd" method="html"/>


    <xsl:template match="page">

    <!--<html xmlns="http://www.w3.org/1999/xhtml">-->
    <html debug="false">
      <head>

        <!--  <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />-->
        <title>
          <xsl:value-of select="@title"/>
        </title>
        <meta name="description" content="ZipoBase" />
        <meta name="keywords" content="simple, fast, database" />
        <style type="text/css">
          @import url(../<xsl:value-of select="@zb_ver"/>/com/zb.css);
          @import url(../<xsl:value-of select="@zb_ver"/>/com/zb.<xsl:value-of select="@browser"/>.css);
        </style>
      </head>
      <body onload="load()" >
        <xsl:apply-templates/>
      </body>
      <script type="text/javascript" ><xsl:attribute name="src">../<xsl:value-of select="@zb_ver"/>/com/zb.<xsl:value-of select="@browser"/>.js</xsl:attribute>.</script>
      <script type="text/javascript" ><xsl:attribute name="src">../<xsl:value-of select="@zb_ver"/>/com/zb.js</xsl:attribute>.<!--IE cant handle empy script elm--></script>
      <script language="JavaScript" type="text/javascript" ><xsl:attribute name="src">../<xsl:value-of select="@zb_ver"/>/com/drag.js</xsl:attribute>.</script>
    </html>
  </xsl:template>
  <xsl:template name="url">
    <xsl:value-of select="/page/@lnk"/>
    <xsl:value-of select="/page/@cxt"/>
    <xsl:value-of select="@xi"/>
  </xsl:template>
  <xsl:template name="link">
    <xsl:attribute name="href">
      <xsl:call-template name="url"/>
    </xsl:attribute>
  </xsl:template>
  
  
  <!-- TITLE BAR  -->
  <xsl:template match="titlebar">
    <td  class="titlebar"  >
            <xsl:apply-templates select="*"/>
    </td>
  </xsl:template>
  <xsl:template match="tb_title">
    <div class="tb_title">
      <xsl:apply-templates select="*"/>
    </div>
  </xsl:template>
  <xsl:template match="tb_butt">
    <div class="tb_butt">
      <xsl:apply-templates select="*"/>
    </div>
  </xsl:template>
  <xsl:template match="tb_text">
    <div class="tb_text">
      <xsl:apply-templates select="*"/>
    </div>
  </xsl:template> 
  <xsl:template match="tb_link">
        <a class="tb_text">
          <xsl:apply-templates select="@*"/>
          <xsl:value-of select="."/>
        </a>
  </xsl:template>




  <!-- INTERFACE  -->
  <xsl:template match="int-group" >
    <td style="vertical-align:top; " >
      <table cellpadding="0" cellspacing="0" height="100%"   >
        <xsl:apply-templates select="@*" />
        <tr>
          <!-- TAB bar row -->
          <td  >
            <table width="100%" cellpadding="2" cellspacing="0" class="tab-bar">
              <tr>
                <xsl:apply-templates select="tab|int/titlebar"/>
              </tr>
            </table>
          </td>
        </tr>
        <xsl:apply-templates select="int/int-body"/>
      </table>
    </td>
  </xsl:template>
  <xsl:template match="int-body">
    <tr>
      <td >
        <table >
          <xsl:attribute name="class">
            <xsl:value-of select="@class"/>
          </xsl:attribute>
          <tr>
            <td class="menubar" >
              <xsl:apply-templates select ="menubar"/>
            </td>
          </tr>
          <tr>
            <td style="vertical-align:top; " >
              <table  >
                <tr>
                  <td >

                    <table  >
                      <xsl:apply-templates select ="main|msgs"/>
                    </table>
                  </td>
                  <td>
                    <table >
                      <tr>
                        <xsl:apply-templates select ="side/int-group"/>
                      </tr>
                    </table>
                  </td>
                </tr>
              </table>
            </td>

          </tr>
          <xsl:apply-templates select ="int_sub_group"/>
        </table>
      </td>
    </tr>
  </xsl:template>
  <xsl:template match="int_sub_group">
    <tr>
      <td >
        <table>
          <tr>
            <xsl:apply-templates select ="int-group"/>
          </tr>
        </table>
      </td>
    </tr>
  </xsl:template>
  <xsl:template match="msg">
    <tr>
      <td class="msg" >
        <p class="msg">
          <xsl:value-of select="."/>
        </p>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="errmsg">
    <tr>
      <td class="errmsg" >
        <xsl:value-of select="."/>
      </td>
    </tr>
  </xsl:template>
  <xsl:template match="int">
    <table cellpadding="0" cellspacing="0" height="100%"  >
      <xsl:apply-templates select="@*" />
      <tr>
        <xsl:apply-templates select ="titlebar"/>
      </tr>
      <xsl:apply-templates select ="msg"/>
      <xsl:apply-templates select ="errmsg"/>
      <xsl:apply-templates select="int-body|int-bodytree"/>
    </table>
  </xsl:template>
  <xsl:template match="tab">
    <td class="tab"  >
      <div class="rounder_tab" />
      <table class="tab" >
        <tr>
          <xsl:for-each select="*">
            <td >
              <xsl:apply-templates select="self::*"/>
    		</td>
          </xsl:for-each>
        </tr>
      </table>
    </td>
  </xsl:template>


  <xsl:template match="menuslot">
    <tr>
      <td class="menuslot" >
        <xsl:apply-templates/>
      </td>
    </tr>
  </xsl:template>
  <xsl:template match="main">
    <tr>
      <td class="int-main">
        <xsl:apply-templates/>
      </td>
    </tr>
  </xsl:template>


  <xsl:template match="menubar">

    <table cellspacing="0" cellpadding="0">
      <tr>
        <xsl:apply-templates/>
      </tr>
    </table>

  </xsl:template>
  <xsl:template match="da">
    <!--<a onmouseover="_hi(this);" onmouseout="_low(this);">-->
    <a  onclick="stopprop(event);">
      <xsl:apply-templates select="@xi"/>
      <xsl:apply-templates select="@*"/>
      <xsl:value-of select="."/>
    </a>
  </xsl:template>
  <xsl:template match="@xi">
    <xsl:attribute name="href">
      <xsl:value-of select="/page/@lnk"/><xsl:value-of select="/page/@cxt"/>.<xsl:value-of select="."/>
    </xsl:attribute>
  </xsl:template>

  <xsl:template match="mi">
    <tr>
      <td class="mi"  onmouseover="mi_hi(this)" onmouseout="mi_low(this)">
        <a class="mi">
          <xsl:apply-templates select="@*"/>
          <xsl:value-of select="."/>
        </a>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="doublehittest">
    <div>
      <xsl:attribute name="onclick">
        location.href='<xsl:value-of select="@href"/>'
      </xsl:attribute>
      <a >
        <xsl:apply-templates select="@*" />
        <xsl:value-of select="."/>
      </a>
    </div>
    <p>
      <svg:svg xmlns:svg="http://www.w3.org/2000/svg" width="10cm" height="8cm"
        viewBox="0 0 500 400" version="1.1">
        <svg:title>A star</svg:title>
        <svg:polygon style="fill:red; stroke:blue; stroke-width:10"
                     points="210,46 227,96 281,97 238,129
                         254,181 210,150 166,181 182,129
                         139,97 193,97"/>
      </svg:svg>
    </p>

  </xsl:template>

  <xsl:template match="menubar_button" >
    <td class="menutop" >
      <a class="menubar_button">
        <xsl:apply-templates select="@*"/>
        <xsl:value-of select="."/>
      </a>
    </td>
  </xsl:template>

  <xsl:template match="menusub">
    <tr>
      <td class="misub" onmouseover="showsubmenu(this,event);" onmouseout="hidesubmenu(this,event);">
        <xsl:value-of select="@title"/>&wdg_right;<table class="menusub" cellspacing="0" cellpadding="0">
          <xsl:apply-templates/>
        </table>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="menu">
    <td class="menutop" align="left" onmouseover="showmenu(this,event);" onmouseout="hidemenu(this,event);">
      <xsl:value-of select="@title"/>
      <div >
        <table class="menu" cellspacing="0" cellpadding="0" border="0">
          <xsl:apply-templates/>
        </table>
      </div>
    </td>
  </xsl:template>
  <!--
  
  Table Tree
  
  -->
  <xsl:template match="tt_row">
    <a class="tt_row" >
      <xsl:apply-templates select="@*" />
      <xsl:value-of select="."/>
    </a>
  </xsl:template>
  <xsl:template match="tt_bull">
    <span class="tt_bull">
      <xsl:value-of select="."/>
    </span>
  </xsl:template>
  <xsl:template match="tt_branch">
    <div class="tt_branch">
      <xsl:apply-templates select ="da"/>
      <xsl:apply-templates select ="tt_bull"/>
      <xsl:apply-templates select ="tt_row"/>
      <xsl:apply-templates select ="tt_branch"/>
    </div>
  </xsl:template>
  <xsl:template match="tt_tree">
    <div class="tt_tree">
      <xsl:apply-templates/>
    </div>
  </xsl:template>
  <!--   Table List -->
  <xsl:template match="tablelist">
    <div class="tablelist">
      <table class="tablelist" cellpadding="3" >
        <xsl:apply-templates />
      </table>
    </div>
  </xsl:template>
  <xsl:template match="tl_form">
    <form>
      <xsl:apply-templates select="@*" />
      <xsl:apply-templates select="tablelist" />
      <xsl:apply-templates select="inputh" />
    </form>
  </xsl:template>
  
    
    
  <xsl:template match="tl_table">

    <table>
      <tr>
        <td>
          <xsl:apply-templates select="tl_navbar"/>
        </td>
      </tr>
      <tr>
        <td width="100%">
          <xsl:apply-templates select="tl_form"/>
          <xsl:apply-templates select="tablelist"/>
          <xsl:apply-templates select="tt_tree"/>
        </td>
      </tr>
    </table>
  </xsl:template>
  <xsl:template match="ch_arwup">
    <div class="sp_char">&arw_up;</div>
  </xsl:template>
  <xsl:template match="ch_arwdown">
    <div class="sp_char">&arw_down;</div>
  </xsl:template>
  <xsl:template match="tl_sort">
    <th class="tl_sort" >
      <xsl:attribute name="title">
        Sort by:<xsl:value-of select="."/>
      </xsl:attribute>
      <a >
        <xsl:call-template name="link"/>
        <!-- <xsl:value-of select="."/>-->
        <xsl:apply-templates/>
      </a>
    </th>
  </xsl:template>
  <xsl:template match="tl_head">
    <th class="tl_sort" >
      <xsl:apply-templates/>

    </th>
  </xsl:template>
  <xsl:template match="tl_navbar">
    <table class="tl_navbar">
      <tr>
        <xsl:apply-templates/>
      </tr>
    </table>
  </xsl:template>
  <xsl:template match="tl_navbut">
    <td  class="tl_navbut" ONMOUSEOVER="this.className='tl_navbut_hi'"  ONMOUSEOUT="this.className='tl_navbut'">
      <xsl:attribute name="onclick">
        location.href='<xsl:call-template name="url"/>'
      </xsl:attribute><xsl:attribute name="title">
        Go to page#<xsl:value-of select="."/>
      </xsl:attribute>&bull;
    </td>
  </xsl:template>
  <xsl:template match="tl_navpage">
    <td  class="tl_navbut" ONMOUSEOVER="this.className='tl_navbut_hi'"  ONMOUSEOUT="this.className='tl_navbut'">
      <xsl:attribute name="onclick">
        location.href='<xsl:call-template name="url"/>'
      </xsl:attribute>
      <xsl:value-of select="."/>
    </td>
  </xsl:template>
  <xsl:template match="tl_navbut_here">
    <td  class="tl_navbut_here" >
      <xsl:attribute name="title">
        current page#<xsl:value-of select="."/>
      </xsl:attribute>&wdg_down;
    </td>
  </xsl:template>
  <xsl:template match="tl_rh">
    <tr class="rowclick" >
      <xsl:apply-templates/>
    </tr>
  </xsl:template>
  <xsl:template match="tl_row">
    <tr class="rowclick" ONMOUSEOVER="this.className='tl_row_hi'"
    ONMOUSEOUT="this.className='tl_row'">
      <xsl:attribute name="onclick">
        return go(this,event,' <xsl:value-of select="/page/@lnk"/><xsl:value-of select="/page/@cxt"/><xsl:value-of select="tl_key/@xi"/>')
      </xsl:attribute>
      <xsl:apply-templates/>
    </tr>
  </xsl:template>
  <xsl:template match="tl_c">
    <td class="tl_c">
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates/>
    </td>
  </xsl:template>
  <xsl:template match="tl_c_page">
    <td class="tl_c_page">
      <xsl:apply-templates/>
    </td>
  </xsl:template>
  <xsl:template match="tl_key">
    <td class="tl_key">
      <a class="tl_key" onclick="stopprop(event);">
        <xsl:attribute name="title">
          View Record#<xsl:value-of select="."/>
        </xsl:attribute>
        <xsl:call-template name="link"/>
        <xsl:value-of select="."/>
      </a>
    </td>
  </xsl:template>
  <xsl:template match="tl_edit_field">
    <td class="tl_key">
      <center>
        <a class="tl_key">
          <xsl:attribute name="title">
            Edit Field '<xsl:value-of select="."/>'
          </xsl:attribute><xsl:call-template name="link"/>(edit)
        </a>
      </center>
    </td>
  </xsl:template>
  <xsl:template match="tl_move">
    <td class="tl_key">
      <a class="tl_key" onclick="stopprop(event);">
        <xsl:call-template name="link"/>
        <xsl:apply-templates/>
      </a>
    </td>
  </xsl:template>
  <xsl:template match="tl_delkey">
    <td class="tl_key">
      <a>
        <xsl:attribute name="onclick">
          event.stopPropagation();
          window.event.cancelBubble = true;
        </xsl:attribute><xsl:attribute name="title">
          Delete Record#<xsl:value-of select="."/>
        </xsl:attribute>
        <xsl:attribute name="href">
          <xsl:call-template name="url"/>
        </xsl:attribute>DEL
      </a>
    </td>
  </xsl:template>
  <xsl:template match="tl_selkey2">
    <td class="tl_key" onclick="stopprop(event);" >
      <input type="checkbox" value="X"  >
        <xsl:attribute name="onclick">event.stopPropagation();window.event.cancelBubble = true;</xsl:attribute>
        <xsl:attribute name="name">
          <xsl:value-of select="@name"/>
        </xsl:attribute>
      </input>
    </td>
  </xsl:template>
  <xsl:template match="tl_selkey">
    <td class="tl_key" onclick="stopprop(event);">
      <xsl:apply-templates />
    </td>
  </xsl:template>
  <xsl:template match="dbg_node">
    <table class="dbg_node" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td class="dbg_node" colspan="2" >
          <xsl:value-of select="@name"/>
        </td>
      </tr>
      <tr>
        <td class="dbg_node">&space;</td>
        <td class="dbg_node">
          <table class="dbg_node" >
            <xsl:apply-templates select="dbg_var"/>
          </table>
        </td>
      </tr>
      <xsl:for-each select="dbg_node">
        <tr>
          <td class="dbg_node">&space;</td>
          <td class="dbg_subnode">
            <xsl:apply-templates select="self::dbg_node"/>
          </td>
        </tr>
      </xsl:for-each>
    </table>
  </xsl:template>

  <xsl:template match="dbg_var">
    <tr>
      <td class="dbg_var">
        <xsl:value-of select="@name"/>
      </td>
      <td class="dbg_var_val">
        <xsl:value-of select="@val"/>
      </td>
    </tr>
  </xsl:template>
  <xsl:template match="formsaveclose">
    <input type="submit" name="saveclose" class="form"  value="Save &amp; Close"/>
  </xsl:template>
  <xsl:template match="form">
    <form>
      <xsl:apply-templates select="@*" />
      <xsl:apply-templates select="tablelist" />
      <table    >
        <xsl:apply-templates select="@class" />
        <xsl:apply-templates select="form_input" />
        <xsl:apply-templates select="tr" />
        <xsl:apply-templates select="inputh" />
        <xsl:apply-templates select="rec_fld" />
        <tr>
          <td></td>
          <td>
            <input type="submit" name="saveview"  class="form">
              <xsl:attribute name="value">
                <xsl:value-of select="@submit_text"/>
              </xsl:attribute>
            </input>
            <xsl:apply-templates select="child::input" />
            <input type="reset" class="form" value="undo"/>
          </td>
        </tr>
      </table>
    </form>
  </xsl:template>
  <xsl:template match="form_input">
    <tr>
      <xsl:attribute name="title">
        <xsl:value-of select="@help"/>
      </xsl:attribute>
      <td >
        <xsl:value-of select="@label"/>
      </td>
      <td >
        <xsl:apply-templates />
      </td>
    </tr>

  </xsl:template>
  <xsl:template match="inputh">
    <input>
      <xsl:apply-templates select="@*" />
      <xsl:attribute name="value">
        <xsl:value-of select="."/>
      </xsl:attribute>
    </input>
  </xsl:template>

  <xsl:template match="recoutline">
    <div class="recoutline"  >
      <xsl:apply-templates />
    </div>
  </xsl:template>
  <xsl:template match="recvw[@tableid='7']">
    <table class="recvw"  >
      <xsl:apply-templates />
    </table>
  </xsl:template> 
  <xsl:template match="recvw">
    <table class="recvw"  >
      <xsl:apply-templates />
    </table>
  </xsl:template>

  <xsl:template match="rec_fld">
    <tr class="rec_fld"  >
      <xsl:if test="count(rec_label)=0">
        <td class="rec_label"  >
          <xsl:value-of select="@name"/>
        </td>
      </xsl:if>
      <xsl:apply-templates />
    </tr>
  </xsl:template>
  <xsl:template match="rec_label">
    <td class="rec_label"  >
      <xsl:apply-templates />
    </td>
  </xsl:template>
  <xsl:template match="rec_val">
    <td class="rec_val"  >
      <xsl:apply-templates />
    </td>
  </xsl:template>

  <xsl:template match="li">
    <li>
      <xsl:apply-templates select="@*" />
      <xsl:apply-templates />
    </li>
  </xsl:template>

  <xsl:template match="ord_item">
    <li class="order">
      <xsl:attribute name="id">o<xsl:value-of select="@ord"/></xsl:attribute>
      <xsl:apply-templates />
    </li>
  </xsl:template>
  <xsl:template match="ord_list">
    <ul class="boxy">
      <input type="hidden" name="order" value=""  />
      <xsl:apply-templates />
    </ul>
  </xsl:template> 
  <xsl:template match="ord_form">
    <form class="ord_form">
      <xsl:apply-templates select="@*" />
      <xsl:apply-templates />
      <input type="submit" name="save" value="save"  class="form"/>
    </form>
  </xsl:template>
  <xsl:template match="div">
    <div>
      <xsl:apply-templates select="@*" />      
      <xsl:apply-templates />
    </div>
  </xsl:template>
  <xsl:template match="ul">
    <ul>
      <xsl:apply-templates select="@*" />
      <xsl:apply-templates />
    </ul>
  </xsl:template>
  <xsl:template match="tablewide">
    <table width="100%" cellpadding="12" cellspacing="0"  class="tablewide" >
      <xsl:apply-templates select="@*" />
      <xsl:apply-templates />
    </table>
  </xsl:template>
  <xsl:template match="td">
    <td>
      <xsl:apply-templates select="@*" />
      <xsl:apply-templates />
    </td>
  </xsl:template>
  <xsl:template match="tr">
    <tr>
      <xsl:apply-templates/>
    </tr>
  </xsl:template>
  <xsl:template match="wrap|msgs">
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="dom_debug">
    <table onload="">
      <tr>
        <td class="tabbar" >
          <table class="tabbar">
            <tr>
              <xsl:apply-templates/>
            </tr>
          </table>
        </td>
      </tr>
    </table>
  </xsl:template>
  <xsl:template match="*">
    <xsl:copy-of select="."/>
  </xsl:template>


  <xsl:template match="@*">
    <xsl:copy-of select="."/>
  </xsl:template>

</xsl:stylesheet>
