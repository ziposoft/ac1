phantom.injectJs('inc/ncleg.js');
function Legger(chamber,name,url)
{
	this.chamber=chamber;
	this.member_name=name;
	
	zipo.scrape.Page.call(this, chamber+"-"+name, url);
};
zipo.subClass(zipo.scrape.Page, Legger);
Legger.prototype.evalScrape = function()
{
	result = new resultPage();
	try
	{
		if (_ac$('#titleMemberBio').length == 0)
			result.status = "no #data found";
		else
		{
			result.data = _ac$('div#titleMemberBio').text();
			result.status = "done";
		}
	}
	catch (e)
	{
		console.log("Legger.prototype.evalScrape error:" + e);
	}
	return result;
}
Legger.prototype.processData = function(result)
{
	// var racelist = this.p.evaluate(scrape_list_of_results);
	var s = this;
	var data = result.data;
	console.log(data)
	return true; // We are done
};
// LegList.prototype.evalScrapeTest = function() { return "ready";};
function LegList(chamber,url)
{
	this.chamber=chamber;
	zipo.scrape.Page.call(this, "LegList", url);
};
zipo.subClass(zipo.scrape.Page, LegList);
LegList.prototype.evalScrape = function()
{
	result = new resultPage();
	try
	{
		var x = _ac$("a[href^='/gascripts/members/viewMember.pl']");
		if (x.length == 0)
			result.status = "no #data found";
		else
		{
			result.data = _ac$('#mainBody').html();
			result.status = "done";
		}
	}
	catch (e)
	{
		console.log("Legger.prototype.evalScrape error:" + e);
	}
	return result;
}
LegList.prototype.processData = function(result)
{
	// var racelist = this.p.evaluate(scrape_list_of_results);
	var s = this;
	try
	{
		var data = jQuery(result.data);
		
		
		
		jQuery("a[href^='/gascripts/members/viewMember.pl']", data).each(function()
		{
			var t = $(this).text();
			var h=$(this).attr("href");	
			var url=s.makeFullUrl(h);
			var l=new Legger( s.chamber,t,url);
			zipo.scrape.que.add(l);
		});
	}
	catch (e)
	{
		console.log("Legger.prototype.evalScrape error:" + e);
	}
	return true; // We are done
};
// $( "input[name^='news']" ).val( "news here!" );
/*
 * RUN THAT BABY!
 * 
 * 
 */
zipo.scrape.que.add(new LegList('House','http://www.ncleg.net/gascripts/members/memberListNoPic.pl?sChamber=House'))
// zipo.scrape.que.add(new
// Legger('http://www.ncleg.net/gascripts/members/viewMember.pl?sChamber=senate&nUserID=392'))
