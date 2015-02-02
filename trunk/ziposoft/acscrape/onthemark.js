phantom.injectJs('inc/godiva.js');
function OnTheMarkRace(raceobj)
{
	this.race = raceobj;

	//this.race.output(gConsole);
	zipo.running.races.list.push(this.race);
	zipo.scrape.Page.call(this, raceobj.name, raceobj.url);
};

zipo.subClass(zipo.scrape.Page,OnTheMarkRace);
OnTheMarkRace.prototype.evalScrapeTest = function()
{
	if(_ac$("a:contains('<< Back to Race List')").length==0)
		return "NOT A RESULT PAGE";
	
	if (_ac$('table.data').length)
	{
		// console.log("table.data ready");
		return "ready";//ready
	}
	return "wait";//not ready
};

OnTheMarkRace.prototype.evalScrape = function()
{
	result=new resultPage();
	
	//console.log("helloooo:"+_acs.status);
	if (_ac$('table.data').length == 0) 
		{
		result.status="error";
		return result;
		
		}
	result.data= _ac$('table.data').html();
	var next = _ac$("a:contains('>>')");
	if (next.length ) 
		{
		_ac$('table.data').attr('class', 'done');
		clickElement(next[0]);
		result.status="more";
		}
	else
		result.status="done"
		
	return result;
}

OnTheMarkRace.prototype.processData = function(result)
{

	try
	{
		
	var data=jQuery(result.data); 
	var rawcolumns=[];
	var ourcolumns={};

	jQuery('th',data).each(function(i)
	{
		var t= $( this ).text().toLowerCase().trim();
		//console.log("column:"+t)
		if(t in zipo.running.columnAlias)
		{
			ourcolumns[zipo.running.columnAlias[t]]=i;
		    
		}
		else
			{
			zipo.dbgout(t+" is not a column!")
			
			}
	});
	//console.log(JSON.stringify(ourcolumns));
	var arr = [];
	var arr = jQuery('tr',data).map(function()
	{
		return [ jQuery('td', this).map(function()
		{
			var a = jQuery(this).text();
			return a.trim();
		}).toArray() ];
	}).toArray();
	

	var race = this.race;
	//console.log(this.race.name  +" found " + results.length + " results")
	jQuery.each(arr, function(i, val)
	{
		
		var rr=new zipo.running.Result();
		rr.create(ourcolumns,val);
		zipo.running.runners.process(race,rr);
	
	});
	}
	catch (e)
	{
		console.log(e);
	}	
	//var next = this.p.evaluate(scrape_next_page);
	//if (!next) return true; //We are done
	return true;
};
/*
 * OnTheMarkResultList
 * 
 */
function OnTheMarkResultList()
{
	zipo.scrape.Page.call(this, "OnTheMarkSports Results List", 'http://onthemarksports.com/results/');
};

zipo.subClass(zipo.scrape.Page,OnTheMarkResultList);

/*
 * OnTheMarkResultList - Get the list of results
 * 
 */
OnTheMarkResultList.prototype.evalScrape = function()
{
	result=new resultPage();
	try
	{
		var data= punchgs.com.greensock.TweenLite.selector.fn.dataTable.settings[0].aoData;
		
		if (data.length == 0) 
			result.status="no punchgs.com.greensock.TweenLite found";
		else
		{
			var arr=[];
			jQuery.each(data, function(i, val)
			{
				arr.push(val._aData);
			});
			
			result.data=JSON.stringify(arr);
			result.status="done";
			
			}
	}
	catch (e)
	{
		console.log(e);
	}
	return result;	

}
OnTheMarkResultList.prototype.evalScrapeTest = function()
{
	if (_ac$('#tablepress-5_wrapper').length > 0)
	{
		// console.log("results list ready");
		return "ready";//ready
	}
	return "wait";//not ready
};
OnTheMarkResultList.prototype.processData = function(result)
{
	var s=this;
	var data=JSON.parse(result.data);
	try
	{
		console.log("table len=" + data.length)
		jQuery.each(data, function(i, val)
		{
			if (i < 300)
			{
				var r =  new zipo.running.Race();
				r.date = val[0];
				r.url = val[1].split('"')[1];
				r.name = val[1].split('>')[1].split('<')[0];
				r.city = val[2];
				r.state = val[3];
				zipo.scrape.que.add(new OnTheMarkRace(r))
			}
		});
	}
	catch (e)
	{
		console.log(e);
		return false;
	}
	return true; //We are done
};
/*
 * RUN THAT BABY!
 * 
 * 
 */

zipo.scrape.que.add(new OnTheMarkResultList())
