var acs = (function()
{
	var config =
	{ 
		delayBetweenRuns : 0,
		timeoutInterval : 100,
		timeoutLimit : 20000,
	};
	var var_x;
	// expose namespaces and API functions
	return 	{
		var_x : var_x,
	/**
	 * Set one or more config variables, applying to all suites
	 * 
	 * @name pjs.config
	 * @param {String|Object}
	 *            key Either a key to set or an object with multiple values to set
	 * @param {mixed}
	 *            [val] Value to set if using config(key, val) syntax
	 */
	config : function(key, val)
	{
		if (!key)
		{
			return config;
		} else if (typeof key == 'object')
		{
			extend(config, key);
		} else if (val)
		{
			config[key] = val;
		}
	},
	func_var_args : function()
	{
	// /uites = Array.prototype.concat.apply(suites, arguments);
	},
	func_sanple : function(url, scraper)
	{
	// suites.push({url:url, scraper:scraper});
	}
	};
}());