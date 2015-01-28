

var acsColumns=
{

		'div pl': 'placeGroup',
		'time': 'time',
		'group': 'group',
		'division': 'group',
		'div': 'group',
		'city': 'city',
		'gender': 'gender',
		'pl': 'placeOverall',
		'place': 'placeOverall',		
		'age': 'age',	
		'name': 'nameFull',	
		'first name': 'nameFirst',	
		'last name': 'nameLast',	

};
var resultRow=function()
{
	this.last="";
	this.first="";
	this.raceage=0;
	this.time="";
	this.placeOverall=0;
	this.placeGroup=0;
	this.group="";

}

var acsRace = function(name, date, url, city, state)
{
	this.name = name;
	this.date = date;
	this.url = url;
	this.city = city;
	this.state = state;
	this.results = [];
}
acsRace.prototype =
{
	output : function(f)
	{
		f.out("\n\n"+this.name)
		f.out(this.city + ", " + this.state + "    " + this.date+"\n")
		if(this.results.length==0)
			{
			
			f.out("(no Godiva Runners Found)")
			}
		jQuery.each(this.results, function(i, val)
		{
			val.output(f);
		});
		
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
	// console.log(this.first+ " " +this.last)
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
		return calcAge(date, this.dob);
		;
	}
}
var raceResult = function(race, runner, time, placeOverall, group, placeGroup)
{
	this.race = race;
	this.runner = runner;
	this.placeOverall = placeOverall;
	this.placeGroup = placeGroup;
	this.group = group;
	this.time = time;
	this.age = 0;
	this.ageDiff = null;
}
raceResult.prototype =
{
	output : function(f)
	{
		var str = this.runner.first + " " + this.runner.last + "  " + this.time;
		if (this.ageDiff)
		{
			str+= "(Age "+ this.ageDiff+" !=" +this.age+")";
				
		}
		f.out(str)
	}
}
var gRaceResults =
{
	byrace :
	{
		results : []
	},
	results : [],
	races : [],
	add : function(race, runner, placeOverall, placeGroup, time)
	{
		byrace[race.name].results.push()
	},
	output : function (f)
	{
		jQuery.each(this.races, function(i, val)
		{
			val.output(f);
		});		
		
		
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
	process : function(race, resultrow)
	{
		for (var i = 0, len = this.list.length; i < len; i++)
		{
			var r = this.list[i];
			if (r.match(last, first))
			{
				var result = new raceResult(race, r, time, placeOverall, group, placeGroup);
				if ((race.date) && (raceage))
				{
					var calcAge = r.getAge(race.date);
					result.age = calcAge;
					if (calcAge != raceage)
					{
						result.ageDiff = raceage;
						// console.log("----AGE DOES NOT MATCH: "+r.name+"
						// shouldbe="+calcAge+ " is="+raceage)
					}
				}
				race.results.push(result);
				return;
			}
		}
		return;
	}
};

function acsProcessRaceData(raceobj,htmldata)
{
	


}
