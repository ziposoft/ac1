var zipo = zipo || {};
var zipo = (function(z)
{
	var acsColumns =
	{
		'div pl' : 'placeGroup',
		'time' : 'time',
		'group' : 'group',
		'division' : 'group',
		'div' : 'group',
		'city' : 'city',
		'state' : 'state',
		'gender' : 'gender',
		'pl' : 'placeOverall',
		'place' : 'placeOverall',
		'age' : 'age',
		'name' : 'nameFull',
		'first name' : 'nameFirst',
		'last name' : 'nameLast',
	};
	var resultRow = function()
	{
		this.last = "";
		this.first = "";
		this.raceage = 0;
		this.time = "";
		this.placeOverall = 0;
		this.placeGroup = 0;
		this.group = "";
		this.gender = "U";
		this.age = 0;
	}
	resultRow.prototype =
	{
		create : function(columns, data)
		{
			for ( var i in columns)
			{
				this[i] = data[columns[i]];
			}
			if (this.nameFull)
			{
				var names = this.nameFull.split(',');
				this.last = names[0];
				this.first = names[1].trim();
			}
		}
	};
	var acsRace = function()
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
			f.out("\n\n" + this.name)
			f.out(this.city + ", " + this.state + "    " + this.date + "\n")
			if (this.results.length == 0)
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
			if ((!last) || (!first)) return false;
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
	var matchResult = function(race, runner, rr)
	{
		this.race = race;
		this.rr = rr;
		this.runner = runner;
		this.calcAge = null;
	}
	matchResult.prototype =
	{
		output : function(f)
		{
			var str = this.runner.first + " " + this.runner.last + "  " + this.rr.time;
			if (this.calcAge)
			{
				str += "(Age " + this.calcAge + " !=" + this.rr.age + ")";
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
		output : function(f)
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
		process : function(race, rr)
		{
			for (var i = 0, len = this.list.length; i < len; i++)
			{
				var runner = this.list[i];
				if (runner.match(rr.last, rr.first))
				{
					var result = new matchResult(race, runner, rr);
					if ((race.date) && (rr.age))
					{
						var calcAge = runner.getAge(race.date);
						result.age = calcAge;
						if (calcAge != rr.age)
						{
							result.calcAge = calcAge;
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
	function acsProcessRaceData(raceobj, htmldata)
	{
	}
	return z;
}(zipo));
