phantom.injectJs('inc/godiva.js');

/*
 * OnTheMarkResultList
 * 
 */
function TestRaceList()
{
	zipo.scrape.Page.call(this, "cnn", 'http://www.cnn.com');
};
zipo.subClass(zipo.scrape.Page,TestRaceList);


TestRaceList.prototype.evalScrape = function()
{
	
	var t=_ac$('strong').text();
	console.log(t);
	result=new resultPage();
	result.status="done";

	return result;
}
TestRaceList.prototype.evalScrapeTest = function()
{
	if (_ac$('strong').length == 0)	
		{
		
		console.log("waiting");
		return "wait";
		}
	return "ready";
};
TestRaceList.prototype.processData = function(result)
{

	return true; //We are done
};
/*
 * RUN THAT BABY!
 * 
 * 
 */

zipo.scrape.que.add(new TestRaceList())
