<?php
include $header;

include $root.'/inc/ncleg.php';
$legid=0;
if (array_key_exists ( "id", $_GET )) 
	$legid= $_GET ["id"];
else
	echo '<h2>No id selected</h2>';


$g_leg_list=new leg_list();



$leg=$g_leg_list->get_leg_by_id($legid);






?>


<div class="text_wrap">
<table>

<?php 
$leg->print_list_row();
?>
</table>
<H2>Bills Sponsored</H2>

<?php $leg->print_list_sponsorship(); ?>


<H2>Voting Record</H2>

<?php $leg->print_list_votes(); ?>


	 
</div>


<?php include $footer; ?>