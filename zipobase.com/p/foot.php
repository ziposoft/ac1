
<?php 
    foreach ( $zs_foot_jsfile as $item ) {
		echo("<script type='text/javascript' src='$item'></script>");	}
		
	foreach ( $zs_foot_script as $item ) {
		echo("<script type='text/javascript'>$item</script>");	}		
?>
<script>


<?php echo("call_zipo('$current_url');"); ?>
	
</script>
</body>

</html>