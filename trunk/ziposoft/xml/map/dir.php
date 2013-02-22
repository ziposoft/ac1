<trails>
 <?php
if ($handle = opendir('trails')) {
    /* This is the correct way to loop over the directory. */
    while (false !== ($entry = readdir($handle))) 
    {
        echo "<trail>$entry</trail>\n";

    }
    closedir($handle);
}
?>
</trails>