<?php


$zs_foot_jsfile=["/inc/jquery.js","/inc/ncvaw.js","/inc/zs_menu.js"];
$zs_head_jsfile=array();
$zs_foot_script=array();

function getCookie( $name)
{
	if(array_key_exists($name,$_COOKIE))
		return $_COOKIE[$name];
	return 0;
}
function getParam( $name)
{
	if(array_key_exists($name,$_GET))
		return $_GET[$name];
	return 0;
}
$meta_extra='';
$page_title='';
$fb_domain="http://landfilldogs.com";
$fb_link = $fb_domain.$_SERVER['REQUEST_URI'];
$fb_title = "Shannon Johnstone";
$fb_image= $fb_domain."/cover1.jpg";
$fb_meta_images="";
$fb_description="NCVAW";
date_default_timezone_set('US/Eastern');
include $root.'/inc/Mobile_Detect.php';
$detect = new Mobile_Detect;
$isPhone = ($detect->isMobile() && (!$detect->isTablet()));
$deviceType = ($detect->isMobile() ? ($detect->isTablet() ? 'tablet' : 'phone') : 'computer');

/*
$isPhone=getParam("m");

$isPhone=0;
if($isPhone)
{
    $header=$root.'/inc/mheader.p';
    $footer=$root.'/inc/mfooter.p';
}
else
{
    $header=$root.'/inc/header.p';
    $footer=$root.'/inc/footer.p';
}
*/
$header=$root.'/inc/header.p';
$footer=$root.'/inc/footer.p';
$dist_state_senate=getCookie('dist_state_senate');
$dist_state_house=getCookie('dist_state_house');
$debug=getCookie('debug');


?>