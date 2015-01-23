var gDebugLevel=1;

function debugOut(s)
{
	if(gDebugLevel>0)
		console.log(s);
}

function subClass(base,subclass)
{
	subclass.prototype = Object.create(base.prototype); 
	subclass.prototype.constructor = base;	
}
/*
 * 
 *  class acsFileBase
 */
var acsFileBase = function()
{
}
acsFileBase.prototype = 
{
	out : function(s)
	{
		
		console.log(s);
	}
}
var gConsole= new acsFileBase();

var acsFile = function(name)
{
	this.handle= fs.open(name, "w");
	acsFileBase.call(this);
};
subClass(acsFileBase,acsFile);
acsFile.prototype.out=function(s)
{
	this.handle.writeLine(s);
}
acsFile.prototype.close=function()
{
	this.handle.flush();
	this.handle.close();
}

/*
 * 
 *  class acsFileBase
 */


function clone(obj) {
    if(obj == null || typeof(obj) != 'object')
        return obj;

    var temp = obj.constructor(); // changed

    for(var key in obj) {
        if(obj.hasOwnProperty(key)) {
            temp[key] = clone(obj[key]);
        }
    }
    return temp;
}

function copyObj(fromObj,toObj) {
    if(fromObj == null || typeof(fromObj) != 'object')
        return ;

  
    for(var key in fromObj) {
        if(fromObj.hasOwnProperty(key)) {
        	toObj[key] = clone(fromObj[key]);
        }
    }
    return ;
}
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