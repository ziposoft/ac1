var system = require('system');
if (system.args.length === 1) {
    console.log('Try to pass some args when invoking this script!');
} else {
    system.args.forEach(function (arg, i) {
            console.log(i + ': ' + arg);
    });
}
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
function phantom_exit(code) {
   // if (page) page.close();
    setTimeout(function(){ phantom.exit(code); }, 0);
    phantom.onError = function(){};
    throw new Error('');
}


var fn_test_ready =function(context) {
	return 1;
		//return $("li.right").length>0;
};
var count=200;
function openpage()
{
	
	var page = require('webpage').create();

	page.onConsoleMessage = function(msg) {
	    console.log(msg);
	};
	page.onCallback = function(msg) {
	    console.log(":"+msg);
	    
	};	
	// Open Twitter on 'sencha' profile and, onPageLoad, do...
	page.open("http://massacremarathon.com/", function (status) {
	    // Check for page load success
		page.injectJs('client/jquery.js');
		page.evaluate(function() {
			window._ac$ = jQuery.noConflict(true); 
		});		
		console.log("page opened");
		var context="context";
	    if (status !== "success") {
	        console.log("Unable to access network");
	    } else {
	        // Wait for 'signin-dropdown' to be visible
	        waitFor(function() {
	            // Check in the page if a specific element is now visible
	            return page.evaluate(fn_test_ready,context);
	        }, function() {
	           console.log("The sign-in dialog should be visible now.");
	           
	           page.close();
	           delete page;
	           count--;
	           if(count)
	        	   setTimeout(openpage , 1000);
	        	  
	           else
	        	   
	        	   phantom_exit(0);
	        });        
	    }
	});
}
setTimeout(openpage , 2000);
setTimeout(openpage , 1100);
setTimeout(openpage , 800);
setTimeout(openpage , 500);

