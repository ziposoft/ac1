var xslStylesheet;
var xsltProcessor = new XSLTProcessor();
var myDOM;
 
var xmlDoc;
 
function runit(){
 
  // load the xslt file, example1.xsl
  var myXMLHTTPRequest = new XMLHttpRequest();
  myXMLHTTPRequest.open("GET", "ajax.xsl", false);
  myXMLHTTPRequest.send(null);
 
  xslStylesheet = myXMLHTTPRequest.responseXML;
  xsltProcessor.importStylesheet(xslStylesheet);
 
 
  // load the xml file, example1.xml
  myXMLHTTPRequest = new XMLHttpRequest();
  myXMLHTTPRequest.open("GET", "ajax.xml", false);
  myXMLHTTPRequest.send(null);
 
  xmlDoc = myXMLHTTPRequest.responseXML;
 
  var fragment = xsltProcessor.transformToFragment(xmlDoc, document);
 
  document.getElementById("example").innerHTML = "";
 
  myDOM = fragment;
  document.getElementById("example").appendChild(fragment);
}