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
			console.log('Running '+s.name);
			s.run();
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
			console.log('Queueing '+s.name);
		}
	},
	done : function(scraper)
	{
		var s = popByName(this.mapRunning,scraper.name);
		if (!s)
		{	
			console.log('error '+s.name+'not running');
			return;
		}
		
		while(this.mapPending.length)
		{
			if (this.mapRunning.length >= this.maxrun)
				break;
			var next=this.mapPending.pop();
			this.run(next)

		}		
		if(this.mapRunning.length==0)
		{
			console.log('Processed '+this.processed);
			console.log('Queue empty, exiting');
			this.onComplete();
			
			phantom_exit(0);
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
 * acs constructor
 */
var acs = function(name, url)
{
	this.name = name;
	this._url = url;
	this.p = require('webpage').create();
	this.p.onConsoleMessage = function(msg)
	{
		console.log(msg);
	};
	this.p.onCallback = function(msg)
	{
		console.log(":" + msg);
	};
	this._timer;
	this._timer_start;
	this._timer_timeout = 30000;
	this._timer_interval = 250;
	this._context = "context";
	this.eval
};
acs.prototype =
{
	evalTestReady : function()
	{
		return true;
	},
	evalScrape : function()
	{},
	onComplete : function()
	{
		this.p.close();
		//console.log(this.name + " completed!");
		acsQue.done(this);
	},
	onTestReady : function()
	{
		//console.log(this.name + " loading...");
		return this.p.evaluate(this.evalTestReady, this._context);
	},
	onReady : function()
	{
		console.log("'waitFor()' finished in " + (new Date().getTime() - this._timer_start) + "ms.");
	},
	onTimeout : function()
	{
		// If condition still not fulfilled (timeout but condition is 'false')
		console.log(this.name + "'waitFor()' timeout");
		//phantom.exit(1);
	},
	waitInterval : function(s)
	{
		if (s.onTestReady())
		{
			clearInterval(s._timer); //< Stop this interval
			if(s.onReady()) //return true, we are done
				s.onComplete();
			else
				s.waitFor();
		}
		else if (new Date().getTime() - s._timer_start > s._timer_timeout)
		{
			clearInterval(s._timer); //< Stop this interval
			s.onTimeout();
			s.onComplete();
		}
	},
	waitFor : function()
	{
		//console.log(this.name+" calling wait!");
		this._timer_start = new Date().getTime();
		this._timer = setInterval(this.waitInterval, 500, this); //< repeat check every 250ms
	},
	run : function()
	{
		var s = this;
		s.p.open(s._url, function(status)
		{
			var context = "context";
			if (status != "success")
			{
				var status = "unknown";
				if (s.p.resource) status = s.p.resource.status;
				console.log('Page did not load (status=' + status + '): ' + s._url);
				s.onComplete();
				return;
			}
			s.p.injectJs('inc/jquery.js');
			//s.p.injectJs('inc/race.js');
			
			s.p.evaluate(function() {
				
				window._ac$ = jQuery.noConflict(true); 
			    //window.$ = window.jQuery =  _pjs$;
			});
			//console.log("calling wait!");
			s.waitFor();
		});
	}
}