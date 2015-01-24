phantom.injectJs('inc/acs.js');
function TestRace(raceobj)
{
	this.race = new acsRace();
	acsCopyObj(raceobj, this.race);
	//this.race.output(gConsole);
	gRaceResults.races.push(this.race);
	acs.call(this, raceobj.name, raceobj.url);
};
subClass(acs,TestRace);

TestRace.prototype.evalScrapeTest = function()
{
	if (_ac$('table.data').length)
	{
		// console.log("table.data ready");
		return true;
	}
	return false;
};
TestRace.prototype.evalScrape = function()
{
	if (_ac$('table.data').length == 0) return null;
	var arr = [];
	var arr = _ac$('table.data tr').map(function()
	{
		return [ _ac$('td', this).map(function()
		{
			var a = _ac$(this).text();
			return a.trim();
		}).toArray() ];
	}).toArray();
	return arr;
}
function scrape_next_page()
{
	var clickElement = function(el)
	{
		var ev = document.createEvent("MouseEvent");
		ev.initMouseEvent("click", true /* bubble */, true /* cancelable */, window, null, 0, 0, 0, 0, /* coordinates */
		false, false, false, false, /* modifier keys */
		0 /*left*/, null);
		el.dispatchEvent(ev);
	};
	var next = _ac$("a:contains('>>')");
	if (next.length == 0) return false;
	//var url= $(next).attr('href');
	clickElement(next[0]);
	return true;
}
TestRace.prototype.processData = function()
{

	try
	{
	//console.log(JSON.stringify(results[2]));
	/*
	 0= overall place
	 1= Division
	 2= Div Place
	 3= name 
	 4= age
	 5=city
	 6=st
	 7=gender
	 8=bib#
	 9=time
	  
	 
	 */
	var race = this.race;
	//console.log(this.race.name  +" found " + results.length + " results")
	jQuery.each(this.data, function(i, val)
	{
		if (val[3])
		{
			var names = val[3].split(',');
			var age = val[4];
			runners.process(race,names[0],
					names[1].trim(), age,val[9],val[0],val[1],val[2])

		}
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
	acs.call(this, "Results List", 'http://localhost/test_site/raceresults.html');
};
subClass(acs,TestRaceList);


TestRaceList.prototype.evalScrape = function()
{
	 console.log("fn_scrape")
	try
	{
		
		if (_ac$('#data').length == 0) 
			return null;
		var s= _ac$('table#data').html();
		
		
		return s;
		var races = [];
		
		//var table = punchgs.com.greensock.TweenLite.selector.fn.dataTable.settings[0].aoData;
		//console.log("table len=" + table.length)

	}
	catch (e)
	{
		console.log(e);
	}
	return null;
}
TestRaceList.prototype.evalScrapeTest = function()
{
	if (_ac$('#data').length > 0)
	{
		 console.log("results list ready");
		return true;
	}
	return false;
};
TestRaceList.prototype.processData = function()
{
	// console.log("READY!");
	//var racelist = this.p.evaluate(scrape_list_of_results);
	var data=jQuery(this.data);
	var races = [];
	jQuery('tr',data).each(function()
	{
		
		
		var l=jQuery("td", this);
		if(l.length<4)
			return;
		
		var h=l.eq(1);
		var href=jQuery("a", h).attr("href");			

		var r = {};
		r.date = l.eq(0).text();
		//r.url = _ac.toFullUrl(href);
		r.url = href;
		r.name = h.text();
		r.city = l.eq(2).text();
		r.state = l.eq(3).text();
		races.push(r);			
	});

	
	acsQue.add(new TestRaceList());
	return true;
	jQuery.each(races, function(i, val)
	{
		acsQue.add(new TestRace(val))
	});
	return true; //We are done
};
/*
 * RUN THAT BABY!
 * 
 * 
 */
acsQue.onComplete = function()
{
	gRaceResults.output(gConsole)

	
	var f=new acsFile("output.txt")
	console.log("DONE! Outputing...");
	gRaceResults.output(f)
	f.close();
	phantom_exit();
}
runners.loadfile("CGTC.csv");
acsQue.add(new TestRaceList())
