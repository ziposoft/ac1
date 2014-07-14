
<?php

   $xslDoc = new DOMDocument();
   $xslDoc->load("trailmap.xsl");

   $xmlDoc = new DOMDocument();
   $xmlDoc->load("Umstead.svg");

   $proc = new XSLTProcessor();
   $proc->importStylesheet($xslDoc);
   echo $proc->transformToXML($xmlDoc);

?>