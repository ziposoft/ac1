<?php

include $root.'/inc/db.php';
$legid=getParam( "id");

if (!$legid)
	echo '<h2>No id selected</h2>';
$leg=getobj("leg_list")->get_leg_by_key($legid);

if($leg)
	$page_title=$leg->name;

include $header;


?>


<div class="text_wrap">
<?php 
$leg->print_list_row();?>

<?php 

$leg->print_survey();?>




<H3>Bills Sponsored</H3>
<table  class='votes'>
  <thead>
    <tr>
<th>Vote</th>
<th>Bill</th>

</tr> </thead>
<?php $leg->print_list_sponsorship(); ?>
</table>

<H3>Voting Record</H3>
<table  class='votes'>
  <thead>
    <tr>
<th>Vote</th>
<th>Bill</th>

</tr> </thead>
<?php $leg->print_list_votes(); ?>
</table>

	 
</div>


<?php include $footer; ?>