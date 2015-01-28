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
		
	var data=jQuery(result.data); 
	var columns={};

	jQuery('th',data).each(function(i)
	{
		var t= $( this ).text();
		console.log("column:"+t)
		if(t in acsColumns){
			columns[acsColumns[t]]=i;
		    
		}
	});
	console.log(JSON.stringify(columns));
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
		var firstname="";
		var lastname="";
		if('nameFull' in columns)
		{
			var names = val[columns.nameFull].split(',');
			lastname=names[0];
			firstname=names[1].trim();
		}
			
		if (firstname && lastname)
		{
			
			var age = val[4];
			runners.process(
					race,lastname,
					firstname,  
					val[columns.age],
					val[9],
					val[0],
					val[1],
					val[2])

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
		console.log(e);
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
	// console.log("READY!");
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

		var r = {};
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
