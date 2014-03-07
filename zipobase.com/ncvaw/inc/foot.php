 </div> <!--main-->
</div><!--wrapper-->
 



</body>
<?php 
    foreach ( $zs_foot_jsfile as $item ) {
		echo("<script type='text/javascript' src='$item'></script>");	}
		
	foreach ( $zs_foot_script as $item ) {
		echo("<script type='text/javascript'>$item</script>");	}		
?>

<script>
jQuery(document).ready(function () 
		{
		<?php 
		foreach ( $funcs_init as $item ) {echo("$item;");	}	
		?>
		});
call_zipo();		
</script>
</html>
