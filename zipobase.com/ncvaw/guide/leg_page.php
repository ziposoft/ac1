<?php
include $header;

include $root.'/inc/ncleg.php';
$legid=0;
if (array_key_exists ( "id", $_GET )) 
	$legid= $_GET ["id"];
else
	echo '<h2>No id selected</h2>';


$g_leg_list=new leg_list();
$g_bill_list=new bill_list();
$g_vote_data=new vote_data();


$leg=$g_leg_list->get_leg_by_id($legid);






?>


<div class="text_wrap">
<table>

<?php 
$leg->print_list_row();
?>
</table>

<H2>Voting Record</H2>

<?php 

$g_vote_data->print_list_votes($leg->id);

?>


	 
</div>


<?php include $footer; ?>