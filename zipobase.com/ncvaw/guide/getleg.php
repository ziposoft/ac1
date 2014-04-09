<?php







include $root.'/inc/ncleg.php';
 
$leglist=getobj("leg_list");

$chamber=0;
$district=0;
if (array_key_exists ( "dist", $_GET )) 
	$district= $_GET ["dist"];
if (array_key_exists ( "ch", $_GET )) 
	$chamber= $_GET ["ch"];

	

$leglist->get_list ($chamber,$district  );
$leglist->print_list ();




?>



