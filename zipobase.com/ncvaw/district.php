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
$next="";
$counties="";
if($distobj)
{
	$counties=$distobj->get('counties');
	
	$nextdist=intval ($district)+1;
	$next="/district.php?ch=$chamber&dist=$nextdist";
}
?>


	<div id="page" >
	<a href='<?php print $next; ?>'>Next District</a>
	<div id="map_canvas"   ></div>
	<h1><?php 
	echo(($chamber=='H' ? 'House' : 'Senate') . ' District #' . $district);
	

		?>
	</h1>
	<p>
	<?php echo($counties);?>
	</p>
	
	<h2>Primary Election 5/6/2014</h2>
<?php 
getobj('canidates')->printlist($chamber,$district,"primary");
?>		
<h2>General Election 11/4/2014</h2>
<?php	
	getobj('canidates')->printlist($chamber,$district,"gen");
	?>
	</div>


<?php include $footer; ?>
				
				