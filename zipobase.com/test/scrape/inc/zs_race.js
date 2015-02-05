zipo.running = (function(z)
{
	var running=this;

	running.columnAlias =
	{
		'bib #' : 'bib',
		'div pl' : 'placeGroup',
		'chip time' : 'timechip',
		'time' : 'time',
		'gun time' : 'timegun',
		'group' : 'group',
		'division' : 'group',
		'div' : 'group',
		'city' : 'city',
		'club' : 'club',
		'state' : 'state',
		'st' : 'state',
		'gender' : 'gender',
		'pl' : 'placeOverall',
		'place' : 'placeOverall',
		'age' : 'age',
		'name' : 'nameFull',
		'first name' : 'nameFirst',
		'last name' : 'nameLast',
		
	};
	
	
	running.Result = function()
	{
		this.last = "?";
		this.first = "?";
		this.ageCalc = 0;
		this.time = "?";
		this.placeOverall = 0;
		this.placeGroup = 0;
		this.group = "?";
		this.gender = "U";
		this.ageResult = 0;
	}
	running.Result.prototype =
	{
		dbg : function()
		{
			z.dbgout(this.first + " " +this.last)
		},			
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
	running.races =
	{
		list : [],
		output : function(f)
		{
			jQuery.each(this.list, function(i, val)
			{
				val.output(f);
			});
		}
	}	
	running.RaceEvent = function()
	{
		this.name = "";
		this.distance = 0;

	}	
	running.Race = function()
	{
		this.name = "";
		this.date = "";
		this.url = "";
		this.city = "";;
		this.state = "";;
		this.events = [];
		this.results = [];
	}
	running.Race.prototype =
	{
		output : function(f)
		{
			if(this.results.length==0)
				{
				//f.out(this.name+ " (no Godiva Runners Found)")
				return;
				}
			f.out("\n"+ this.name)
			f.out(this.city + ", " + this.state + "    " + this.date )
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
	running.Runner = function()
	{
		
		this.first ="";// z.capitalize(first);
		this.last  ="";// z.capitalize(last);
		this.state  ="";// z.capitalize(state);
		this.city  ="";// z.capitalize(city);
		this.gender  ="U";// gender;
		this.dob  ="";// dob;
		this.alias  =[];//alias;
		
		// console.log(this.first+ " " +this.last)
	}
	running.Runner.prototype =
	{
		match : function(last, first)
		{
			if ((!last) || (!first)) return false;
			// console.log("runner: "+ JSON.stringify(this))
			last = z.capitalize(last);
			if (this.last != last) return false;
			if (first)
			{
				first = z.capitalize(first);
				if (this.first == first) return true;
				return false;
			}
			return true;
		},
		init : function()
		{
			this.first = z.capitalize(this.first);
			this.last = z.capitalize(this.last);
			this.state = z.capitalize(this.state);
			this.city = z.capitalize(this.city);
			
		},
		getAge : function(date)
		{
			// console.log("runner: "+ JSON.stringify(this))
			return z.calculateAge(date, this.dob);
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
			var str ="\t"+ this.runner.first + " " + this.runner.last + "\t" + this.rr.time;
			if (this.calcAge)
			{
				str += "(Age " + this.calcAge + " !=" + this.rr.age + ")";
			}
			f.out(str)
		}
	}

	running.runners =
	{
		list : [],
		loadfile : function(filename)
		{
			
			this.list=z.csv_load("CGTC.csv",z.running.Runner);
			
		},
		process : function(race, rr)
		{
			//rr.dbg();
			for (var i = 0, len = this.list.length; i < len; i++)
			{
				var runner = this.list[i];
				
				if (runner.match(rr.last, rr.first))
				{
					var matchresult = new matchResult(race, runner, rr);
					if ((race.date) && (rr.age))
					{
						var calcAge = runner.getAge(race.date);
						if (calcAge != rr.age)
						{
							matchresult.calcAge = calcAge;
							// console.log("----AGE DOES NOT MATCH: "+r.name+"
							// shouldbe="+calcAge+ " is="+raceage)
						}
					}
					race.results.push(matchresult);
					return;
				}
			}
			return;
		}
	};

	return running;
}(zipo));

