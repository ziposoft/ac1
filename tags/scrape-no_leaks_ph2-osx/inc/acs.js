var fs = require('fs'), system = require('system');
phantom.injectJs('inc/acutil.js');
phantom.injectJs('inc/jquery.js');
phantom.injectJs('inc/runners.js');
if (system.args.length === 1)
{
	console.log('no args');
}
else
{
	system.args.forEach(function(arg, i)
	{
		console.log(i + ': ' + arg);
	});
}
var acsQue =
{
	processed : 0,
	maxrun : 4,
	mapPending : [],
	mapRunning : [],
	mapDone : [],
	run : function(s)
	{
		if (this.mapRunning.length < this.maxrun)
		{
			this.mapRunning.push(s);
			console.log('Running ' + s.name);
			//setTimeout(acsScrapePage, 1000, s);
			acsScrapePage(s);
			return true;
		}
		return false;
	},
	add : function(s)
	{
		this.processed++;
		if (!this.run(s))
		{
			this.mapPending.push(s);
			console.log('Queueing ' + s.name);
		}
	},
	done : function(scraper)
	{
		var s = popByName(this.mapRunning, scraper.name);
		if (!s)
		{
			console.log('error ' + s.name + 'not running');
			return;
		}
		while (this.mapPending.length)
		{
			if (this.mapRunning.length >= this.maxrun) break;
			var next = this.mapPending.pop();
			this.run(next)
		}
		if (this.mapRunning.length == 0)
		{
			console.log('Processed ' + this.processed);
			console.log('Queue empty, exiting');
			this.onComplete();
			// phantom_exit(0);
		}
	},
	onComplete : function()
	{
		console.log('COMPLETE!');
	}
}
/*
 * 
 * 
 * 
 * 
 * acs constructor
 */
var acs = function(name, url)
{
	this.data={};
	this.name = name;
	this._url = url;
	this.useJquery = true;
	
	this._timer;
	this._timer_start;
	this._timer_timeout = 30000;
	this._timer_interval = 250;
	this.context = "context";
};
acs.prototype =
{
	evalScrapeTest : function()
	{
		return true;
	},
	evalScrape : function()
	{
		return true;
	},
	onScapeComplete : function()
	{
		console.log(this.name + " Complete");
		//console.log(JSON.stringify(this.data));
		this.processData();
		acsQue.done(this);
	},
	onScapeError: function(error)
	{
		console.log(this.name + " error");
		acsQue.done(this);
	},
	onScrapeTimeout : function()
	{
		console.log(this.name + " Timeout");
		acsQue.done(this);
	},
	processData : function()
	{
		console.log("processData");
	},
	
	
};
function waitFor(testReady, onReady, onTimeout, onComplete, timeOutMillis)
{
	var maxtimeOutMillis = timeOutMillis ? timeOutMillis : 15000;
	var start = new Date().getTime(), condition = false;
	var interval = setInterval(function()
	{
		if (testReady())
		{
			clearInterval(interval);
			if (onReady())
				onComplete();
			else
				{
				debugOut("calling waitFor recursively")
				waitFor(testReady, onReady, onTimeout, onComplete, timeOutMillis);
				
				}
		}
		else if ((new Date().getTime() - start > maxtimeOutMillis))
		{
			// If not time-out yet and condition not yet fulfilled
			console.log("'waitFor()' timeout");
			onTimeout();
			clearInterval(interval); // < Stop this interval
		}
	}, 500); // < repeat check every 250ms
};
function acsScrapePage(s)
{
	var page = require('webpage').create();
	page.onConsoleMessage = function(msg)
	{
		console.log(msg);
	};
	page.onCallback = function(msg)
	{
		console.log(":" + msg);
	};
	// Open Twitter on 'sencha' profile and, onPageLoad, do...
	page.open(s._url, function(status)
	{
		// Check for page load success
		if(s.useJquery)
		{
			page.injectJs('inc/jquery.js');
			page.evaluate(function()
			{
				window._ac$ = jQuery.noConflict(true);
			});
			page.injectJs('inc/acs_client.js');			
		}
		
		if (status !== "success")
		{
			var status = "unknown";
			if (page.resource) status = page.resource.status;
			console.log('Page did not load (status=' + status + '): ' + s._url);
			page.close();
			delete page;			
			s.onScapeError(status);
			return false;
		}
		waitFor(
			function() // testReady
			{
				return page.evaluate(s.evalScrapeTest, s.context);
			}, 
			function() //onReady
			{
				s.data= page.evaluate(s.evalScrape, s.context);
				return true;
			},
			function() //onTimeout
			{
				page.close();
				delete page;
				s.onScrapeTimeout();
			},
			function() //onComplete
			{
				page.close();
				delete page;
				s.onScapeComplete();
			}			
		); //End waitFor
		
	});
	return true; //started OK
}

