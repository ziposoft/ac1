<?php
include $header;

include $root.'/inc/ncleg.php';

$bill_list=new bill_list();







?>


<div class="text_wrap">
<table>

<?php 
$leg->print_list_row();
?>
</table>

<H2>Voting Record</H2>

<?php 

$leg->print_list_votes();

?>


	 
</div>


<?php include $footer; ?>