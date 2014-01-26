<?php
include $header;






include $root.'/inc/ncleg.php';
 
$list=get_bill_list();

$doc=0;

if (array_key_exists ( "doc", $_GET )) 
	$doc= $_GET ["doc"];


$bill=$list->get_bill ($doc  );
$bill->print_page ();

?>
<h3>Primary Sponsors</h3>
<?php get_vote_data()->print_bill_votes( $bill->doc,'psp',0); ?>
<h3>Sponsors</h3>
<?php get_vote_data()->print_bill_votes( $bill->doc,'sp',0);
if($bill->svid)
{
	echo("<h3>Senate Votes For</h3>");
	get_vote_data()->print_bill_votes( $bill->doc,'Aye',$bill->svid);
	echo("<h3>Senate Votes Against</h3>");
	get_vote_data()->print_bill_votes( $bill->doc,'No',$bill->svid);
		
}
if($bill->hvid)
{
	echo("<h3>House Votes For</h3>");
	get_vote_data()->print_bill_votes( $bill->doc,'Aye',$bill->hvid);
	echo("<h3>House Votes Against</h3>");
	get_vote_data()->print_bill_votes( $bill->doc,'No',$bill->hvid);

}
?>



<?php include $footer; ?>