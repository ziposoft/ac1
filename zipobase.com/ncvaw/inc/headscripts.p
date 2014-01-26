<meta http-equiv="content-type" content="text/html;charset=iso-8859-1" />
<meta name="author" content="www.landfilldogs.info" />
<meta property="fb:app_id" content="556512511086342" />
<meta name="og:site_name" content="Landfill Dogs" />


<meta property="og:title"  content="<?php echo $fb_title ?>"/>
<meta property="og:type"  content="Website"/>  
<?php if(!$fb_meta_images)
	$fb_meta_images="<meta property='og:image' content='$fb_image'/>";
	echo("$fb_meta_images");

?>
<meta property="og:image:width" content="600">
<meta property="og:image:height" content="600">
<meta property="og:description"  content="<?php echo $fb_description ?>" />
    <?php echo $meta_extra ?>
    <title>North Carolina Voters For Animal Welfare<?php if($page_title)  echo ' - '.$page_title ?></title>
<link href='http://fonts.googleapis.com/css?family=Open+Sans:600,400,300' rel='stylesheet' type='text/css'>
<link href='/inc/zs_menu.css' rel='stylesheet' type='text/css'>
    




<script type="text/javascript">
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');
  ga('create', 'UA-6839263-2', 'landfilldogs.info');
  ga('send', 'pageview');
</script>

 <?php
function faq($question,$answer)
{
    echo("<p class='faq'><table>
    <tr><td class='faq_bullet'>Q: </td><td class='faq_text' style='font-style:italic'>$question</td></tr>
    <tr><td class='faq_bullet'>A: </td><td class='faq_text'>$answer</td></tr></table></p>");
}

function media_link($photo,$date,$source,$link,$title)
{
    echo("<div  style='margin:5px;padding:15px;border-top:solid 3px #aaa;display:block;clear:both' >
    <a   target='_blank' href='$link'><img style='margin-right:5px; width:200px;float:left;' src='$photo'/></a><p>$date</p><p>$source</p><p ><a  target='_blank' href='$link'>$title</a></p></div>");
}
 ?> 