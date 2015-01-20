//phantomjs

var system = require('system');
if (system.args.length === 1) {
    console.log('Try to pass some args when invoking this script!');
} else {
    system.args.forEach(function (arg, i) {
            console.log(i + ': ' + arg);
    });
}


/*
phantom.onError = function(msg, trace) {
  var msgStack = ['PHANTOM ERROR: ' + msg];
  if (trace && trace.length) {
    msgStack.push('TRACE:');
    trace.forEach(function(t) {
      msgStack.push(' -> ' + (t.file || t.sourceURL) + ': ' + t.line + (t.function ? ' (in function ' + t.function +')' : ''));
    });
  }
  console.error(msgStack.join('\n'));
  phantom.exit(1);
};
*/


function waitFor(testFx, onReady, timeOutMillis) {
    var maxtimeOutMillis = timeOutMillis ? timeOutMillis : 3000, //< Default Max Timout is 3s
        start = new Date().getTime(),
        condition = false;
	console.log("waitFor");    
    var interval = setInterval(
    	function()
    	{
    		console.log("interval");
            if ( (new Date().getTime() - start < maxtimeOutMillis) && !condition ) 
            {
                // If not time-out yet and condition not yet fulfilled
                condition = (typeof(testFx) === "string" ? eval(testFx) : testFx()); //< defensive code
                console.log("'condition="+condition);
            } else {
                if(!condition) {
                    // If condition still not fulfilled (timeout but condition is 'false')
                    console.log("'waitFor()' timeout");
                    phantom.exit(1);
                } else {
                    // Condition fulfilled (timeout and/or condition is 'true')
                    console.log("'waitFor()' finished in " + (new Date().getTime() - start) + "ms.");
                    typeof(onReady) === "string" ? eval(onReady) : onReady(); //< Do what it's supposed to do once the condition is fulfilled
                    clearInterval(interval); //< Stop this interval
                }
            }
        },
        250); //< repeat check every 250ms
};
var page = require('webpage').create();
page.onConsoleMessage = function(msg) {
    console.log(msg);
};
page.injectJs('client/jquery.js');
//var url = 'http://onthemarksports.com/results/';
var url = 'http://localhost/test_site/ready.html';
//Calls to "callPhantom" within the page 'p' arrive here
page.onCallback = function(msg) {
    console.log(":"+msg);
    
};

var fn_scrape = function(context) {

	window.callPhantom(context)
	
	var x;
	
	//x=$('li').first().text();
	x=$("li").first().text();
	return x;
} 
var fn_test_ready =function(context) {
	window.callPhantom("fn_test_ready");
	return $("li.right").length>0;
};
page.open(url, function(status) 
{
	var context="context";
	
	if (status != "success") 
	{
		var status="unknown";
		if(page.resource)
			status = page.resource.status;
		console.log('Page did not load (status=' + status + '): ' + url);
			complete();
	        return;
	    }
	
	console.log("calling wait!");
	waitFor(function() {
         // Check in the page if a specific element is now visible
         return page.evaluate(fn_test_ready,context);
     }, function() {
        console.log("READY!");
     });        	
	var js = page.evaluate(fn_scrape,context);
	console.log(js);
	complete();
});



var complete = function() {
	console.log("done!");
	phantom.exit();
		
};



