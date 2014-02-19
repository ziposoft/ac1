<?php
include $header;






include $root.'/inc/ncleg.php';
 
$list=getobj("bill_list");

$doc=0;

if (array_key_exists ( "doc", $_GET )) 
	$doc= $_GET ["doc"];


$bill=$list->get_bill ($doc  );
$bill->print_page ();
$votes=getobj("vote_data");
$votes->print_bill_votes("Primary Sponsors", $bill->doc,'psp',0); 
$votes->print_bill_votes( "Sponsors",$bill->doc,'sp',0);
if($bill->svid)
{
	$votes->print_bill_votes("Senate Votes For", $bill->doc,'Aye',$bill->svid);
	$votes->print_bill_votes("Senate Votes Against", $bill->doc,'No',$bill->svid);
		
}
if($bill->hvid)
{
	$votes->print_bill_votes("House Votes For", $bill->doc,'Aye',$bill->hvid);
	$votes->print_bill_votes("House Votes Against", $bill->doc,'No',$bill->hvid);

}
?>



<?php include $footer; ?>