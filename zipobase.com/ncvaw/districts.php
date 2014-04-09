<?php

$chamber=getParam("ch");




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
<?php 
 
$leglist=getobj("leg_list");

$leglist->get_list ($chamber,$district  );
$leglist->print_list ();

getobj('canidates')->printlist($chamber,$district);

?>	
	
	</div>


<?php include $footer; ?>
				
				