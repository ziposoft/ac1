<?php
$page_title='List Of Bills';
include $header;
include $root.'/inc/db.php';



/*
function file_get_contents_curl($url)
{
	$ch = curl_init();

	curl_setopt($ch, CURLOPT_HEADER, 0);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);

	$data = curl_exec($ch);
	curl_close($ch);

	return $data;
}

$html = file_get_contents_curl("http://watchdogwire.com/northcarolina/2013/10/25/nc-senator-sponsor-ag-gag-whistle-blower-bill-meets-with-felon-usda-inspector/");

//parsing begins here:
$doc = new DOMDocument();
@$doc->loadHTML($html);
$nodes = $doc->getElementsByTagName('title');

//get and display what you need:
$title = $nodes->item(0)->nodeValue;
$keywords="";
$description="";
$image="";

$metas = $doc->getElementsByTagName('meta');

for ($i = 0; $i < $metas->length; $i++)
{
    $meta = $metas->item($i);
    if($description=="")
    if($meta->getAttribute('name') == 'description')
        $description = $meta->getAttribute('content');

    if($image=="")
	    if($meta->getAttribute('property') == 'og:image')
	    	$image = $meta->getAttribute('content');    
}

echo ("<img src='$image'/>");
echo ("<h4>$title</h4>");
echo ("<p>$description</p>");

*/


$exlinks=getobj("exlinks");

$exlinks->print_list ();
?>

<?php include $footer; ?>