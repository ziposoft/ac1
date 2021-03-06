zipo.scrape = (function(z)
{
	var scrape=this;
	scrape.appname="default";

	scrape.cacheuse=true;
	scrape.runscrape=true;
	
	
	

	scrape.que =
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
				z.trace(3,'Queueing ' + s.name);
			}
		},
		done : function(scraper)
		{
			
			var s = z.popByName(this.mapRunning, scraper.name);
			if (!s)
			{
				z.errormsg('error ' + scraper.name + 'not running');
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
				z.trace(1,'Processed ' + this.processed);
				z.trace(1,'Queue empty, exiting');
				this.onComplete();
				// phantom_exit(0);
			}
			z.trace(3,scraper.name + " done. Que=" + this.mapPending.length + " Running=" + this.mapRunning.length)
			
		},
		onComplete : function()
		{
			console.log('default scrape.que.onComplete function');
			phantom_exit();
		}
	};	
	
	
	
	
	
	scrape.Page = function(name, url)
	{
		this.tracelvl=2;
		this.data = [];
		this.name = name;
		this.url = url;
		this.urlparts = z.urlSplit(url);
		this.useJquery = true;
		this.cacheLoad = scrape.cacheuse;
		this.cacheSave = true;
		this.runScrape = scrape.runscrape;
		this.p = null;
		this.timer;
		this.timer_start;
		this.timer_timeout = 20000;
		this.timer_interval = 500;
		this.context = {};
		this.cacheDir= "cache/"+ scrape.appname + "/"+ this.name;
		this.cacheFileName = this.cacheDir + "/cache.txt";
		this.tempFileName =this.cacheDir  + "/temp.txt";
	};
	scrape.Page.prototype =
	{
		trace : function(lvl,s)
		{
			if(lvl<=this.tracelvl)
				console.log(this.name + " " + s);
		},
		makeFullUrl : function(url)
		{
			s = z.urlSplit(url);
			if (s.host) return url;
			var full;
			if(url.charAt(0) == '/')
				{
				
				
				 full = this.urlparts.protocol + "://" + this.urlparts.host  + url;
				
				}
			else
				
			 full = this.urlparts.protocol + "://" + this.urlparts.host + this.urlparts.path + '/' + url;
			return full;
		},
		evalScrapeTest : function()
		{
			return "ready";
		},
		evalScrape : function()
		{
			return {};
		},
		start : function()
		{
			var cd = this.cacheDir ;
			this.trace(3,"START");
			if (this.cacheLoad) if (fs.isDirectory(cd))
			{
				if (fs.isFile(this.cacheFileName))
				{
					var r = fs.read(this.cacheFileName);
					if (r)
					{
						try
						{
							this.data = JSON.parse(r);
							if (this.data)
							{
								this.trace(2,"found cached data")
								this.cacheSave = false;
								this._processData();
							}
							return;
						}
						catch (e)
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
			if(this.runScrape)
				this.scrapeStart();
			else
				this.onScapeError("NO DATA")
		},
		onScapeComplete : function()
		{
			this.trace(1,"scrape success!");
			// console.log(JSON.stringify(this.data));
			this.scrapeEnd()
			fs.write(this.cacheFileName, JSON.stringify(this.data, null, '\t'));
			this._processData();
		},
		onScapeError : function(error)
		{
			this.trace(0,"error:" + error);
			this.scrapeEnd()
			scrape.que.done(this);
		},
		onScrapeTimeout : function()
		{
			this.trace(0,"Timeout");
			this.scrapeEnd()
			scrape.que.done(this);
		},
		processData : function(data)
		{
			this.trace(3,"onProcessData");
		},
		onScrape : function()
		{
			this.trace(3,"onScrape");
			result = this.p.evaluate(this.evalScrape);
			this.data.push(result);
			if (this.cacheSave)
			{
				this.trace(3,"saving temp data to cache");
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
			this.trace(3,"processData");
			for (var i = 0; i < this.data.length; i++)
				this.processData(this.data[i]);
			scrape.que.done(this);
			
		},
		scrapeEnd : function()
		{
			this.trace(3,"scrapeEnd");
			if (this.p)
			{
				this.p.close();
				delete this.p;
				this.p = null;
			}
		},
		scrapeTimerCallback : function(s)
		{
			s.trace(4,"TimerCallback");
			r = s.onScrapeTest();
			if (r == "wait")
			{
				if (new Date().getTime() - s.timer_start < s.timer_timeout) return;
			}
			clearInterval(s.timer); // < Stop this interval
			if (r == "wait")
				return s.onScrapeTimeout();
			
			if (r == "ready")
			{
				result = s.onScrape();
				if (result.status == 'more') return s.scrapeStartTimer();
				if (result.status == 'done') return s.onScapeComplete();
				return s.onScapeError('Error on scrape:' + result.status);
			}
			s.onScapeError(r);
		},
		scrapeStartTimer : function()
		{
			this.trace(4," scrapeStartTimer");
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
			s.trace(3," scrapeStart");
			s.p = require('webpage').create();
			s.p.onConsoleMessage = function(msg)
			{
				console.log(msg);
			};
			s.p.onCallback = function(msg)
			{
				console.log(":" + msg);
			};
			s.p.onResourceTimeout = function(request)
			{
				console.log('Response (#' + request.id + '): ' + JSON.stringify(request));
			};
			s.trace(3,"calling upen...");
			s.p.open(s.url, function(status)
			{
				var context = "context";
				s.trace(3," opened: " + s.url)
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
				s.trace(3," opened: " + s.url+"success")
				s.scrapeStartTimer();
			});
		}
	};
	return scrape;
}(zipo));
