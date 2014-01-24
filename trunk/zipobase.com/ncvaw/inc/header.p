<!DOCTYPE html>
<html>
<head>
    <link href='/inc/ncvaw.css' rel='stylesheet' type='text/css'>
    <?php 
    include $root.'/inc/headscripts.p';
    include $root.'/inc/zs_menu.php';
    

    ?>






</head>

<body>

 <?php
	include $root . '/inc/fb_script.p';
	?>
  
<div id="wrapper">
<a href="/"><img style="float: left" src="/img/NCVAW_paw.png" alt="North Carolina Voters for Animal Welfare"></a>
 
<a href="/"><img style="clear:right;margin-top:10px" src="/img/NCVAW_txt.png" alt="North Carolina Voters for Animal Welfare"></a>
  	
<?php

make_menu([
	z_menu("About Us",[
		z_mi("About Us","/aboutus.html","HOME"),
		z_mi("map","/map.html","MAP")]),
	z_menu("News",[
		z_mi("Home2","/home.html","HOME"),
		z_mi("map2","/map.html","MAP")]),
	z_mbi("Donate","/donate.html","Support Animal Welfare in NC"),
	

	z_menu("Get Involved",[
		z_mi("Home2","/home.html","HOME"),
		z_mi("map2","/map.html","MAP")]),		
	z_menu("Voters Guide",[
		z_mi("LEGISLATIVE REPORT CARD","/guide/leglist.html","HOME"),
		z_mi("Find your district","/guide/find.html",""),
		z_menu_sub("Search Legislature",[
			z_mi("LEGISLATIVE REPORT CARD","/guide/leglist.html","HOME"),
			z_mi("Find your district","/guide/find.html",""),	
			]),	
		]),
	
		
]);

         
         
         
?>
		<div id="main">