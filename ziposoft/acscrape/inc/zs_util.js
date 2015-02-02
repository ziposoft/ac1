var fs = require('fs'), system = require('system');
phantom.injectJs('inc/jquery.js');
phantom.injectJs('inc/acutil.js');
phantom.injectJs('inc/runners.js');
phantom.injectJs('inc/acs_shared.js');


var gAcsUseCache=true;
if (system.args.length === 1)
{
	console.log('no args');
}
else
{
	system.args.forEach(function(arg, i)
	{
		if(arg=="refresh")
			gAcsUseCache=false;
			
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
			s.start();
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
			console.log('error ' + scraper.name + 'not running');
			return;
		}
		while (this.mapPending.length)
		{
			if (this.mapRunning.length >= this.maxrun) break;
			var next = this.mapPending.pop();
			this.run(next)
		}
		debugOut("Que=" + this.mapPending.length + " Running=" + this.mapRunning.length)
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
};

var acsCacheDir = "cache";
/*
 * acs constructor
 */

var acs = function(name, url)
{
	this.data = [];
	this.name = name;
	this.url = url;
	this.urlparts = urlSplit(url);
	this.useJquery = true;
	this.cacheLoad = gAcsUseCache;
	this.cacheSave = true;
	this.p = null;
	this.timer;
	this.timer_start;
	this.timer_timeout = 3000;
	this.timer_interval = 500;
	this.context = {};
	this.cacheFileName = acsCacheDir + "/" + this.name + "/cache.txt";
	this.tempFileName = acsCacheDir + "/" + this.name + "/temp.txt";
};
acs.prototype =
{
	dbg : function(s)
	{
		debugOut(this.name + " " + s);
	},
	makeFullUrl : function(url)
	{
		s = urlSplit(url);
		if (s.host) return url;
		var full = this.urlparts.protocol + "://" + this.urlparts.host + this.urlparts.path + '/' + url;
		return full;
	},
	evalScrapeTest : function()
	{
		return true;
	},
	evalScrape : function()
	{
		return {};
	},
	start : function()
	{
		var cd = acsCacheDir + "/" + this.name;
		this.dbg("START");
		if(this.cacheLoad)
		if (fs.isDirectory(cd))
		{
			if (fs.isFile(this.cacheFileName))
			{
				var r = fs.read(this.cacheFileName);
				if (r)
				{
					try{
					this.data = JSON.parse(r);
					if (this.data)
					{
						this.dbg("found cached data")
						this.cacheSave=false;
						this._processData();
					}
					return;						
					}
					catch(e)
					{
						console.log(e);
						
					}

				}
			}
		}
		else
		{
			fs.makeDirectory(cd);
		}
		this.scrapeStart();
	},
	onScapeComplete : function()
	{
		this.dbg("onScapeComplete");
		// console.log(JSON.stringify(this.data));
		this.scrapeEnd()
		fs.write(this.cacheFileName, JSON.stringify(this.data, null, '\t'));	
		this._processData();
	},
	onScapeError : function(error)
	{
		this.dbg("error:"+error);
		this.scrapeEnd()
		acsQue.done(this);
	},
	onScrapeTimeout : function()
	{
		this.dbg("Timeout");
		this.scrapeEnd()
		acsQue.done(this);
	},
	processData : function(data)
	{
		this.dbg("onProcessData");
	},
	onScrape : function()
	{
		this.dbg("onScrape");
		result= this.p.evaluate(this.evalScrape);
		this.data.push(result);
		if(this.cacheSave)
		{
			this.dbg("saving data to cache");
			fs.write(this.tempFileName, JSON.stringify(this.data, null, '\t'));			
		}		
		return result;
	},
	onScrapeTest : function()
	{
		return this.p.evaluate(this.evalScrapeTest);
	},
	_processData : function()
	{
		this.dbg("processData");

		for (var i = 0; i < this.data.length; i++) 
			this.processData(this.data[i]);
		acsQue.done(this);
	},
	scrapeEnd : function()
	{
		this.dbg("scrapeEnd");
		if (this.p)
		{
			this.p.close();
			delete this.p;
			this.p = null;
		}
	},
	scrapeTimerCallback : function(s)
	{
		s.dbg("TimerCallback");
		r = s.onScrapeTest();
		if (r == 1)
		{
			if (new Date().getTime() - s.timer_start < s.timer_timeout) return;
		}
		clearInterval(s.timer); // < Stop this interval
		if (r == 1) s.onScrapeTimeout();
		if (r == 0) s.onScapeError("Test Failed");
		if (r == 2)
		{
			result=s.onScrape();
			if(result.status=='more')
				return s.scrapeStartTimer();
			if(result.status=='done')
				return s.onScapeComplete();			
			return s.onScapeError('Error on scrape:'+result.status);			

		}
	},
	scrapeStartTimer : function()
	{
		debugOut(this.name + " scrapeStartTimer");
		this.timer_start = new Date().getTime();
		this.timer = setInterval(this.scrapeTimerCallback, this.timer_interval, this); // <
		// repeat
		// check
		// every
		// 250ms
	},
	scrapeStart : function()
	{
		var s = this;
		s.p = require('webpage').create();
		s.p.onConsoleMessage = function(msg)
		{
			console.log(msg);
		};
		s.p.onCallback = function(msg)
		{
			console.log(":" + msg);
		};
		s.p.onResourceTimeout = function(request) {
		    console.log('Response (#' + request.id + '): ' + JSON.stringify(request));
		};	
		s.p.open(s.url, function(status)
		{
			var context = "context";
			if (status != "success")
			{
				var status = "unknown";
				if (s.p.resource) status = s.p.resource.status;
				console.log('Page did not load (status=' + status + '): ' + s.url);
				s.onScapeError();
				return;
			}
			if (s.useJquery)
			{
				s.p.injectJs('inc/jquery.js');
				s.p.evaluate(function()
				{
					window._ac$ = jQuery.noConflict(true);
				});
				s.p.injectJs('inc/acs_shared.js');
				s.p.injectJs('inc/acs_client.js');

			}
			debugOut(s.name + " opened: " + s.url)
			s.scrapeStartTimer();
		});
	}
};
