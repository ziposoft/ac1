var fileBase = function()
{

}
fileBase.prototype = 
{
	out : function(s)
	{
		console.log(s);
	}
}
var gConsole= new fileBase();


function isVar(vartype)
{
return !( vartype === 'undefined');
};
function phantom_exit(code) {
    setTimeout(function(){ phantom.exit(code); }, 0);
    phantom.onError = function(){};
    throw new Error('');
}
function calcAge(dateString,bday) {
	  var birthday = +new Date(bday);
	  var date= new Date(dateString);
	  return ~~(( date- birthday) / (31557600000));
	}

function popByName(arr,name)
{
	var i;
	for (i = 0; i < arr.length; i++)
	{
		if (arr[i].name == name)
		{
			var o=arr[i];
			arr.splice(i, 1);
			return o;
		}
	}
	return null;
}



String.prototype.capitalize = function() {
     this.charAt(0).toUpperCase() + this.slice(1).toLowerCase();
}
function capitalize(s)
{
    return s && s[0].toUpperCase() + s.slice(1).toLowerCase();
}

var clickElement = function (el){
    var ev = document.createEvent("MouseEvent");
    ev.initMouseEvent(
      "click",
      true /* bubble */, true /* cancelable */,
      window, null,
      0, 0, 0, 0, /* coordinates */
      false, false, false, false, /* modifier keys */
      0 /*left*/, null
    );
    el.dispatchEvent(ev);
};

function waitFor(testFx, onReady, timeOutMillis) {
    var maxtimeOutMillis = timeOutMillis ? timeOutMillis : 3000, //< Default Max Timout is 3s
        start = new Date().getTime(),
        condition = false,
        interval = setInterval(function() {
            if ( (new Date().getTime() - start < maxtimeOutMillis) && !condition ) {
                // If not time-out yet and condition not yet fulfilled
                condition = (typeof(testFx) === "string" ? eval(testFx) : testFx()); //< defensive code
            } else {
                if(!condition) {
                    // If condition still not fulfilled (timeout but condition is 'false')
                    console.log("'waitFor()' timeout");
                    phantom.exit(1);
                } else {
                    // Condition fulfilled (timeout and/or condition is 'true')
                    console.log("'waitFor()' finished in " + (new Date().getTime() - start) + "ms.");
                    typeof(onReady) === "string" ? eval(onReady) : onReady(); //< Do what it's supposed to do once the condition is fulfilled
                    clearInterval(interval); //< Stop this interval
                }
            }
        }, 250); //< repeat check every 250ms
};
