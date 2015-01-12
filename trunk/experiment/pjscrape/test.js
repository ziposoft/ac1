phantom.injectJs("pjscrape.js");

var urlstart = 'http://onthemarksports.com/results/';

var contextobj = {
    config: {
            timeoutInterval: 1000,
            timeoutLimit: 20000,
            log: 'stdout',
            writer: 'stdout',
            format: 'json',
            logFile: 'pjscrape_log.txt',
            outFile: 'pjscrape_out.txt'
        },
     x : "hello" 
  };


pjs.config({
    format: 'csv',
    csvFields: ['a', 'b', 'c', 'd', 'e','a', 'b', 'c', 'd', 'e','a', 'b', 'c', 'd', 'e']
});

pjs.addSuite({
	context: contextobj,
	url: urlstart,
	maxDepth : 4,
	ignoreDuplicates : true,
	moreUrls : function() {
		var urls = [];
		try
		{
			var table = punchgs.com.greensock.TweenLite.selector.fn.dataTable.settings[0].aoData;
			jQuery.each( table, function( i, val ) {
				if(i<1)
					{
					var link = val._aData[1];
					var address=link.split('"');
					urls.push(address[1]);
					}
				
			});
			return urls;
			
			
		
		}
		catch(e) {}
		return null;
	},
	context: contextobj,
    scrapable: function(cont) {
    	//console.log(cont.x);
    	return true;
    },
	ready : function() {
		var url=window.location.href;
		if($('#tablepress-5').length)
			{
			console.log("results list ready");
			return true;
			}
		if($('table.data').length)
			{
			console.log("table.data ready");
			return true;
			}
		return false;
	},
	scraper : function() {
        _pjs.state.counter = _pjs.state.counter + 1 || 0;
        console.log("counter="+  _pjs.state.counter);
        
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
});



pjs.init();



console.log(contextobj.x);








