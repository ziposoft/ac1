var fs = require('fs'), system = require('system');
phantom.injectJs('inc/jquery.js');
phantom.injectJs('inc/zs.js');
phantom.injectJs('inc/zs_race.js');


function phantom_exit(code) {
    setTimeout(function(){ phantom.exit(code); }, 0);
}

var runners=zipo.csv_load("CGTC.csv",zipo.running.Runner);

console.log(runners.length)

console.log(JSON.stringify(runners[0]))


/*

console.log(JSON.stringify(urlSplit("http://localhost/test_site/raceresults.html")));
console.log(JSON.stringify(urlSplit("https://localhost")));
console.log(JSON.stringify(urlSplit("/test_site/raceresults.html")));
*/
phantom_exit(0);

