<?php
$dog_folder = $root . "/dogs/";
$dog_data_file = $dog_folder . "dogs.data";

$dog_page = 0;
$dog_list = 0;
include $root . "/inc/photo.php";
if (array_key_exists ( "name", $_GET )) {
	$list = new doglist ();
	$dog_page = $list->get_dog ( $_GET ["name"] );
	
	$dog_page->page_init ();
} else {
	
	$dog_list = new doglist ();
	$dog_list->get_list ( $dog_filter );
}
function dog_print_page_or_list() {
	global $dog_page;
	global $dog_list;
	if ($dog_page)
		$dog_page->show_page ();
	else if ($dog_list)
		$dog_list->print_list ();
}
function dog_get_directory($name) {
	return preg_replace ( "/[^A-Za-z0-9]/", "_", $name );
	// return preg_replace("[^A-Za-z0-9]","_",$name);
}
function dog_get_data_file() {
	global $dog_data_file;
	// Parsing this spreadsheet: https://spreadsheets.google.com/pub?key=0Ah0xU81penP1dFNLWk5YMW41dkcwa1JNQXk3YUJoOXc&hl=en&output=html
	$SP_KEY = "0Am_PTj36WN20dHFoeEF5aHBfdEdGOTZlcVZXbmh3Smc";
	// https://docs.google.com/spreadsheet/ccc?key=0Am_PTj36WN20dHFoeEF5aHBfdEdGOTZlcVZXbmh3Smc&hl=en&output=html
	$url = 'http://spreadsheets.google.com/feeds/list/0Am_PTj36WN20dHFoeEF5aHBfdEdGOTZlcVZXbmh3Smc/od6/public/values?alt=json';
	$file = file_get_contents ( $url );
	$fp = fopen ( $dog_data_file, 'w' );
	fwrite ( $fp, $file );
	fclose ( $fp );
}
class dog {
	public $data;
	public $name;
	public $displayname;
	public $id;
	public $url_dog_page;
	public $url_cover_jpg;	
	public $url_home_jpg;	
	public $jpg_path;
	public $photo_set;
	public $photo_shelter;
	public $status_code; // A, W, H, P
	public $status_text; // adopted, waiting, home
	public function __construct($data_in) {
		$this->data = $data_in;
	}
	public function get_days_in_shelter() {
		$datetime1 = date_create ( $this->data->{ 'gsx$dateaq' }->{'$t' } );
		
		$datetime2 = date_create ( 'now' );
		if ($this->status_code != 'W') {
			$date_left = $this->data->{ 'gsx$dateleft' }->{'$t' };
			$datetime2 = date_create ( $date_left );
		}
		
		$interval = date_diff ( $datetime1, $datetime2 );
		return $interval->format ( '%a days' );
	}
	public function page_init() {
		global $fb_description;
		global $fb_title;
		$this->init ();
		$fb_description = htmlentities ( $this->get_bio () );
		$fb_title = $this->name;
		$this->photo_set = photo_set_get ( $this->jpg_path . '800/' );
		$photo_shelter_set = photo_set_get ( $this->jpg_path . '/shelter800/' );
		if (count ( $photo_shelter_set ))
			$this->photo_shelter = $photo_shelter_set [0];
		photo_set_meta_fb ( $this->photo_set );
	}
	public function init() {
		global $root;
		$name = $this->data->{	'gsx$name' }->{'$t' };
		$this->displayname=$name;
		$newname= $this->data->{	'gsx$newname' }->{'$t' };
		if($newname)
		{
			$this->displayname.=' (now '.$newname .')';
		
		}
		$this->id = dog_get_directory ( $name );
		$this->name = $name;
		$this->url_dog_page = $_SERVER ['PHP_SELF'] . "?name=" . $this->id;
		$this->jpg_path = '/dogs/' . $this->id . '/';
		$this->get_status ();
		$this->url_cover_jpg=photo_get_pattern( $this->jpg_path . '800/',"home_*.jpg");		
		if(!$this->url_cover_jpg)
			$this->url_cover_jpg=$this->data->{	'gsx$thumb' }->{'$t' };
			
				
		return $name;
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
	public function get_bio() {
		$text = $this->data->{	'gsx$bio' }->{'$t' };
	
		return $text;
	}
	public function get_display_name() {
		$text = $this->data->{	'gsx$bio' }->{'$t' };
		
		return $text;
	}
	public function get_status() {
		$status = $this->data->{	'gsx$status' }->{'$t' };
		$this->status_text = 'Adopted';
		$this->status_code = 'A';
		if ($status == 'Waiting') {
			$this->status_code = 'W';
			$this->status_text = 'Waiting for a home';
		}
		if ($status == 'PTS') {
			$this->status_code = 'P';
			$this->status_text = 'PTS';
		}
		if ($status == 'Rescue') {
			$this->status_code = 'A';
			$this->status_text = 'Went to rescue';
		}
		
		if ($status == 'Home') {
			$this->status_text = 'Adopted';
			$this->status_code = 'H';
		}
		$date_left = $this->data->{ 'gsx$dateleft' }->{'$t' };
		if ($date_left)
			$this->status_text .= ' ' . $date_left;
	}
	public function print_bio() {
		$text = $this->data->{	'gsx$bio' }->{'$t' };
		$fb_description = $text;
		echo "<tr ><td colspan='2' class='dogbio'>$text</td></tr>";
	}

	public function print_list_row_phone() {
		global $dog_folder;
		


		$has_dir = file_exists ( $dog_folder . $this->id );
		
		echo "<tr class='doglist'><td class='dogthumb' >";
		if ($has_dir)
			echo "<a href='$this->url_dog_page'>";
		echo "<img  src='$this->url_cover_jpg'/>";

		if ($has_dir)
			echo "</a>";
		echo "</td><td class='td_doglist_info' ><h2 >$this->displayname</h2></td></tr><tr><td colspan='2'><table class='doginfo'>";
		
		$this->print_table_row ( 'Status', $this->status_text );
		$this->print_table_val ( 'Impoundment', 'id' );
		$this->print_table_val ( 'Breed', 'breed' );
		$this->print_table_row ( 'Days in shelter', $this->get_days_in_shelter () );
		$this->print_table_val ( 'Age', 'age' );
		
		echo '</table>';
		if ($has_dir) {
			echo ("<a style='padding-bottom:20px'  href='$this->url_dog_page'>$this->name's Page</a>");
		}
		
		echo "<div style='padding:20px'></div></td></tr>";
	}
	public function print_list_row() {
		global $dog_folder;
		global $isPhone;
		
		$this->init ();
		if($isPhone)
			return $this->print_list_row_phone();
		
	
		$has_dir = file_exists ( $dog_folder . $this->id );
		
		echo "<tr class='doglist'><td class='dogthumb' >";
		if ($has_dir)
			echo "<a href='$this->url_dog_page'>";
		echo "<img class='dog_cover' src='$this->url_cover_jpg'/>";

		if ($has_dir)
			echo "</a>";
		echo "</td><td class='td_doglist_info' ><h2 >$this->displayname</h2><table class='doginfo'>";
		
		$this->print_table_row ( 'Status', $this->status_text );
		$this->print_table_val ( 'Adopted By:', 'adopter' );		
		$this->print_table_val ( 'Impoundment', 'id' );
		$this->print_table_val ( 'Breed', 'breed' );
		$this->print_table_row ( 'Days in shelter', $this->get_days_in_shelter () );
		$this->print_table_val ( 'Age', 'age' );
		
		echo '</table>';
		if ($has_dir) {
			echo ("<a href='$this->url_dog_page'>$this->name's Page</a>");
		}
		echo '</td></tr>';
	}
	public function show_page() {
		global $root;
		global $isPhone;
		$dogdir = $this->id;
		$dog_row = $this->data;
		if($isPhone)
			include $root . '/inc/mdogpage.p';
		else
			include $root . '/inc/dogpage.p';
				
	}
}
class doglist {
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

