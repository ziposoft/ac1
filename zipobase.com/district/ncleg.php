<?php

$leg_data_file="";
		
		

function ncleg_get_data_file() {
	global $leg_data_file;
	// Parsing this spreadsheet: https://spreadsheets.google.com/pub?key=0Ah0xU81penP1dFNLWk5YMW41dkcwa1JNQXk3YUJoOXc&hl=en&output=html
	$SP_KEY = "0Am_PTj36WN20dHFoeEF5aHBfdEdGOTZlcVZXbmh3Smc";
	// https://docs.google.com/spreadsheet/ccc?key=0Am_PTj36WN20dHFoeEF5aHBfdEdGOTZlcVZXbmh3Smc&hl=en&output=html
	$url = 'http://spreadsheets.google.com/feeds/list/0AonA9tFgf4zjdEJNRC1UM3k4U01kQlZDaTZ5QTBpcVE/od6/public/values?alt=json';
	$file = file_get_contents ( $url );
	$fp = fopen ( $leg_data_file, 'w' );
	fwrite ( $fp, $file );
	fclose ( $fp );
}



class legislator {
	public $data;
	public $name;
	public $displayname;
	public $id;
	public $url_cover_jpg;

	public $jpg_path;
	
	public function __construct($data_in) {
		$this->data = $data_in;
		$this->name = $this->data->{	'gsx$name' }->{'$t' };
		
	}	
	
}






class leg_list {
	public $rows;
	public $list;
	public function print_list() {
		global $dog_folder;
		echo "<table class='tbl_doglist' border='1'>";
		foreach ( $this->list as $d ) {
			$d->print_list_row ();
		}
		echo '</table>';
	}
	public function __construct() {
		global $dog_data_file;
		if (array_key_exists ( "refresh", $_GET )) {
			dog_get_data_file ();
		}
		if (! file_exists ( $dog_data_file )) {
			dog_get_data_file ();
		}
		$file = file_get_contents ( $dog_data_file );
		$json = json_decode ( $file );
		$this->rows = $json->{'feed' }->{'entry' };
	}
	public function get_dog($dog_id) {
		foreach ( $this->rows as $row ) {
			$name = $row->{	'gsx$name' }->{'$t' };
			$dogdir = dog_get_directory ( $name );
			
			if ($dog_id == $dogdir) {
				return new dog ( $row );
			}
		}
		return 0;
		echo ("Not Found:" . $dog_id);
	}
	public function get_list($status_filter) {
		$this->list = array ();
		
		foreach ( $this->rows as $row ) {
			$name = $row->{	'gsx$name' }->{'$t' };
			if (($name == 'name') || ($name == ''))
				continue;
			$status = $row->{'gsx$status' }->{	'$t' };
			if ($status_filter)
				if (! in_array ( $status, $status_filter ))
					continue;
			$this->list [$name] = new dog ( $row );
		}
		ksort ( $this->list );
		return $this->list;
	}
}

?>

