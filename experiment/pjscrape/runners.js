var race = function(name, date, url, city, state)
{
	this.name = name;
	this.date = date;
	this.url = url;
	this.city = city;
	this.state = state;
}
race.prototype = 
{
		output : function(fnout)
		{
			fnout(this.name)
			fnout(this.city+", "+this.state+"    "+this.date)
			
			
		}

}
var runner = function(last, first, city, state, sex, dob, alias)
{
	this.first = capitalize(first);
	this.last = capitalize(last);
	this.state = capitalize(state);
	this.city = capitalize(city);
	this.sex = sex;
	this.dob = dob;
	this.alias = alias;
	//console.log(this.first+ " " +this.last)
}
runner.prototype =
{
	match : function(last, first)
	{
		// console.log("runner: "+ JSON.stringify(this))
		last = capitalize(last);
		if (this.last != last) return false;
		if (first)
		{
			first = capitalize(first);
			if (this.first == first) return true;
			return false;
		}
		return true;
	},
	getAge : function(date)
	{
		// console.log("runner: "+ JSON.stringify(this))
		return calcAge(date, this.dob);;
	}
}
var raceResult = function(race, runner,placeOverall,placeGroup,time)
{
	this.race = race;
	this.runner = runner;
	this.placeOverall=placeOverall;
	this.placeGroup=placeGroup;
	this.placeOverall=placeOverall;
	this.time=time;
}
var gRaceResults=
{
	byrace : {results : []},
	results : [],
	races : [],
	add : function(race, runner,placeOverall,placeGroup,time)
	{
		byrace[race.name].results.push()
		
		
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
				console.log("successfully opened " + filename);
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
				col.forEach(function(val)
				{
					if (val) val.capitalize();
				});
				var r = new runner(col[0], col[1], col[2], col[3], col[4], col[5], col[6]);
				this.list.push(r);
			}
			console.log("Found " + this.list.length + " runners");
		}
		else
			console.log("Error! Nothing found in: " + filename);
	},
	search : function(last, first, racedate, raceage)
	{
		for (var i = 0, len = this.list.length; i < len; i++)
		{
			var r = this.list[i];
			if (r.match(last, first))
			{
				if ((racedate) && (raceage))
				{
					var calcAge=r.getAge(racedate);
					if(calcAge==raceage)
						console.log("Age matches="+calcAge)
						
					else
						{
						console.log("----AGE DOES NOT MATCH: "+r.name+" shouldbe="+calcAge+ " is="+raceage)
						}
					
				}
				return r;
			}
		}
		return null;
	}
};
