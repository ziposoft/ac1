<?php

$chamber=getParam("ch");
$district=getParam("dist");

array_push($zs_foot_jsfile,
	"http://www.google.com/jsapi",
	"http://maps.google.com/maps/api/js?sensor=false",

	"/inc/maps_lib2.js");

array_push($zs_foot_script,
	"jQuery(document).ready(function () {map_init();map_show_district('$chamber','$district'	); } );");



include $header;
include $root.'/inc/ncleg.php';
?>


	<div id="page" style="clear: both; margin: auto; padding: 10px 20px 0 20px; height: 90%;">
	<div id="map_canvas" style="display:inline;float:right;height: 300px; width:500px;"  ></div>
	<h1>Who represents me?</h1>
	
	
	</div>


<?php include $footer; ?>
				
				