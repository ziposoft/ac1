<?php
$page_title="Canidate";


include $root.'/inc/db.php';
$key=getParam( "key");
$canidate=null;
if ($key)
{
	$canidate=getobj("canidates")->get_candiate($key);
    $page_title=$canidate->displayname;
}


include $header;
echo("<div class='text_wrap'>");


if($canidate)
{
	$canidate->print_list_row();
	//$canidate->print_survey();	
	
}
else
	echo ("<h2>Canidate $key not found</h2>");
echo("</div>");

 include $footer; ?>