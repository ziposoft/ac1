var fs = require('fs'), system = require('system');
String.prototype.capitalize = function()
{
	this.charAt(0).toUpperCase() + this.slice(1).toLowerCase();
}
var zipo = zipo || {};
var zipo = (function(z)
{
	/*
	 * debug 0=always 1=error 2=warning 3=
	 * 
	 * 
	 * 
	 */
	z.debug_level = 1;
	z.trace_lvl = 1;
	z.errormsg = function(s)
	{
		console.log(s);
	}
	z.dbgout = function(s)
	{
		if (z.debug_level > 0) console.log(s);
	}
	z.trace = function(lvl, s)
	{
		if (lvl <= z.trace_lvl) console.log(s);
	}
	// CLASSES
	z.subClass = function(base, subclass)
	{
		subclass.prototype = Object.create(base.prototype);
		subclass.prototype.constructor = base;
	}
	// FILES
	var FileBase = function()
	{
	}
	FileBase.prototype =
	{
		out : function(s)
		{
			console.log(s);
		},
		wr : function(s)
		{
			console.log(s);
		}
	}
	z.File = function(name)
	{
		this.handle = fs.open(name, "w");
		FileBase.call(this);
	};
	z.File.prototype.wr = function(s)
	{
		this.handle.write(s);
	}
	z.File.prototype.out = function(s)
	{
		this.handle.writeLine(s);
	}
	z.File.prototype.close = function()
	{
		this.handle.flush();
		this.handle.close();
	}
	z.con = new FileBase();
	z.csv_stringify=function(str)
	{
		if(typeof str === 'undefined')
			return "";
		if(str.search(',')!=-1)
		{
			str='"'+str+'"';
		}		
		return str;
		
	}
	z.csv_save = function(arr, filename)
	{
		try
		{
			var file;
			if (filename)
				file = new z.File(filename);
			else
				file = z.con;
			var comma = false;
			for ( var key in arr[0])
			{
				if (comma) file.wr(',');
				file.wr(z.csv_stringify(key));
				comma = true;
			}
			file.wr('\n');
			for (var i = 0; i < arr.length; i++)
			{
				comma = false;
				var obj = arr[i];
				for ( var key in obj)
				{
					if (comma) file.wr(',');
					file.wr(z.csv_stringify(obj[key]));
					comma = true;
				}
				file.wr('\n');
			}
			file.close();
		}
		catch (e)
		{
			console.log("csv_save failed:" + e)
		}
	}
	z.csv_load = function(filename, fn_obj)
	{
		var f = null;
		var output = [];
		try
		{
			var col = [];
			var line;
			var numcol = 0;
			f = fs.open(filename, "r");
			if (f)
			{
				console.log("successfully opened " + filename);
			}
			if (f.atEnd()) throw "File Empty";
			line = f.readLine();
			col = line.split(",");
			numcol = col.length;
			if (!numcol) throw "No Columns";
			while (!f.atEnd())
			{
				line = f.readLine();
				data = line.split(",");
				var obj = new fn_obj();
				for (var i = 0; i < numcol; i++)
					obj[col[i]] = data[i];
				obj.init();
				output.push(obj);
			}
		}
		catch (e)
		{
			console.log(e);
		}
		if (f) f.close();
		console.log("loaded " + output.length + " lines");
		return output;
	};
	// ARRAYS
	z.popByName = function(arr, name)
	{
		var i;
		for (i = 0; i < arr.length; i++)
		{
			if (arr[i].name == name)
			{
				var o = arr[i];
				arr.splice(i, 1);
				return o;
			}
		}
		return null;
	}
	// MISC
	z.calculateAge = function(dateString, bday)
	{
		var birthday = +new Date(bday);
		var date = new Date(dateString);
		return ~~((date - birthday) / (31557600000));
	}
	// STRINGS
	z.capitalize = function(s)
	{
		return s && s[0].toUpperCase() + s.slice(1).toLowerCase();
	}
	z.urlSplit = function(str)
	{
		var r =
		{
			protocol : "http",
			host : "",
			path : ""
		}
		var parts = str.split("://");
		if (parts.length == 2)
		{
			r.protocol = parts[0];
			str = parts[1];
			var i = str.indexOf('/');
			if (i == -1)
			{
				r.host = str;
				return r;
			}
			r.host = str.substring(0, i);
			var n = str.lastIndexOf('/');
			if (n == -1) n = str.length;
			r.path = str.substring(i, n);
		}
		else
			// It is relative URL
			r.path = str;
		return r;
	}
	return z;
}(zipo));
