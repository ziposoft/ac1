phantom.injectJs('inc/godiva.js');
function TestRace(raceobj)
{
	this.race = raceobj;

	//this.race.output(gConsole);
	zipo.running.races.list.push(this.race);
	zipo.scrape.Page.call(this, raceobj.name, raceobj.url);
};
zipo.subClass(zipo.scrape.Page,TestRace);

TestRace.prototype.evalScrapeTest = function()
{
	if (_ac$('div#ajax1').length == 0)	
		return 0;
	
	if (_ac$('table.data').length)
	{
		// console.log("table.data ready");
		return 2;
	}
	// console.log("table.data not ready");
	return 1;
};
TestRace.prototype.evalScrape = function()
{
	result=new resultPage();
	
	//console.log("helloooo:"+_acs.status);
	if (_ac$('table.data').length == 0) 
		{
		result.status="error";
		return result;
		
		}
	result.data= _ac$('table.data').html();
	var next = _ac$("a:contains('NextPage')");
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

TestRace.prototype.processData = function(result)
{

	try
	{
		
	var data=jQuery(result.data); 
	var columns={};

	jQuery('th',data).each(function(i)
	{
		var t= $( this ).text();
		//console.log("column:"+t)
		if(t in zipo.running.columnAlias){
			columns[zipo.running.columnAlias[t]]=i;
		    
		}
	});
	//console.log(JSON.stringify(columns));
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
		
		var rr=new zipo.running.result();
		rr.create(columns,val);
		runners.process(race,rr);
	
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
function TestRaceList()
{
	zipo.scrape.Page.call(this, "Results List", 'http://localhost/test_site/raceresults.html');
};
zipo.subClass(zipo.scrape.Page,TestRaceList);


TestRaceList.prototype.evalScrape = function()
{
	result=new resultPage();
	try
	{
		
		if (_ac$('#data').length == 0) 
			result.status="no #data found";
		else
			{
			result.data= _ac$('table#data').html();
			result.status="done";
			
			}
		

	}
	catch (e)
	{
		console.log("TestRaceList.prototype.evalScrape error:"+e);
	}
	return result;
}
TestRaceList.prototype.evalScrapeTest = function()
{
	if (_ac$('div#ajax1').length == 0)	
		return 0;
	if (_ac$('#data').length > 0)
	{
		 console.log("results list ready");
		return 2;
	}
	return 1;
};
TestRaceList.prototype.processData = function(result)
{
	//var racelist = this.p.evaluate(scrape_list_of_results);
	var s=this;
	var data=jQuery(result.data);
	var races = [];
	jQuery('tr',data).each(function()
	{
		
		
		var l=jQuery("td", this);
		if(l.length<4)
			return;
		
		var h=l.eq(1);
		var href=jQuery("a", h).attr("href");			
		var r =  new zipo.running.Race();
		
		r.date = l.eq(0).text();
		//r.url = _ac.toFullUrl(href);
		r.url = s.makeFullUrl(href);
		r.name = h.text();
		r.city = l.eq(2).text();
		r.state = l.eq(3).text();
		races.push(r);			
	});
	jQuery.each(races, function(i, val)
	{
		zipo.scrape.que.add(new TestRace(val))
	});
	return true; //We are done
};
/*
 * RUN THAT BABY!
 * 
 * 
 */

zipo.scrape.que.add(new TestRaceList())
