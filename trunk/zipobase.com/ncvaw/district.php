<?php

$chamber=getParam("ch");
$district=getParam("dist");

array_push($zs_foot_jsfile,
	"http://www.google.com/jsapi",
	"http://maps.google.com/maps/api/js?sensor=false",

	"/inc/maps_lib2.js");

array_push($funcs_init,
	"map_init();map_show_district('$chamber','$district'	);");



include $header;
include $root.'/inc/ncleg.php';

$distobj=getobj('districts')->get($chamber,$district);
$counties=$distobj->get('counties');

$nextdist=intval ($district)+1;
$next="/district.php?ch=$chamber&dist=$nextdist";
?>


	<div id="page" 
	
	style="clear: both; margin: auto; padding: 10px 20px 0 20px; height: 90%;">
	<a href='<?php print $next; ?>'>Next District</a>
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

getobj('canidates')->printlist($chamber,$district);

?>	
	
	</div>


<?php include $footer; ?>
				
				