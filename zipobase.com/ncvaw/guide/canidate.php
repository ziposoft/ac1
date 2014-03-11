<?php
include $header;

include $root.'/inc/ncleg.php';
$key=getParam( "key");

if (!$legid)
	echo '<h2>No id selected</h2>';



$leg=getobj("canidates")->get_candiate($key);

?>


<div class="text_wrap">
<?php 
$leg->print_list_row();

$leg->print_survey();



?>



<?php include $footer; ?>