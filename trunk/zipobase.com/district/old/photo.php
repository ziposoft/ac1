<?php
class photo {
	public $local_path;
	public $path;
	public $id;
	public $thumbid;
	public $title;
	public function __construct($local, $path) {
		$this->local_path = $local;
		$path_parts = pathinfo ( $local );
		$this->id = $path_parts ['filename'];
		$this->path = $path . $path_parts ['basename'];
		$this->thumbid = $this->id . '_th';
	}
	public function print_thumb() {
		echo "<span class='thumb' id='$this->thumbid'>\n";
		echo "<img style='width:60px'
  	      src=\"$this->path\" onclick=\"thumb_clicked('$this->path','$this->id','')\" 
          onmouseover=\"mouse_over_img('$this->path','$this->id')\" /></span>\n";
	}
	public function print_img_m($titles) {
		global $root;
		echo "<div style='text-align:center;display:block;margin:10px 5px 20px 5px;'>
		<img class='mgallery' style='margin: 0px;padding: 0px;width:100%' src='$this->path' /> ";
		if ($titles) 
			if(array_key_exists($this->id,$titles))
			{
				$title = $titles [$this->id];
				
				echo "<h5 style='margin: 0px;padding: 0px;'>$title </h5> ";
			}
		echo "</div>\n";
	}
	public function print_img($titles) {
		global $root;
		$fullsize = 0;
		echo "<div class='photo_hide' id='$this->id'>\n";
		if ($fullsize)
			echo "<a href='$fullsize'>";
		echo "<img   class='gallery' src='$this->path'	onclick=\"img_clicked('$this->id')\" onload=\"photo_load('$this->path','$this->id')\"/>";
		if ($fullsize)
			echo "</a>";
		if ($titles) {
			if(array_key_exists($this->id,$titles))
			{
				$title = $titles [$this->id];
				echo "<h3>$title</h3>";
			}
		}
		echo "</div>";
	}
	public function get_meta_fb() {
		global $fb_domain;
		return "<meta property='og:image' content='$fb_domain$this->path'/>";
	}
}
function photo_set_print_thumbs($set) {
	foreach ( $set as $v ) {
		$v->print_thumb ();
	}
	;
}
function photo_set_print_imgs_m($set, $titles) {
	foreach ( $set as $v ) {
		$v->print_img_m ( $titles );
	}
	;
}
function photo_set_print_imgs($set, $titles) {
	foreach ( $set as $v ) {
		$v->print_img ( $titles );
	}
	;
}
function photo_set_meta_fb($set) {
	global $fb_meta_images;
	foreach ( $set as $v ) {
		$fb_meta_images .= $v->get_meta_fb ();
	}
	;
}
function photo_get_pattern($path,$pattern) {
	global $root;
	$jpg_local_dir = $root . $path;
	

	$jpg_local_array = glob ( $jpg_local_dir.'/' .$pattern );
	if (count ( $jpg_local_array )==0)
		return 0;
	$local=  $jpg_local_array [0];
	$path_parts = pathinfo ( $local );
	$path = $path . $path_parts ['basename'];

	return $path;
}
function photo_set_get($path) {
	global $root;
	$jpg_local_dir = $root . $path;
	
	$set = array ();
	
	$jpg_local_array = glob ( $jpg_local_dir . '/*.jpg' );
	foreach ( $jpg_local_array as $v ) {
		$set [] = new photo ( $v, $path );
	}
	return $set;
}

?>

