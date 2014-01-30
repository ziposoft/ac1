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
<table  >

<?php 
$leg->print_list_row();
?>
</table>
<H3>Bills Sponsored</H3>

<?php $leg->print_list_sponsorship(); ?>


<H3>Voting Record</H3>
<table  class='votes'>
  <thead>
    <tr>
<th>Vote Grade</th>
<th>Bill</th>
<th>Info</th>
<th>How Voted</th>
<th>Good for Animals</th>
</tr> </thead>
<?php $leg->print_list_votes(); ?>
</table>

	 
</div>


<?php include $footer; ?>