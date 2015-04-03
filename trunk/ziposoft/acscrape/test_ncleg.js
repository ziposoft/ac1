phantom.injectJs('inc/ncleg.js');

var members=[];
zipo.scrape.que.onComplete = function()
{
	console.log("saving results..");
	zipo.csv_save(members,"members.csv");

	phantom_exit();
}



function Legger(chamber,name,url)
{
	this.chamber=chamber;
	this.member_name=name;
	
	var uida=url.split('nUserID=');
	
	this.uid=uida[1];
	zipo.scrape.Page.call(this, chamber+"-"+name, url);
};
zipo.subClass(zipo.scrape.Page, Legger);
Legger.prototype.evalScrape = function()
{
	result = new resultPage();
	try
	{
		if (_ac$('.memberTabBar').length == 0)
			result.status = "no #data found";
		else
		{
			result.data = _ac$('.memberTabBar').html();
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
	var data = jQuery(result.data);
	var member={};
	member.email = $("a[href^='mailto:']",data).text();
	var p=member.email.split('@')[0];
	var aid=p.split('.');
	member.key=aid[1]+'.'+aid[0];
	member.uid=this.uid;
	member.name=this.member_name;
	member.chamber=this.chamber;
	members.push(member);
	
	return true; // We are done
};
// LegList.prototype.evalScrapeTest = function() { return "ready";};
function LegList(chamber,url)
{
	this.chamber=chamber;
	zipo.scrape.Page.call(this, "LegList-"+chamber, url);
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
		var count=2;
		console.log("count:" + count);
	
		
		jQuery("a[href^='/gascripts/members/viewMember.pl']", data).each(function()
		{
			if(count==0)
				return;
			count--;
				
			var t = $(this).text();
			t=t.replace(/\u00a0/g, " ");
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
zipo.scrape.que.add(new LegList('Senate','http://www.ncleg.net/gascripts/members/memberListNoPic.pl?sChamber=Senate'))





// zipo.scrape.que.add(new
// Legger('http://www.ncleg.net/gascripts/members/viewMember.pl?sChamber=senate&nUserID=392'))
