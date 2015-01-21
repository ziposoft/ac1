var fs = require('fs'),
    system = require('system');
phantom.injectJs('acutil.js');
phantom.injectJs('acs.js');

if (system.args.length === 1) {
    console.log('no args');
} else {
    system.args.forEach(function (arg, i) {
            console.log(i + ': ' + arg);
    });
}




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



