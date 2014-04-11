<?php
// called via AJAX!! Not standalone
include $root.'/inc/db.php';
 
$leglist=getobj("leg_list");

$chamber=0;
$district=0;
if (array_key_exists ( "dist", $_GET )) 
	$district= $_GET ["dist"];
if (array_key_exists ( "ch", $_GET )) 
	$chamber= $_GET ["ch"];

	

$leg=$leglist->get_leg_by_district ($chamber,$district  );
$leg->print_list_row ();




?>



