
var race = function(name,date,url,city,state)
{
	this.name = name;
	this.date = date;
	this.url = url;	
	this.city = city;	
	this.state = state;	
}
race.prototype =
{

}
var runner = function(last, first, city, state,sex, dob, alias)
{
	this.first = first;
	this.last = last;
	this.state = state;
	this.city = city;
	this.sex = sex;
	this.dob = dob;
	this.alias = alias;
}
runner.prototype =
{
	match : function(last, first)
	{
		//console.log("runner: "+ JSON.stringify(this))
		last=last.toLowerCase();
		if (this.last != last) return false;
		if (first)
		{
			first=first.toLowerCase();
			if (this.first == first) return true;
		}
		return true;
	}
}
var runners =
{
	list : [],
	loadfile : function(filename)
	{
		var content = '', f = null, lines = null, eol = system.os.name == 'windows' ? "\r\n" : "\n";
		try
		{
			f = fs.open(filename, "r");
			if (f)
			{
				console.log("successfully opened "+filename);
			}
			content = f.read();
		}
		catch (e)
		{
			console.log(e);
		}
		if (f) f.close();
		if (content)
		{
			lines = content.split(eol);
			for (var i = 0, len = lines.length; i < len; i++)
			{
				var col = [];
				var line = lines[i].toLowerCase();
				col = line.split(",");
				
				col.forEach(function(val) { if(val) val.toLowerCase();  });
				
				var r = new runner(col[0], col[1], col[2], col[3], col[4], col[5], col[6]);
				this.list.push(r);
			}
			console.log("Found "+this.list.length + " runners");
		}
		else
			console.log("Error! Nothing found in: "+filename);
			
	},
	find : function(last, first)
	{
		for (var i = 0, len = this.list.length; i < len; i++)
		{
			var r = this.list[i];
			if (r.match(last, first)) return r;
		}
		return null;
	}
};
