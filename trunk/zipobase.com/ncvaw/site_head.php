<?php

$options=[
'grades'=>1,
'legpages'=>0
];
function option( $name)
{
	global $options;
	if(array_key_exists($name,$options))
		return $options[$name];
	return null;
}

$zs_foot_jsfile=["/inc/jquery.js","/inc/ncvaw.js","/inc/zs_menu.js"];
$zs_head_jsfile=array();
$zs_foot_script=array();
$funcs_init=["ncvaw_init()"];
function getCookie( $name)
{
	if(array_key_exists($name,$_COOKIE))
		return $_COOKIE[$name];
	return null;
}

function getParam( $name)
{
	if(array_key_exists($name,$_GET))
		return $_GET[$name];
	return null;
}
$meta_extra='';
$page_title='';

$fb_domain="http://elect.ncvaw.org";
$fb_link = $fb_domain.$_SERVER['REQUEST_URI'];
$fb_title = "NCVAW";
$fb_image= $fb_domain."/img/coverfb2.jpg";
$fb_meta_images="";
$fb_description="Find your districts and representatives, browse the report cards, and make sure your NC legislators are fighting for animal welfare.";

date_default_timezone_set('US/Eastern');

/*
include $root.'/inc/Mobile_Detect.php';
$detect = new Mobile_Detect;
$isPhone = ($detect->isMobile() && (!$detect->isTablet()));
$deviceType = ($detect->isMobile() ? ($detect->isTablet() ? 'tablet' : 'phone') : 'computer');
*/

$current_url =$_SERVER['PHP_SELF'];

$refresh_data=getParam("refresh");

if($refresh_data)
{
	array_map('unlink', glob("$root/data/*.json"));
}
if(getParam("deldata"))
{
	array_map('unlink', glob("$root/data/*.json"));
	array_map('unlink', glob("$root/data/*.pdata"));
}
if(getParam("delpdata"))
{
	array_map('unlink', glob("$root/data/*.pdata"));
}
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
$header=$root.'/inc/head.php';
$footer=$root.'/inc/foot.php';

$admin=getParam('admin');
if($admin)
{
	if($admin=='on')
	{
		setcookie('admin','on',time()+90000000,'/');
	}	
	if($admin=='off')
	{
		setcookie('admin');
		$admin=0;
	}	
}
else
{
	$admin=getCookie('admin');
}




?>