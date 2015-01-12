


var page = require('webpage').create();
//var url = 'http://onthemarksports.com/results/';
var url = 'http://localhost/';

page.open(url, function (status) {
	
    if (status != "success") {
    	console.log('Page did not load (status=' + status + '): ' + url);
           
        return;
    }
    // look for 4xx or 5xx status codes
    var statusCodeStart = page.resource && String(page.resource.status).charAt(0);
    if (statusCodeStart == '4' || statusCodeStart == '5') {
        if (page.resource.status == 404) {
        	console.log('Page not found: ' + url);
        } else {
        	console.log('Page error code ' + page.resource.status + ' on ' + url);
        }
        complete(false);
        return;
    }	
	
	
	
	page.injectJs('client/jquery.js');
	var js = page.evaluate(function () {
		return $('h1');
	});
	console.log(JSON.stringify(js));
	phantom.exit();
});

phantom.exit();
exit();
/*
var page = require('webpage').create({
    // set up console output
    onConsoleMessage: function(msg, line, id) {
        // kill initialization message
        if (msg.indexOf('___') === 0) return;
        id = id || 'injected code';
        if (line) msg += ' (' + id + ' line ' + line + ')';
        log.msg('CLIENT: ' + msg);
    },
    onAlert: function(msg) { log.alert('CLIENT: ' + msg); }
});
*/

var acscraper = (function(){
    var config = {

        };
        
    var suites = [];
    var page;
    var x;
    
    
    return {
        x: x,




        /**
         * Add one or more scraper suites to be run.
         * @name pjs.addSuite
         * @param {Object} suite    Scraper suite configuration object
         * @param {Object} [...]    More suite configuration objects
         */
        addSuite: function() { 
            suites = Array.prototype.concat.apply(suites, arguments);
        },

        /**
         * Shorthand function to add a simple scraper suite.
         * @name pjs.addScraper
         * @param {String|String[]} url     URL or array of URLs to scrape
         * @param {Function|Function[]}     Scraper function or array of scraper functions
         */
        addScraper: function(url, scraper) {
            suites.push({url:url, scraper:scraper});
        }
    };
}());    
