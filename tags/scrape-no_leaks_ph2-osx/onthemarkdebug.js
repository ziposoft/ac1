phantom.injectJs('inc/acs.js');
function OnTheMarkRace(raceobj)
{
	this.race = new acsRace();
	acsCopyObj(raceobj, this.race);
	//this.race.output(gConsole);
	gRaceResults.races.push(this.race);
	acs.call(this, raceobj.name, raceobj.url);
};
subClass(acs,OnTheMarkRace);

OnTheMarkRace.prototype.evalScrapeTest = function()
{
	return true;	

};

OnTheMarkRace.prototype.evalScrape = function()
{
	return null;

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
OnTheMarkRace.prototype.processData = function()
{
	//console.log(this.race.name + " READY!");
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
	
	//var next = this.p.evaluate(scrape_next_page);
	//if (!next) return true; //We are done
	return false;
};
/*
 * OnTheMarkResultList
 * 
 */
function OnTheMarkResultList()
{
	acs.call(this, "OnTheMarkSports Results List", 'http://onthemarksports.com/results/');
	this.useJquery=true;
};
subClass(acs,OnTheMarkResultList);

/*
 * OnTheMarkResultList - Get the list of results
 * 
 */
OnTheMarkResultList.prototype.evalScrape = function()
{
	// console.log("fn_scrape")
	var races = [];
	try
	{
		var table = punchgs.com.greensock.TweenLite.selector.fn.dataTable.settings[0].aoData;
		console.log("table len=" + table.length)
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
	catch (e)
	{
		console.log(e);
	}
	return null;
}
OnTheMarkResultList.prototype.evalTestReady = function()
{

	if (_ac$('#tablepress-5').length > 0)
	{
		// console.log("results list ready");
		return true;
	}
	return false;
};
OnTheMarkResultList.prototype.processData = function()
{
	acsQue.add(new OnTheMarkResultList());
	return false;
	// console.log("READY!");
	jQuery.each(this.data, function(i, val)
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
}
runners.loadfile("CGTC.csv");
acsQue.add(new OnTheMarkResultList())
