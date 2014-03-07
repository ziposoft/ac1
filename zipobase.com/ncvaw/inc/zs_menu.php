<?php

function z_out_ch(&$str,$items) {
	foreach ( $items as $item )
	 {
		if($item)
			$str .=$item;
		
	}
}
function z_mi($text,$link,$help=null,$id=null) 
{ 
	
	return "<li  id='$id' class='zs_menu_item " . ($text==null? 'zs_menu_hide':'') ."' ><a class='zs_menu_item' href='$link' >$text</a></li>";
}

function z_mbi($text,$link,$help=null,$id=null) {
	return("<li class='menu_bar_item' id='$id'><a class='zs_menu_item' href='$link' >$text</a></li>");
	
 }
 function z_menu($text,$items,$id=null)
 {
 	$str="<li class='" . ($text==null? 'zs_menu_hide':'zs_menutop');
 		
 	$str.="' id='$id' onmouseover='zs.menu.show(this,event);' onmouseout='zs.menu.hide(this,event);'>
 			<a class='zs_menu_link' >$text</a><ul class='zs_menu_hide'>";
 	z_out_ch($str,$items);
 	$str.="</ul></li>";
 	return $str;
 }

 function z_menu_sub($text,$items)
 {
 	$str="<li class='zs_menu_item_sub' onmouseover='zs.menu.show_sub(this,event);'
 			onmouseout='zs.menu.hide_sub(this,event);'>
 			<a class='zs_menu_link' >$this->text  &#9658;</a><ul class='zs_menu_hide' >";
 	z_out_ch($str,$items);
 	$str.="</ul></li>";
 	return $str;
 }
 
 function make_menu($items)
 {
 	$str="<div  class='zs_menu_all'
 				onmouseover='zs.menu.show(this,event);'
 			onmouseout='zs.menu.hide(this,event);'	
 			><span class='zs_menu_content' 	></span>
 			<ul class='zs_menu_bar zs_mb_hide'>";
 	z_out_ch($str,$items);
 	$str.="</ul></div>";
 	echo( $str);
 }
 
?>





