phantom.injectJs('acs.js');
function OnTheMarkRace(raceobj)
{
	this.race = raceobj;
	acs.call(this, raceobj.name, raceobj.url);
};
OnTheMarkRace.prototype = Object.create(acs.prototype); // See note below
OnTheMarkRace.prototype.constructor = acs;
OnTheMarkRace.prototype.evalTestReady = function()
{
	if ($('table.data').length)
	{
		// console.log("table.data ready");
		return true;
	}
	return false;
};
var scrape_race_results = function()
{
	if ($('table.data').length == 0) return null;
	var arr = [];
	var arr = $('table.data tr').map(function()
	{
		return [ $('td', this).map(function()
		{
			var a = $(this).text();
			return a.trim();
		}).toArray() ];
	}).toArray();
	return arr;
}
var scrape_next_page = function()
{
	var clickElement = function (el){
	    var ev = document.createEvent("MouseEvent");
	    ev.initMouseEvent(
	      "click",
	      true /* bubble */, true /* cancelable */,
	      window, null,
	      0, 0, 0, 0, /* coordinates */
	      false, false, false, false, /* modifier keys */
	      0 /*left*/, null
	    );
	    el.dispatchEvent(ev);
	};	
	var next=$( "a:contains('>>')" );
	if (next.length == 0) return false;
	//var url= $(next).attr('href');

	clickElement(next[0]);

	return true;

}
/*
 * 
 * http://onthemarksports.com/results/2014-jingle-jog-5k-results/#/results:&start_key=000001/OVERALL/0000000101
 * http://onthemarksports.com/results/2014-jingle-jog-5k-results/#/results::1421895512562
 * http://onthemarksports.com/?p=2400
 */

OnTheMarkRace.prototype.onReady = function()
{
	//console.log(this.race.name + " READY!");
	var results = this.p.evaluate(scrape_race_results);
	 console.log(JSON.stringify(results[2]));
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
	var race=this.race;
	console.log("found "+results.length+" results")
	jQuery.each(results, function(i, val)
	{
		if (val[3])
		{
			var names = val[3].split(',');
			var age= val[4];
			var r = runners.search(names[0], names[1].trim(),race.date,age)
			if (r)
			{
			
				var result=new raceResult(race,r,val[0],val[2],val[9]);
				gRaceResults.push(result);
			}
		}
	});
	var next = this.p.evaluate(scrape_next_page);
	if(!next)
		return true; //We are done
	

	return false;
	
};
/*
 * OnTheMarkResultList
 * 
 */
function OnTheMarkResultList()
{
	acs.call(this, "OnTheMarkSports Results List", 'http://onthemarksports.com/results/');
};
OnTheMarkResultList.prototype = Object.create(acs.prototype); // See note
// below
OnTheMarkResultList.prototype.constructor = acs;
/*
 * OnTheMarkResultList - Get the list of results
 * 
 */
var scrape_list_of_results = function()
{
	// console.log("fn_scrape")
	var races = [];
	try
	{
		var table = punchgs.com.greensock.TweenLite.selector.fn.dataTable.settings[0].aoData;
		console.log("table len=" + table.length)
		jQuery.each(table, function(i, val)
		{
			if (i < 1)
			{
				var r = {};
				r.date = val._aData[0];
				r.url = val._aData[1].split('"')[1];
				r.name = val._aData[1].split('>')[1].split('<')[0];
				r.city = val._aData[2];
				r.state = val._aData[3];
				// var name= val._aData[1].split('>')[1].split('<')[0];
				// var r=new
				// race(name,val._aData[0],url,val._aData[2],val._aData[3]);
				// console.log(name);
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
	if ($('#tablepress-5').length > 0)
	{
		// console.log("results list ready");
		return true;
	}
	return false;
};
OnTheMarkResultList.prototype.onReady = function()
{
	// console.log("READY!");
	var racelist = this.p.evaluate(scrape_list_of_results);
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
	

}


runners.loadfile("CGTC.csv");
acsQue.add(new OnTheMarkResultList())
