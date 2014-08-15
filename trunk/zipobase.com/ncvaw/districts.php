<?php

$chamber=getParam("ch");




include $header;
include $root.'/inc/db.php';

$distobj=getobj('districts')->print_list();


?>	
	
	


<?php include $footer; ?>
				
				