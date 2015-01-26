phantom.injectJs('inc/acs.js');


var x=null;
if(x)
	console.log("x= something: "+x);

x="";
if(x)
	console.log("x= : "+x);



console.log(JSON.stringify(urlSplit("http://localhost/test_site/raceresults.html")));
console.log(JSON.stringify(urlSplit("https://localhost")));
console.log(JSON.stringify(urlSplit("/test_site/raceresults.html")));

phantom_exit(0);

function test(x)
{
	x=5;

}
b={};
test(b);
console.log(b);

phantom_exit(0);
var raceday = new Date("12/7/2014");
var bday = new Date("5/27/1970");

var time = new Date(raceday - bday); 
function calcAge(dateString,bday) {
	  var birthday = +new Date(bday);
	  var date= new Date(dateString);
	  return ~~(( date- birthday) / (31557600000));
	}

console.log("time=" +calcAge("12/7/2014","5/27/1970"))

var arr = [];

arr[4]="dad";
arr[5]="mom";
console.log(arr.length)

var o={}
o.name="fred"
o.date="12/5/2014";
o.city="Cary";
o.state="NC";

var f=new acsFile("output.txt")
f.out("hello?")
f.out("hello? too?")
f.close();
phantom_exit(0);
/*

 var scraper = new acs('http://onthemarksports.com/results/');



 scraper.evalTestReady=function(context) {
 if($('#tablepress-5').length>0)
 {
 console.log("results list ready");
 return true;
 }
 return false;
 };


 var fn_scrape = function(context) {

 var urls = [];
 try
 {
 var table = punchgs.com.greensock.TweenLite.selector.fn.dataTable.settings[0].aoData;
 jQuery.each( table, function( i, val ) {
 if(i<100)
 {
 var link = val._aData[1];
 var address=link.split('"');
 urls.push(address[1]);
 }

 });
 return urls;



 }
 catch(e) {}
 return null;
 } 
 scraper.onReady=function() {
 var context="context";	
 console.log("READY!");
 var js = scraper.p.evaluate(fn_scrape,context);
 console.log(js);
 };








 scraper.run();




 var scraper = new acs('http://localhost/test_site/ready.html');



 scraper.evalTestReady=function(context) {
 return $("li.right").length>0;
 };


 var fn_scrape = function(context) {
 var x;
 x=$("li").first().text();
 return x;
 } 
 scraper.onReady=function() {
 var context="context";	
 console.log("READY!");
 var js = scraper.p.evaluate(fn_scrape,context);
 console.log(js);
 };








 scraper.run();
 */
