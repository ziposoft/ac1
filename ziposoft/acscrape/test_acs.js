phantom.injectJs('inc/acs.js');
function TestRace(raceobj)
{
	this.race = new acsRace();
	copyObj(raceobj, this.race);
	//this.race.output(gConsole);
	gRaceResults.races.push(this.race);
	acs.call(this, raceobj.name, raceobj.url);
};
subClass(acs,TestRace);

TestRace.prototype.evalTestReady = function()
{
	if (_ac$('table.data').length)
	{
		// console.log("table.data ready");
		return true;
	}
	return false;
};
function scrape_race_results()
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
TestRace.prototype.onReady = function()
{
	//console.log(this.race.name + " READY!");
	var results = this.p.evaluate(scrape_race_results);
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
	jQuery.each(results, function(i, val)
	{
		if (val[3])
		{
			var names = val[3].split(',');
			var age = val[4];
			runners.process(race,names[0],
					names[1].trim(), age,val[9],val[0],val[1],val[2])

		}
	});
	var next = this.p.evaluate(scrape_next_page);
	if (!next) return true; //We are done
	return false;
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

/*
 * OnTheMarkResultList - Get the list of results
 * 
 */

function process_result()
{
		_ac$.each(table, function(i, val)
		{
			if (i < 1000)
			{
				var r = {};
				r.date = val._aData[0];
				r.url = val._aData[1].split('"')[1];
				r.name = val._aData[1].split('>')[1].split('<')[0];
				r.city = val._aData[2];
				r.state = val._aData[3];
				races.push(r);
			}
		});
		return races;
}
TestRaceList.prototype.evalScrape = function()
{
	// console.log("fn_scrape")
	var races = [];
	try
	{
		
		if (_ac$('table.data').length == 0) 
			return null;
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
	jQuery.each(racelist, function(i, val)
	{
		acsQue.add(new OnTheMarkRace(val))
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
	var f=new acsFile("output.txt")
	console.log("DONE! Outputing...");
	gRaceResults.output(f)
	f.close();
	phantom_exit();
}
runners.loadfile("CGTC.csv");
acsQue.add(new TestRaceList())
