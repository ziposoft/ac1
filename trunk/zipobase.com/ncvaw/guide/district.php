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

$distobj=getobj('districts')->get($chamber,$district);
$counties=$distobj->get('counties');
?>


	<div id="page" 
	draggable="true" ondragstart="dragstart(event)" ondragend="dragend(event)"
	style="clear: both; margin: auto; padding: 10px 20px 0 20px; height: 90%;">
	<div id="map_canvas" style="display:inline;float:right;height: 300px; width:500px;"  ></div>
	<h1><?php 
	echo(($chamber=='H' ? 'House' : 'Senate') . ' District #' . $district);
	

		?>
	</h1>
	<p>
	<?php echo($counties);?>
	</p>
<?php 
 
$leglist=new leg_list();

$leglist->get_list ($chamber,$district  );
$leglist->print_list ();
?>	
	
	</div>


<?php include $footer; ?>
				
				