
<?php
class zs_menu_base {
	var $text;
	public function __construct() {
	}
	public function out() {
	}
}
class zs_menu_item  {
	var $link;
	var $help;
	var $text;
	public function __construct($text,$link,$help) {
		$this->text=$text;
		$this->link=$link;
		$this->help=$help;
	}
	public function out() {
		//echo("<tr><td class='zs_menu_link' ><a class='zs_menu_link' href='$this->link' >$this->text</a></td></tr>");
		echo("<li class='zs_menu_item' ><a class='zs_menu_item' href='$this->link' >$this->text</a></li>");
		
	}
}
function z_mi($text,$link,$help) { return new zs_menu_item($text,$link,$help); }

class zs_menubar_item  {
	var $link;
	var $help;
	var $text;
	public function __construct($text,$link,$help) {
		$this->text=$text;
		$this->link=$link;
		$this->help=$help;
	}
	public function out() {
		//echo("<td class='menu_bar_item' ><a class='zs_menu_link' href='$this->link' >$this->text</a></td>");
		echo("<li class='menu_bar_item' ><a class='zs_menu_item' href='$this->link' >$this->text</a></li>");
		
	}
}
function z_mbi($text,$link,$help) { return new zs_menubar_item($text,$link,$help); }

class zs_menu extends zs_menu_base {
	public $items; // array
	public function __construct($text,$items) {
		$this->text=$text;
		$this->items=$items;
	}
	public function out_ch() {
		foreach ( $this->items as $item ) {
			$item->out ();
		}
	}
	public function out() {
		echo ("<li class='zs_menutop' 
				onmouseover='zs.menu.show(this,event);' onmouseout='zs.menu.hide(this,event);'>
				<a class='zs_menu_link' >$this->text</a><ul class='zs_menu'>");
		
		$this->out_ch ();
		echo ("</ul></li>");
	}
}
function z_menu($text,$items) 
{ 
	$menu= new zs_menu($text,$items);
	return $menu;	
 }
class zs_menu_sub extends zs_menu {
	public function __construct($text,$items) {
		$this->text=$text;
		$this->items=$items;
	}
	public function out() {
		echo ("<li class='zs_menu_item_sub' onmouseover='zs.menu.show_sub(this,event);' 
				onmouseout='zs.menu.hide_sub(this,event);'>	
				<a class='zs_menu_link' >$this->text  &#9658;</a><ul class='zs_menu_sub' >");
		$this->out_ch ();
		echo ("</ul></li>");
	}
}
function z_menu_sub($text,$items)
{
	$menu= new zs_menu_sub($text,$items);
	return $menu;
}
class zs_menu_bar extends zs_menu {
	public function __construct($items) {
		$this->items=$items;
	}
	public function out() {
		//echo ("<table class='zs_menu_bar' cellspacing='0' cellpadding='0'><tr>");
		echo("<ul class='zs_menu_bar'>");
		$this->out_ch ();
		echo("</ul>");
		//echo ("</tr></table>");
	}
}
class mod_zs_menu {
	public function __construct($text) {
		
	}
	public function get_file_css() {
		return "zs_menu.css";
	}
	public function get_file_js() {
		return "zs_menu.js";
	}
}
function make_menu($items)
{
	$bar = new zs_menu_bar($items);	
	$bar->out();
	
}
?>





