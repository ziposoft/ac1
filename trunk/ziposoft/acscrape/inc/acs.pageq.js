var fs = require('fs'), system = require('system'), phantomWebPage = require('webpage');
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

const gMaxPages = 4;
var acsQue =
{
	processed : 0,
	maxrun : gMaxPages,
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
		while(this.mapDone.length)
		{
			var next=this.mapDone.pop();
			console.log('Deleting '+next.name);
			
			delete next;

		}			
		var s = popByName(this.mapRunning,scraper.name);
		if (!s)
		{	
			console.log('error '+s.name+'not running');
			return;
		}
		this.mapDone.push(s);
		
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
var acsWebPage = function(index)
{
	this.index=index;
	this.p = phantomWebPage.create();
	this.p.onConsoleMessage = function(msg)
	{
		console.log(msg);
	};
	this.p.onCallback = function(msg)
	{
		console.log(":" + msg);
	};
	this.p.onResourceTimeout = function(request) {
	    console.log('Response (#' + request.id + '): ' + JSON.stringify(request));
	};	
	this.free=true;


};
acsWebPage.prototype =
{
	close : function()
	{
		//this.p.close();
		this.free=true;
		gAcsPages.free(this);
		
	}
}
var gAcsPages = 
{
	list : [],
	init : function()
	{
		var i;
		for(i=0;i<gMaxPages;i++)
		{
			this.list.push(new acsWebPage(i));
		}
	},
	get : function()
	{
		var i;
		for(i=0;i<gMaxPages;i++)
		{
			if(this.list[i].free)
			{
				this.list[i].free=false;
				return this.list[i];
			}
		}
		console.log('ERROR cannot get PAGE!');
	},
	free : function(p)
	{
		p.free=true;
	},	
};



gAcsPages.init();
/*


 * 
 * 
 * acs constructor
 */
var acs = function(name, url)
{
	this.name = name;
	this._url = url;
	

	this._timer;
	this._timer_start;
	this._timer_timeout = 6000;
	this._timer_interval = 250;
	this._context = "context";

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

		this.acp.close();
		
		
		//console.log(this.name + " completed!");
		acsQue.done(this);
	},
	onTestReady : function()
	{
		//console.log(this.name + " loading...");
		return this.acp.p.evaluate(this.evalTestReady, this._context);
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
		console.log(s.name+" waitInterval");
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
		console.log(this.name+" calling wait!");
		this._timer_start = new Date().getTime();
		this._timer = setInterval(this.waitInterval, 500, this); //< repeat check every 250ms
	},
	run : function()
	{
		var s = this;
		s.acp = gAcsPages.get();
		console.log("calling "+s._url+"  opened!");
		s.acp.p.open(s._url, function(status)
		{
			console.log(s._url+"  opened!");
			var context = "context";
			if (status != "success")
			{
				var status = "unknown";
				if (s.acp.p.resource) status = s.acp.p.resource.status;
				console.log('Page did not load (status=' + status + '): ' + s._url);
				s.onComplete();
				return;
			}
			s.acp.p.injectJs('inc/jquery.js');
			//s.p.injectJs('inc/race.js');
			
			s.acp.p.evaluate(function() {
				
				window._ac$ = jQuery.noConflict(true); 
			    //window.$ = window.jQuery =  _pjs$;
			});
			//console.log("calling wait!");
			s.waitFor();
		});
	}
}