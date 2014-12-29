/////////////////////    INIT   /////////////////////////////////////////////////////
$(document).ready(function () {
	// initialization code goes here

	if ((BrowserDetect.browser != "Chrome") && (BrowserDetect.browser != "Firefox")) {
		window.alert("Trail Map has only been tested in the latest versions of Chrome and Firefox.\nIt probably will not work in your browser");
	}

	/*
    var rname = getCookie("route_name");
    document.getElementById("routename").textContent = rname;
    segment_template = document.getElementById("segtmp");
    route_table = document.getElementById("route");
    window.ondblclick = MouseWheelHandler;
    window.ondblclick= OnDoubleClick;
    */
	gSvgDoc = document.getElementById("svgdoc");




	window.onkeypress = on_key_press;
	window.onmousemove = MouseMoveHandler;
	window.onresize = onWindowResize;
/*
	zs.menu.option_set_callback = on_menu_set;
	*/
	onWindowResize();
	if (window.addEventListener) {
		// IE9, Chrome, Safari, Opera
		window.addEventListener("mousewheel", MouseWheelHandler, true);
		// Firefox
		window.addEventListener("DOMMouseScroll", MouseWheelHandler, true);
	} else window.attachEvent("onmousewheel", MouseWheelHandler);
	setViewBox();
	create_mouse_paths();
	init_menu();

	document.getElementById("browser_info").innerHTML = BrowserDetect.browser + " " + BrowserDetect.version + " " + BrowserDetect.OS;
	//$("#scratch").load("http://localhost/cgi-bin/trailmap.cgi?act=list");

	var list = $.getValues("http://localhost/cgi-bin/trailmap.cgi?act=list");
	$("#scratch").html(list);
	//var list = get_route_list();
	//document.getElementById("scratch").innerHTML = list;
});


jQuery.extend
(
    {
    	getValues: function (url) {
    		var result = null;
    		$.ajax(
                {
                	url: url,
                	type: 'get',
                	dataType: 'html',
                	async: false,
                	cache: false,
                	success: function (data) {
                		result = data;
                	}
                }
            );
    		return result;
    	}
    }
);
