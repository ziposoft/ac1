phantom.injectJs('acs.js');


function OnTheMarkRace(raceobj)
{
	this.race = raceobj;
	acs.call(this, raceobj.name,raceobj.url);
};
OnTheMarkRace.prototype = Object.create(acs.prototype); // See note below
OnTheMarkRace.prototype.constructor = acs;
OnTheMarkRace.prototype.evalTestReady = function()
{
	if($('table.data').length)
	{
	console.log("table.data ready");
	return true;
	}
	return false;
};
var scrape_race_results = function()
{
	if($('table.data').length==0)
		return null;
	var arr = [];
	
	 var arr= $('table.data tr').map(function() { 
         return [$('td', this).map(function() {
        	 var a=$(this).text();
             return a.trim(); 
         }).toArray()]; 
     }).toArray();
    
	return arr;	

}
OnTheMarkRace.prototype.onReady = function()
{
	console.log(this.race.name + " READY!");
	var results = this.p.evaluate(scrape_race_results);
	console.log(JSON.stringify(results[0]));
	jQuery.each(results, function(i, val)
	{
		
	});
	
};
/*
 * OnTheMarkResultList
 * 
 */
function OnTheMarkResultList()
{
	acs.call(this, "OnTheMarkSports Results List",'http://onthemarksports.com/results/');
};
OnTheMarkResultList.prototype = Object.create(acs.prototype); // See note below
OnTheMarkResultList.prototype.constructor = acs;
/*
 * OnTheMarkResultList  -  Get the list of results
 * 
 */
var scrape_list_of_results = function()
{
	console.log("fn_scrape")
	var races = [];
	try
	{
		var table = punchgs.com.greensock.TweenLite.selector.fn.dataTable.settings[0].aoData;
		console.log("table len="+table.length)
		jQuery.each(table, function(i, val)
		{
			if (i < 20)
			{
				var r={};
				r.date=val._aData[0];
				r.url= val._aData[1].split('"')[1];
				r.name= val._aData[1].split('>')[1].split('<')[0];
				r.city=val._aData[2];
				r.state=val._aData[3];
				//var name= val._aData[1].split('>')[1].split('<')[0];
				//var r=new race(name,val._aData[0],url,val._aData[2],val._aData[3]);
				//console.log(name);
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
		console.log("results list ready");
		return true;
	}
	return false;
};

OnTheMarkResultList.prototype.onReady = function()
{
	console.log("READY!");
	var racelist = this.p.evaluate(scrape_list_of_results);
	jQuery.each(racelist, function(i, val)
	{
		var s = new OnTheMarkRace(val);
		s.run();
	});
	console.log("Falling through?");
};
/*
 * RUN THAT BABY!
 * 
 * 
 */
var scraper = new OnTheMarkResultList();
scraper.run();
