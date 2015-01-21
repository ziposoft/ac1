var fs = require('fs'),
    system = require('system');
phantom.injectJs('acutil.js');
phantom.injectJs('client/jquery.js');
phantom.injectJs('runners.js');

if (system.args.length === 1) {
    console.log('no args');
} else {
    system.args.forEach(function (arg, i) {
            console.log(i + ': ' + arg);
    });
}

/*
 

* 
* 
* acs constructor
 */
var acs = function(name,url) {
	this.name=name;
	this._url=url;
	this.p = require('webpage').create();
	this.p.onConsoleMessage = function(msg) {
	    console.log(msg);
	};	
	this.p.onCallback = function(msg) {
	    console.log(":"+msg);
	    
	};	
	
	
	
	this._timer;
	this._timer_start;
	this._timer_timeout=30000;
	this._timer_interval=250;
	this._context="context";
	
	this.eval
};

acs.prototype = {
	evalTestReady : function ()
	{
		return true;
	},
	evalScrape : function ()
	{
		
	},	
	onComplete : function ()
	{
		this.p.close();
		console.log(this.name + " completed!");		
		
	},		
	onTestReady : function ()
	{
		console.log(this.name +" loading...");
		return this.p.evaluate(this.evalTestReady,this._context);
	},		
	onReady : function ()
	{
		console.log("'waitFor()' finished in "
				+ (new Date().getTime() - this._timer_start) + "ms.");		
	},
	onTimeout : function ()
	{
		// If condition still not fulfilled (timeout but condition is 'false')
		console.log(this.name + "'waitFor()' timeout");
		//phantom.exit(1);
	},	
	waitInterval : function(s) {
		
		if(s.onTestReady())
		{
			clearInterval(s._timer); //< Stop this interval
			s.onReady();
			s.onComplete();
		}
		else
		if (new Date().getTime() - s._timer_start > s._timer_timeout)
			{
			clearInterval(s._timer); //< Stop this interval
			s.onTimeout();
			s.onComplete();
			}
	},
	waitFor : function() {
		this._timer_start = new Date().getTime();
		
		this._timer = setInterval(this.waitInterval, 500,this); //< repeat check every 250ms
	},
	run : function() 
	{
		var s=this;
		s.p.open(s._url, function(status) 
		{
			var context="context";
			
			if (status != "success") 
			{
				var status="unknown";
				if(s.p.resource)
					status = s.p.resource.status;
					console.log('Page did not load (status=' + status + '): ' + url);
					s.onComplete();
			        return;
			    }
			
				s.p.injectJs('client/jquery.js');
				/*
				s.p.evaluate(function() {
					console.log(window.title);
					window._pjs$ = jQuery.noConflict(true); 
				    window.$ = window.jQuery =  _pjs$;
				});*/
			//console.log("calling wait!");
			s.waitFor(); 	
		});	
	}
}