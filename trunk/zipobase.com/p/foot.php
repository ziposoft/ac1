</body>

<script>
jQuery(document).ready(function () 
		{
		<?php 
		foreach ( $funcs_init as $item ) {echo("$item;");	}	
		?>
		});

<?php echo("call_zipo('$current_url');"); ?>
	
</script>
</html>