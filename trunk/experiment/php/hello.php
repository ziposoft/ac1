<html>
 <head>
  <title>PHP Test</title>
 </head>
 <body>
 <?php echo '<p>Hello World</p>';
  echo $_GET["dog"]; 
  echo $_GET["proj"]; 
  $dir    = '.';
$files1 = scandir($dir);
$images_array = glob('*.jpg');
#$files2 = scandir($dir, 1);

#print_r($images_array);
#print_r($files2);
 foreach ($images_array as $v){
echo '<img src="', $v, '" />';
}
 ?> 
 
 <?php 
    #print_r($images_array);
    ?>
 </body>
</html>