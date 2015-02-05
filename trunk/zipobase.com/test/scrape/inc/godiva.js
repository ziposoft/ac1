var fs = require('fs'), system = require('system');
phantom.injectJs('inc/jquery.js');

function phantom_exit(code) {
    setTimeout(function(){ phantom.exit(code); }, 0);
}
phantom.injectJs('inc/zs.js');
phantom.injectJs('inc/zs_scrape.js');
phantom.injectJs('inc/zs_race.js');

if (system.args.length === 1)
{
	console.log('no args');
}
else
{
	system.args.forEach(function(arg, i)
	{
		if(arg=="refresh")
			zipo.scrape.cacheuse=false;
		if(arg=="noscrape")
			zipo.scrape.runscrape=false;
			
		console.log(i + ': ' + arg);
	});
}

zipo.running.runners.loadfile("CGTC.csv");
zipo.scrape.que.onComplete = function()
{
	zipo.running.races.output(zipo.con)

	
	var f=new zipo.File("output.txt")
	console.log("DONE! Outputing...");
	zipo.running.races.output(f)
	f.close();
	phantom_exit();
}
