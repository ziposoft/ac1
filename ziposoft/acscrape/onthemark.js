phantom.injectJs('inc/godiva.js');

function OnTheMarkRacePage(raceobj) {
    this.race = raceobj;

    var filename = raceobj.name.replace(/[^a-z0-9]/gi, '_').toLowerCase();
    csvFileName = "results/" + filename + ".csv";
    this.csvFile = fs.open(csvFileName, 'w');
    this.csvFileNameGodiva = "results/" + filename + "_godiva.txt";
    this.csvFileGodiva = false;
    this.processColHeaders = true;
    this.ourcolumns = {};
    //this.race.output(gConsole);
    zipo.running.races.list.push(this.race);
    zipo.scrape.Page.call(this, raceobj.name+"_PAGE", raceobj.url);
};
zipo.subClass(zipo.scrape.Page, OnTheMarkRacePage);
OnTheMarkRacePage.prototype.evalScrapeTest = function ()
{
    if(_ac$("a:contains('<< Back to Race List')").length==0)
        return "NOT A RESULT PAGE";
	
    if (_ac$('table.data').length) {
        // console.log("table.data ready");
        return "ready";//ready
    }
    return "wait";//not ready
};

OnTheMarkRacePage.prototype.evalScrape = function () {
    result = new resultPage();
    len = _ac$('#RR-360_results').length;
    if (len == 0) {
        result.status = "error";
        return result;

    }
    console.log("len=" + len);
    result.data = _ac$('div#tabs').html();
    console.log(result.data);
    result.status = "done";
    return result;
}

OnTheMarkRacePage.prototype.processData = function (result) {
    var data = jQuery(result.data);
    if (jQuery('a',data).length == 0) {
        console.log("no events found");
        zipo.scrape.que.add(new OnTheMarkRace(this.race));
        return true;

    }

   
    race = this.race;
    jQuery('a', data).each(function (i) {
        var t = $(this).text();
        var event = new zipo.running.Race();
        event.name = race.name + '-' + t;
        event.url = "http://onthemarksports.com" + $(this).attr("href");
        console.log(event.name);
        console.log(event.url);
        zipo.scrape.que.add(new OnTheMarkRace(event));
    });

    return true;
}
function OnTheMarkEvent(racename,eventname)
{
    var filename = racename.replace(/[^a-z0-9]/gi, '_').toLowerCase();
    if (eventname.length > 0) {
        var filename_event = eventname.replace(/[^a-z0-9]/gi, '_').toLowerCase();
        filename = filename + '(' + filename_event + ')';
    }
    csvFileName = "results/" + filename + ".csv";
	this.csvFile = fs.open(csvFileName, 'w');
	this.csvFileNameGodiva = "results/" + filename + "_godiva.txt";
	this.csvFileGodiva = false;
	this.processColHeaders = true;
	this.ourcolumns = {};
}
function OnTheMarkRace(raceobj)
{
	this.race = raceobj;
	

	this.events = {};
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
    var racename = _ac$("H1").text();

	result=new resultPage();
	var tabselected = _ac$(".ui-tabs-selected");
	if (tabselected.length == 1) {
	    console.log("current tab:" + tabselected.text());
	    result.event = tabselected.text();
	}
	else
	    result.event = "";
	if (_ac$('table.data').length == 0) 
		{
		result.status="error";
		return result;
		
		}
	result.data= _ac$('table.data').html();
	var next = _ac$("a:contains('>>')");
	var numrows = _ac$('table.data tr').length;
	if (numrows != 51) {
	    if(next.length)
	        console.log("ERROR! " + racename + "-" + result.event + " ASSUMING LAST PAGE. rows=" + numrows);
	}
    else
	if (next.length) {
	    console.log(racename + "-" + result.event + " : clicking next page");
	    _ac$('table.data').attr('class', 'done');
	    clickElement(next[0]);
	    result.status = "more";
	    return result;
	}
	var tab = _ac$(".ui-tabs-selected").next().children();
	if (tab.length != 1) {
	    result.status = "done";
	    return result;
	}
	console.log(racename + " clicking tab:" + tab.text());
	_ac$('table.data').attr('class', 'done');
	clickElement(tab[0]);
	result.status = "more";
		
	return result;
}

OnTheMarkRace.prototype.processData = function(result)
{
    var race = this;
    var eventname = result.event;
    var event;
    if (eventname in race.events) {
        event = race.events[eventname];
    }
    else {
        event = new OnTheMarkEvent(race.name, eventname);
        race.events[eventname] = event;
    }

    try {

        var data = jQuery(result.data);
        var rawcolumns = [];
        if (event.processColHeaders) {
            comma = false;
            jQuery('th', data).each(function (i) {
                var t = $(this).text().toLowerCase().trim();
                if (comma)
                    event.csvFile.write(',');
                event.csvFile.write('"'+t+'"');
                comma = true;
                //console.log("column:"+t)
                if (t in zipo.running.columnAlias) {
                    event.ourcolumns[zipo.running.columnAlias[t]] = i;

                }
                else {
                    zipo.dbgout(t + " is not a column!")
                    event.ourcolumns[t] = i;
                }
            });
            event.csvFile.write('\n');
            event.processColHeaders = false;
        }



        var arr = [];
        var arr = jQuery('tr', data).map(function () {
            comma = false;
            row = [jQuery('td', this).map(function () {
                var a = jQuery(this).text();
                t = a.trim();
                if (comma)
                    event.csvFile.write(',');
                event.csvFile.write('"' + t + '"');
                comma = true;
                return t;
            }).toArray()];
            event.csvFile.write('\n');
            return row;
        }).toArray();

        event.csvFile.flush();

        //console.log(JSON.stringify(arr));

        var race = this.race;
        //console.log(this.race.name  +" found " + results.length + " results")
        jQuery.each(arr, function (i, val) {

            var rr = new zipo.running.Result();
            rr.create(event.ourcolumns, val);
            
            match = zipo.running.runners.process(race, rr, eventname);
            if (match)
            {
                if (event.csvFileGodiva == false) {
                    event.csvFileGodiva = new zipo.File(event.csvFileNameGodiva);
                }
                match.output(event.csvFileGodiva);
                match.output(zipo.con);
            }

        });
        if (event.csvFileGodiva)
            event.csvFileGodiva.flush();
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
			    var r = new zipo.running.Race();
			    r.datestr = val[0];
			    r.date = new Date(r.datestr);
			    //console.log("(r.datestr=" +r.datestr)
			    r.url = val[1].split('"')[1];
			    name = val[1].split('>')[1].split('<')[0];

			    r.name = (r.date.getMonth() + 1) + '-' + r.date.getDate() + '-' + r.date.getFullYear() + '-' + name;
				r.city = val[2];
				r.state = val[3];
				if (
                   // (name.indexOf("Crystal Coast") > -1) 
                    // (name == "Raven Rock Trail Race")
				    (r.date.getFullYear() > 2014)
                    )
                    {
				    console.log(r.datestr + " " + r.name);
				    zipo.scrape.que.add(new OnTheMarkRace(r));
				}
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
