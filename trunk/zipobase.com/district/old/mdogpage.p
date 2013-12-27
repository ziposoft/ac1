

<h2><?php echo($this->name) ?></h2>
<table class='tbl_dogcard' style='text-align: top; width: 100%'>
	<?php
	$this->print_table_row ( 'Status', $this->status_text );
	$this->print_table_val ( 'Impoundment#', 'id' );
	$this->print_table_val ( 'Breed', 'breed' );
	$this->print_table_val ( 'Gender', 'gender' );
	$this->print_table_val ( 'Age', 'age' );
	$this->print_table_row ( 'Days in shelter', $this->get_days_in_shelter () );
	
	if ($this->photo_shelter) {
		echo "<tr><td class='dog_label'>Shelter Photo: </td><td class='dog_val'>";
		$this->photo_shelter->print_thumb ();
		echo "</td></tr>";
	}
	$this->print_bio ( 'Bio', 'bio' );
	?>
	</table>


<?php

photo_set_print_imgs_m ( $this->photo_set, 0 );


include $root . '/inc/fblike.p';
?>
