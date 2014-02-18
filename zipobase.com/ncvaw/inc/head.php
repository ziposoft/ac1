<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width initial-scale=1.0 maximum-scale=1.0 user-scalable=yes" />
    <link href='/inc/ncvaw.css' rel='stylesheet' type='text/css'>
    <?php 
    include $root.'/inc/headscripts.p';
    include $root.'/inc/zs_menu.php';
    

    ?>






</head>

<body>

 <?php
	include $root . '/inc/fb_script.p';
	?>
  
<div id="wrapper">
<a href="http://www.ncvaw.org/"><img style="float: left" src="/img/NCVAW_logo.png" alt="North Carolina Voters for Animal Welfare"></a>
<span style="color:#46a;font-size: 30px;">2014 ELECTION CENTRAL</h1>
</span>

<div style="float: right"><small style="color:#46a;font:">Follow us on Facebook</small>
	<div style="display:block;padding:0;margin:0;" class="fb-like-box" data-href="http://www.facebook.com/ncvaw" data-width="300" data-height="70" data-colorscheme="light" data-show-faces="false" data-header="false" data-stream="false" data-show-border="false"></div>
</div>

<!--  
<a href="/"><img style="float: left" src="/img/NCVAW_paw.png" alt="North Carolina Voters for Animal Welfare"></a>
 
<a href="/"><img style="clear:right;margin-top:10px" src="/img/NCVAW_txt.png" alt="North Carolina Voters for Animal Welfare"></a>
-->
  	
<?php
$menu_admin=0;

if($admin)
{
$menu_admin=	z_menu("Admin",[
		z_mi("Admin Off","/home.html?admin=off",""),	
		]);
}
make_menu([
	z_menu("About",[
		z_mi("About Us","/about/aboutus.html","HOME"),
		z_mi("Who We Are","/about/who.html","MAP"),
		z_mi("Contact","/about/contact.html","MAP")]),

	z_mbi("News","http://www.ncvaw.org/news/","Support Animal Welfare in NC",0),
	z_mbi("Donate","/about/donate.html","Support Animal Welfare in NC",0),
	
	z_menu("About",[
		z_mi("About Us","/about/aboutus.html","HOME"),
		z_mi("Who We Are","/about/who.html","MAP"),
		z_mi("Contact","/about/contact.html","MAP")
		]),	
		
	z_menu("Your Districts",[	
	    z_mi("Find your district","/guide/find.html",""),
		z_mi(null,"","","z_mi_dist_senate"),
		z_mi(null,"","","z_mi_dist_house"),]),
	
	

	z_menu("Get Involved",[
		z_mi("Lobbying 101","/about/lobbying101.html",""),
		z_mi("How You Can Help","/about/getinvolved.html","")]),		
	z_menu("Voters Guide",[
		z_mi("Find your district","/guide/find.html",""),	
		//z_menu_sub("Search Legislature",[
		z_mi("Senate List","/guide/senate.html","HOME"),
		z_mi("House List","/guide/house.html",""),
		//]),	
		//z_mi("LEGISLATIVE REPORT CARD","/guide/leglist.html","HOME"),


		]),
		($admin? 
			z_menu("Admin",[
				z_mi("Admin Off","/home.html?admin=off")	,
				z_mi("PHP Info","/phpinfo.html")	,
				]):
			0)
			
		
]);

         
         
         
?>
		<div id="main">