//phantomjs

var system = require('system');
if (system.args.length === 1) {
    console.log('Try to pass some args when invoking this script!');
} else {
    system.args.forEach(function (arg, i) {
            console.log(i + ': ' + arg);
    });
}
phantom.exit(0);


/*
 * AcScraper constructor
 */
var AcScraper = function (urlstart) {
	  this.firstName = firstName;
	  console.log('Person instantiated');
	};

	var person1 = new Person('Alice');
	var person2 = new Person('Bob');


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


var page = require('webpage').create();
//Calls to "callPhantom" within the page 'p' arrive here
page.onCallback = function(msg) {
    console.log(":"+msg);
};


//var url = 'http://onthemarksports.com/results/';
var url = 'http://localhost/';
//Calls to "callPhantom" within the page 'p' arrive here
page.onCallback = function(msg) {
    console.log(":"+msg);
    
};
var context_obj= {
		name : "fred",
		set : function(val)
		{
			this.name = val;
		}
	};
var fn_scrape = function(context) {
	context.stuff="poo poo!"
	window.callPhantom(context.stuff)
	console.log(context);	

	return document.title;
} 
var fn_test_ready =function(context) {
	return true;
} 
page.open(url, function(status) 
{
	var context=Object();
	context.stuff="shit";
	
	if (status != "success") {
		var status="unknown";
		if(page.resource)
			status = page.resource.status;
		console.log('Page did not load (status=' + status + '): ' + url);
			complete();
	        return;
	    }
	var js = page.evaluate(fn_scrape,context);
	console.log(js);
	console.log(context.stuff);	
	complete();
});



var complete = function() {
	console.log("done!");
	phantom.exit();
		
};
console.log("here!");




function waitFor(testFx, onReady, timeOutMillis) {
    var maxtimeOutMillis = timeOutMillis ? timeOutMillis : 3000, //< Default Max Timout is 3s
        start = new Date().getTime(),
        condition = false,
        interval = setInterval(function() {
            if ( (new Date().getTime() - start < maxtimeOutMillis) && !condition ) {
                // If not time-out yet and condition not yet fulfilled
                condition = (typeof(testFx) === "string" ? eval(testFx) : testFx()); //< defensive code
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
        }, 250); //< repeat check every 250ms
};