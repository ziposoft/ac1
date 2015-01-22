
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
Object.prototype.removeItem = function (key) 
{
	   if (!this.hasOwnProperty(key))
	      return
	   if (isNaN(parseInt(key)) || !(this instanceof Array))
	      delete this[key]
	   else
	      this.splice(key, 1)
};



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