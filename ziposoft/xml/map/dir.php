<trails>
 <?php
if ($handle = opendir('trails')) {
    /* This is the correct way to loop over the directory. */
    while (false !== ($entry = readdir($handle))) 
    {
        if($entry == ".")   continue;
        if($entry == "..")   continue;
        if($entry == "dir.php")   continue;
        echo "<trail>$entry</trail>\n";

    }
    closedir($handle);
}
?>
</trails>