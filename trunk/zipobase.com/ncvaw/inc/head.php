<!DOCTYPE html>
<html>
<head>

<meta name="viewport" content="width=device-width initial-scale=1.0 maximum-scale=1.0 user-scalable=yes" />
<meta http-equiv="content-type" content="text/html;charset=iso-8859-1" />
<meta name="author" content="www.ncvaw.org" />



<!-- FACEBOOK  -->
<meta property="fb:app_id" content="490628621063190" />
<meta property="og:site_name" content="NC Voters For Animal Welfare" />
<meta property="og:type" content="Website" />

<?php
if ($fb_description)
	echo ("<meta property='og:description' content='$fb_description'/>");

if (! $fb_meta_images)
	$fb_meta_images = "<meta property='og:image' content='$fb_image'/>";
echo ("$fb_meta_images");

?>	
	
<link href='/inc/ncvaw.css' rel='stylesheet' type='text/css'>
<link href='/inc/zs_menu.css' rel='stylesheet' type='text/css'>
<title>
<?php
if ($page_title)
	echo ("NCVAW - $page_title");
else
	echo ("NC Voters For Animal Welfare");
?></title>
<meta property="og:title" content="<?php echo $page_title ?>" />
    
<?php 
	include $root . '/inc/headscripts.p';
	include $root . '/inc/zs_menu.php';	?>
</head>

<body>
	<div id="wrapper">
		<div style="text-align: center;">
			<a style="margin:5px" href="/home.html">
			<img style="float: left; max-width: 25%;" src="/img/NCVAW_paw.png"
				alt="North Carolina Voters for Animal Welfare"></a>
				<a style="margin: 5px" href="/home.html"> <img
				style="text-align: center; margin: auto; max-width: 40%"
				src="/img/title.png"></a> <a href="http://www.facebook.com/NCVAW"
				target="_new"><img style="float: right; max-width: 25%"
				src="/img/facebook.png" alt="Follow us on Facebook"></a>
				<?php /*
					  Removed the FB like box because it was too slow to load.
						<div style="float: right;max-width:30%"><small style="color:#46a;font:">Follow us on Facebook</small>
						<div style="display:block;padding:0;margin:0;" class="fb-like-box" data-href="http://www.facebook.com/ncvaw" data-width="220" data-height="70" data-colorscheme="light" data-show-faces="false" data-header="false" data-stream="false" data-show-border="false"></div>
						</div> */ ?>
		</div>
		<div style='clear: both'></div>
  	
<?php
$menu_admin = 0;

if ($admin) {
	$menu_admin = z_menu ( "Admin", [ 
			z_mi ( "Admin Off", "/home.html?admin=off", "" ) 
	] );
}
make_menu ( [ 
		z_menu ( "About", [ 
				z_mi ( "About Us", "/about/aboutus.html", "HOME" ),
				z_mi ( "Who We Are", "/about/who.html", "MAP" ),
				z_mi ( "Contact", "/about/contact.html", "MAP" ),
				z_mi ( "Lobbying 101", "/about/lobbying101.html", "" ),
				z_mi ( "How You Can Help", "/about/getinvolved.html", "" ) 
		] ),
		z_mbi ( "News", "/news.html", "Support Animal Welfare in NC", 0 ),
		z_mbi ( "Facebook Feed", "/facebook.html", "Support Animal Welfare in NC", 0 ),
		z_mbi ( "Donate", "http://www.ncvaw.org/donate/", "Support Animal Welfare in NC", 0 ),
		
		z_menu ( "Your Districts", [ 
				z_mi ( "Find your district", "/guide/find.html", "" ),
				z_mi ( null, "", "", "z_mi_dist_senate" ),
				z_mi ( null, "", "", "z_mi_dist_house" ),
				z_mi ( "List of all Districts", "/districts.php", "" )
		] ),
		z_menu ( "Legislature Guide", [
			z_mi ( "District Lists", "/districts.php", "" ),
			z_mi ( "Recent Legislation", "/bills.php", "" ),
			z_mi ( "Senate List", "/guide/leglist.html?ch=S", "HOME" ),
			z_mi ( "House List", "/guide/leglist.html?ch=H", "" ),
			z_mi ( "Complete List", "/guide/leglist.html", "" )
			]
		),
		
		z_menu ( "Voters Guide", [ 
				z_mi ( "Find Who Represents You", "/guide/find.html", "" ),
				z_mi ( "In The News", "/links.php", "" ),
				z_mi ( "Recent Legislation", "/bills.php", "" ),
				z_mi ( "Canidates Survey", "/guide/survey.html", "" ),
		]
		 ),
		($admin ? z_menu ( "Admin", [ 
				z_mi ( "Debug", "/guide/debug.html" ),
				z_mi ( "Admin Off", "/home.html?admin=off" ),
				z_mi ( "Delete Pdata", "/home.html?delpdata=true" ),
				z_mi ( "Delete all", "/home.html?deldata=true" ),
				z_mi ( "PHP Info", "/phpinfo.php" ),
				z_mi ( "Refresh", "$current_url?refresh=true" ) 
		] ) : 0) 
]
 );

?>
		<div id="main">