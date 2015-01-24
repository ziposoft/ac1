phantom.injectJs('acs.js');


runners.loadfile("CGTC.csv");
var name="Corriveau";
var r=runners.find(name)
if(r)
	console.log(JSON.stringify(r))
else
	console.log(name+" not found")
phantom_exit();