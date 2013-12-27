<!DOCTYPE html>
<html>
	<head>
		<?php include $root.'/inc/headscripts.p' ?>
		<meta name="viewport" content="width=device-width initial-scale=1.0 maximum-scale=1.0 user-scalable=yes" />
		<link type="text/css" rel="stylesheet" href="/inc/mlfd.css" />
		<link type="text/css" rel="stylesheet" href="/script/mmenu/jquery.mmenu.all.css" />
		<!-- for the fixed header -->
		<style type="text/css">
			#header,
			#footer
			{
				position: fixed;
				width: 100%;

				-webkit-box-sizing: border-box;
				-moz-box-sizing: border-box;
				-ms-box-sizing: border-box;
				-o-box-sizing: border-box;
				box-sizing: border-box;
			}
		</style>
		<script type="text/javascript" src="/script/mm/jquery.mmenu.js"></script>
        
		<script type="text/javascript">
			$(function() {
				$('nav#menu').mmenu({
					slidingSubmenus: false
				});
			});
		</script>   
	</head>
	<body>
    
 <?php include $root.'/inc/fb_script.p';?>

   
		<div id="page">
			<div id="header" class="mm-fixed-top">
				<a href="#menu">Contents</a>
				Landfill Dogs
			</div>
			<div id="content"><!-- content -->