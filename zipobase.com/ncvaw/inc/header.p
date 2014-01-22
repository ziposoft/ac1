<!DOCTYPE html>
<html>
<head>
    
    <?php 
    include $root.'/inc/headscripts.p';
    include $root.'/inc/zs_menu.php';
    

    ?>


<link href='/inc/ncvaw.css' rel='stylesheet' type='text/css'>



</head>

<body>

 <?php
	include $root . '/inc/fb_script.p';
	?>
  
<div id="wrapper">
<a href="http://www.ncvaw.org/"><img style="float: left" src="/NCVAW_paw.png" alt="North Carolina Voters for Animal Welfare"></a>
 
<a href="http://www.ncvaw.org/"><img style="clear:right;margin-top:10px" src="/NCVAW_txt.png" alt="North Carolina Voters for Animal Welfare"></a>
  	
<?php

make_menu([
	z_menu("About Us",[
		z_mi("Home","/home.html","HOME"),
		z_mi("map","/map.html","MAP")]),
	z_menu("News",[
		z_mi("Home2","/home.html","HOME"),
		z_mi("map2","/map.html","MAP")]),
	z_menu("Donate",[
		z_mi("Home2","/home.html","HOME"),
		z_mi("map2","/map.html","MAP")]),
	z_menu("Get Involved",[
		z_mi("Home2","/home.html","HOME"),
		z_mi("map2","/map.html","MAP")]),		
	z_menu("Voters Guide",[
		z_mi("LEGISLATIVE REPORT CARD","/home.html","HOME"),
		z_mi("map2","/map.html","MAP")])
		
		
]);

         
         
         
?>
		<div id="main">