var gDebugLevel=1;

function debugOut(s)
{
	if(gDebugLevel>0)
		console.log(s);
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

var resultEvent = function()
{
	this.pages = [];

	
};

var resultUrl = function()
{
	this.data = {
			events:[]
	};
	this.status="more"; // fail, more,done
	
};