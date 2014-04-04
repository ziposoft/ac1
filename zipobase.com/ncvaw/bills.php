<?php
$page_title='List Of Bills';
include $header;

include $root.'/inc/ncleg.php';

$bill_list=new bill_list();

$bill_list->print_bills();





?>




<?php include $footer; ?>