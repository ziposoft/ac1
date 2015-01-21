phantom.injectJs('acs.js');





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


/*

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


