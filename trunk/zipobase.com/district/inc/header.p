<!doctype html>
<html >
<head>
    
    <?php include $root.'/inc/headscripts.p' ?>
    
<!--

    <meta property="og:url"  content="http://www.landfilldogs.info/"/>
    <meta property="fb:admins" content="1458587041" />-->
    
   <style type="text/css">
    @import url(/inc/lfd.css);
</style>     

</head>

<body>



 <?php
include $root.'/inc/fb_script.p';
function tab($id, $title, $href, $help)
{
	global $tab_select;
	$cl= "tab";
	if($tab_select == $id)
	{
		$cl= "tab_select";
	}
	echo "<a id='$id' class='$cl' title='$help' href='$href'>$title</a>\n";
}
function subtab($id, $title)
{
	global $subtab_select;
	$cl= "tab";
	if($subtab_select == $id)
	{
		$cl= "subtab_select";
	}
	echo "<a id='$id' class='$cl' title='$title' href='$id.html'>$title</a>\n";
}
?>
   
<div id="wrapper">

    <div class="titlebar" >
        <a class="large" title="Landfill Dogs" href="/">Landfill Dogs
        </a>    	
         <?php

tab('portfolio', 'Portfolio', "/portfolio/statement.html", "Landfill Dog photo portfolio and artist statement");
tab('adopt', 'The Dogs', "/adopt/in_need.html", "Landfill Dogs looking for homes");
//tab('conservation', 'Conservation', '/conservation/conservation.html', 'The consequences of animal overpopulation and prospects of change');
tab('about', 'About', '/about/about.html', 'General information about the Landfill Dog project');
tab('donate', 'Donate', '/adopt/donations.html', 'How you can help');

?>
	 
    </div>
     <div class="titlebar_sub">
       <?php

if($tab_select != "")
{
	include $root.'/'.$tab_select.'/header.p';
}
?>
    </div>
    <div id="main">