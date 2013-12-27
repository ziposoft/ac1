<?php

$leg_data_file=$root."/data/ncleg.json";
		
		

function ncleg_refresh_data_file() {
	global $leg_data_file;
	// Parsing this spreadsheet: https://spreadsheets.google.com/pub?key=0Ah0xU81penP1dFNLWk5YMW41dkcwa1JNQXk3YUJoOXc&hl=en&output=html
	$SP_KEY = "0Am_PTj36WN20dHFoeEF5aHBfdEdGOTZlcVZXbmh3Smc";
	// https://docs.google.com/spreadsheet/ccc?key=0Am_PTj36WN20dHFoeEF5aHBfdEdGOTZlcVZXbmh3Smc&hl=en&output=html
	$url = 'https://spreadsheets.google.com/feeds/list/0AonA9tFgf4zjdEJNRC1UM3k4U01kQlZDaTZ5QTBpcVE/1/public/values?alt=json';
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
	public $chamberId;
	public $chamber;
	public $url_cover_jpg;

	public $jpg_path;
	public $url;
	
	public function __construct($data_in) {
		$this->data = $data_in;
		$this->name = $this->data->{	'gsx$name' }->{'$t' };
		$this->url = $this->data->{	'gsx$url' }->{'$t' };
		$this->id = $this->data->{	'gsx$uid' }->{'$t' };
		$this->chamberId = $this->data->{	'gsx$chamber' }->{'$t' };
		if($this->chamberId=='H')
			$this->chamber='House';
			else
			$this->chamber='Senate';
			
		
		
	}	
	
	public function print_table_val($label, $field) {
		$val = $this->data->{	'gsx$' . $field }->{'$t' };
		if(!$val)
			return;
		$this->print_table_row ( $label, $val );
	}
	public function print_table_row($label, $val) {
		echo "<tr><td class='dog_label'>$label: </td><td class='dog_val'>$val</td></tr>";
	}	
	public function print_list_row() {
		global $isPhone;
		
		//$this->init ();
		if($isPhone)
			return $this->print_list_row_phone();
		
		
		echo "<tr class='doglist'><td class='dogthumb' >";
		echo "<img class='dog_cover' src='http://www.ncleg.net/$this->chamber/pictures/$this->id.jpg'/>";

		echo "</td><td class='td_doglist_info' ><h2 >$this->name</h2><table class='doginfo'>";
		
		$this->print_table_val ( 'district:', 'district' );		
		$this->print_table_val ( 'county', 'county' );		
		$this->print_table_val ( 'email', 'email' );		
		$this->print_table_val ( 'phone', 'phone' );		
				

		
		echo '</table>';
		echo ("<a target='_blank' href='$this->url'>$this->name's Page</a>");
		echo '</td></tr>';
	}	
}






class leg_list {
	public $rows;
	public $list;
	public function print_list() {
		echo "<table class='tbl_doglist' border='1'>";
		foreach ( $this->list as $d ) {
			$d->print_list_row ();
		}
		echo '</table>';
	}
	public function __construct() {
		global $leg_data_file;
		if (array_key_exists ( "refresh", $_GET )) {
			ncleg_refresh_data_file ();
		}
		if (! file_exists ( $leg_data_file )) {
			ncleg_refresh_data_file ();
		}
		$file = file_get_contents ( $leg_data_file );
		$json = json_decode ( $file );
		$this->rows = $json->{'feed' }->{'entry' };
	}
	public function get_leg($leg_id) {
		foreach ( $this->rows as $row ) {
			$id = $row->{	'gsx$id' }->{'$t' };
			
			if ($leg_id == $id) {
				return new legislator ( $row );
			}
		}
		return 0;
		echo ("Not Found:" . $leg_id);
	}
	public function get_list($status_filter) {
		$this->list = array ();
		
		foreach ( $this->rows as $row ) {
			$name = $row->{	'gsx$name' }->{'$t' };
			if (($name == 'name') || ($name == ''))
				continue;
				/*
			$status = $row->{'gsx$status' }->{	'$t' };
			if ($status_filter)
				if (! in_array ( $status, $status_filter ))
					continue;
				*/
			$this->list [$name] = new legislator ( $row );
		}
		ksort ( $this->list );
		return $this->list;
	}
}

?>

