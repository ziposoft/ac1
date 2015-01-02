<?php
$meta_extra='';
$page_title='';
$domain="http://landfilldogs.com";
$fb_link = $domain.$_SERVER['REQUEST_URI'];
$fb_title = "Shannon Johnstone";
$fb_image= $domain."/cover1.jpg";
$fb_meta_images="";
$photo_show_all=0;
$fb_description="A photographic project to showcase the beautiful souls of the most overlooked dogs. ";
date_default_timezone_set('US/Eastern');
/*
include $root.'/script/Mobile_Detect.php';
$detect = new Mobile_Detect;
$isPhone = ($detect->isMobile() && (!$detect->isTablet()));
$deviceType = ($detect->isMobile() ? ($detect->isTablet() ? 'tablet' : 'phone') : 'computer');
if(array_key_exists("m",$_GET))
    $isPhone=1;
*/

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

function setFlag( $name)
{
	$flag=getParam($name);
	if($flag)
	{
		if($flag=='on')
		{
			setcookie($name,'on',time()+90000000,'/');
		}
		if($flag=='off')
		{
			setcookie($name);
			$flag=0;
		}
	}
	else
	{
		$flag=getCookie($name);
	}
	return $flag;
}
$g_admin=setFlag('admin');
$g_debug=setFlag('debug');


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
?>