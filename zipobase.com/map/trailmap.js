var route_table;
var segment_remove_click;
var segment_template;
var gSvgDoc;
var svgdoc_width;
var svgdoc_height;
var svgdoc_offset_x = 200;
var svgdoc_offset_y = 30;

var vb_width_max = 3000;
var vb_height_max = 3500;
var vb_x = 0;
var vb_width = vb_width_max;
var vb_y = 0;
var vb_height = vb_height_max;
var g_path_highlighted = 0;
var g_popup_table;



function show_hide_svg(elm_id, val) {
    document.getElementById(elm_id).setAttribute("visibility", (val ? 'visible' : 'hidden'));
}
var current_route = new HashTable();

var trailmap_options = new HashTable({
    show_water: { def: 1, arg: 'layer_water', func: show_hide_svg },
    show_map: { def: 1, arg: 'layer_map', func: show_hide_svg },
    show_text: { def: 1, arg: 'layer_text', func: show_hide_svg },
    show_outline: { def: 1, arg: 'layer_outline', func: show_hide_svg },
    show_satellite: { def: 0, arg: 'layer_satellite', func: show_hide_svg },
    show_popup: { def: 1 },
    zoom: { def: 2 },
    /*
    two: { def: 2, func: function () { return "function two!" } },
    */
});

function init_menu_item(opt_item_key, opt_item) {
    var opt_val = getCookie(opt_item_key);
    if (opt_val == null)
        opt_val = opt_item.def;
    else {
        var opt_val_number = Number(opt_val);
        if (opt_val_number != NaN)
            opt_val = opt_val_number;
    }

    var menu_item = document.getElementById(opt_item_key);
    if (menu_item) {
        var func_name = "valset_"+menu_item.getAttribute('data-mi-type') ;
        var func = zs.menu[func_name];
        if (func) {
            func(menu_item, opt_val);
        }
    }
    if(opt_item.func)
        opt_item.func(opt_item.arg, opt_val);
    opt_item.value = opt_val;



}
function init_menu() {
    trailmap_options.each(init_menu_item);



}

function create_mouse_paths() {

    var path_ol_group = document.getElementById("path_ol_group")
    var path_group = document.getElementById("path_group")
    var paths = path_group.getElementsByTagName("svg:path");
    if (paths.length == 0)
        paths = path_group.getElementsByTagName("path");
    var path_mouse_group = document.getElementById("path_mouse_group");
    var i;
    var len = paths.length;

    for (i = 0; i < len ; i++) {
        var mouse_path = paths[i].cloneNode();
        mouse_path.setAttribute("id", paths[i].id + "-mouse");
        mouse_path.setAttribute("data-path-id", paths[i].id);
        path_mouse_group.appendChild(mouse_path);
        mouse_path.setAttribute("class", "path_mouse");
        mouse_path.oncontextmenu = path_right_click;
        paths[i].oncontextmenu = path_right_click;
        /*
        var outline = paths[i].cloneNode();
        outline.setAttribute("class", "path_ol");
        outline.setAttribute("id", "");
        path_ol_group.appendChild(outline);
        */

        //new_path.addEventListener("oncontextmenu", path_right_click, true);

    }

}
function on_menu_set(opt_name, opt_val) {
    var opt = trailmap_options.getItem(opt_name);
    if (opt) {
        opt.value = opt_val;
        if(opt.func)
            opt.func(opt.arg, opt_val);
        setCookie(opt_name, opt_val);
    }
}



function on_key_press(e) {
    var e = window.event || e;
    if (g_path_highlighted) {
        var val = e.charCode - 48;
        if (val < 0 || val > 9)
            return;
        path_set_selection(g_path_highlighted, val);

    }


}
$(document).ready(function ()
{
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

    zs.menu.option_set_callback = on_menu_set;

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

});

var dbl_click_toggle = 0;
function OnDoubleClick(e) {
    // cross-browser wheel delta
    var e = window.event || e; // old IE support
    var scale = .30;
    if (dbl_click_toggle) {
        dbl_click_toggle = 0;
        viewBox_reset();
    }
    else {
        dbl_click_toggle = 1;
        Zoom(e, scale);
    }

}

function calcSvgPoint(x, y) {
    var scale_x = vb_width / svgdoc_width;
    var scale_y = vb_height / svgdoc_height;
    var actual_vb_width = vb_width;
    var actual_vb_height = vb_height;
    var scale = scale_x;
    if (scale_y > scale) {
        scale = scale_y;
        actual_vb_width = svgdoc_width * scale;
    }
    else {
        actual_vb_height = svgdoc_height * scale;

    }
    x = x - svgdoc_offset_x;
    y = y - svgdoc_offset_y;

    offset_x = Math.round(x * scale + vb_x - (actual_vb_width - vb_width) / 2);
    offset_y = Math.round(y * scale + vb_y - (actual_vb_height - vb_height) / 2);


    document.getElementById("stat_viewXY").innerHTML = offset_x + " " + offset_y;
    document.getElementById("stat_scale").innerHTML = Math.round(scale * 100) / 100;
    return { 'x': offset_x, 'y': offset_y };

}
function onViewBoxSet() {
    var v = document.getElementById("stat_viewBox");
    var vb_string = v.value;
    gSvgDoc.setAttribute("viewBox", v.value);

    var n = vb_string.split(" ");
    vb_x = n[0];
    vb_y = n[1];
    vb_width = n[2];
    vb_height = n[3];

}
function setViewBox() {
    var vb_string = String(vb_x) + " " + String(vb_y) + " " + String(vb_width) + " " + String(vb_height);

    gSvgDoc.setAttribute("viewBox", vb_string);
    var v = document.getElementById("stat_viewBox");
    v.value = vb_string;
}
function getSvgRect() {
    var rect = gSvgDoc.getBoundingClientRect();
    svgdoc_X = Math.round(rect.left);
    svgdoc_Y = Math.round(rect.top);
    svgdoc_X2 = Math.round(rect.right);
    svgdoc_Y2 = Math.round(rect.bottom);
    document.getElementById("stat_svgrect").innerHTML = svgdoc_X + " " + svgdoc_Y + " " + svgdoc_X2 + " " + svgdoc_Y2;
}
function onWindowResize() {
    document.getElementById("stat_window").innerHTML = window.innerWidth + " x " + window.innerHeight;

    svgwindow = document.getElementById("svgwindow");
    svgdoc_width = window.innerWidth - svgdoc_offset_x;
    svgdoc_height = window.innerHeight - svgdoc_offset_y;
    var x = document.documentElement.clientWidth;
    gSvgDoc.setAttribute("visibility", "hidden");
    gSvgDoc.setAttribute("width", svgdoc_width);
    var w = gSvgDoc.getAttribute("width");
    gSvgDoc.setAttribute("height", svgdoc_height);
    gSvgDoc.setAttribute("visibility", "visible");
    getSvgRect();


}
function viewBox_reset() {
    vb_x = 0;
    vb_width = vb_width_max;
    vb_y = 0;
    vb_height = vb_height_max;
    setViewBox();
}
function MouseMoveHandler(e) {
    var x = e.pageX;
    var y = e.pageY;
    calcSvgPoint(x, y);
    document.getElementById("stat_mouse").innerHTML = String(x) + ":" + String(y);
}
function Zoom(e, scale) {
    var e = window.event || e; // old IE support
    var x = e.pageX;
    var y = e.pageY;
    var svg_point = calcSvgPoint(e.pageX, e.pageY);


    var new_vb_width = vb_width * scale;
    var new_vb_height = vb_height * scale;
    var new_vb_x = svg_point.x - (svg_point.x - vb_x) * scale
    var new_vb_y = svg_point.y - (svg_point.y - vb_y) * scale

    vb_x = Math.round(new_vb_x);
    vb_y = Math.round(new_vb_y);
    vb_width = Math.round(new_vb_width);
    vb_height = Math.round(new_vb_height);
    if (vb_width > vb_width_max)
        viewBox_reset();

    setViewBox();
    getSvgRect();



}
function MouseWheelHandler(e) {
    // cross-browser wheel delta
    var e = window.event || e; // old IE support
    var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));
    var zoom = trailmap_options.getItem('zoom').value;
    var scale = 1 - .10 * zoom;
    if (delta < 0)
        scale = 1 / scale;
    Zoom(e, scale);
}


function seg_remove(elm) {
    var route_table = document.getElementById("route");

    var rows = route_table.getElementsByTagName("tr");
    var num = rows.length;


    for (var i = num - 1; i >= 0; i--) {
        var id = rows[i].getAttribute("data-segid");
        if (id == elm.id) {
            route_table.removeChild(rows[i]);
            return;
        }
    }
}
function seg_add(elm) {
    var route_table = document.getElementById("route");
    var rows = route_table.getElementsByTagName("tr");
    var last_row = rows[rows.length - 1];
    var trail_name = elm.getAttribute("data-trailname");

    if (last_row.getAttribute("data-trailname") != trail_name) {
        last_row = segment_template.cloneNode(true);
        last_row.setAttribute("data-segid", elm.id);
        last_row.setAttribute("data-trailname", trail_name);
        last_row.style.display = "table-row";
        route_table.appendChild(last_row);
    }
    var cells = last_row.getElementsByTagName("td");
    cells[0].textContent = trail_name;
    cells[1].textContent = elm.id;
    var dist = Number(cells[2].textContent);
    dist = dist + Number(elm.getAttribute("data-section_dist"));
    cells[2].textContent = dist;
}

function get_target_path(elm) {
    var cl = elm.getAttribute('class');
    if (cl == "path_mouse") {
        var id = elm.getAttribute('data-path-id');
        elm = document.getElementById(id);
    }
    return elm;
}

function on_over(elm, e) {
    if (!e) e = event;
    var offsetX = 35;
    var offsetY = 35;
    elm = get_target_path(elm);
    var cl = elm.getAttribute('class');
    cl = cl + " highlight";
    elm.setAttribute('class', cl);

    g_path_highlighted = elm;


    if (trailmap_options.getItem('show_popup').value == 0)
        return;
    var info_id = "info-" + elm.getAttribute('id')
    g_popup_table = document.getElementById(info_id);
    g_popup_table.className = "info_table_show";

    if (e.clientX > window.innerWidth / 2)
        offsetX = -g_popup_table.clientWidth - 30;
    if (e.clientY > window.innerHeight / 2)
        offsetY = -g_popup_table.clientHeight - 30;

    var x = e.clientX + document.body.scrollLeft + offsetX;
    var y = e.clientY + document.body.scrollTop + offsetY;


    g_popup_table.style.left = x + 'px';
    g_popup_table.style.top = y + 'px';



}
function on_out(elm) {
    elm = get_target_path(elm);
    var cl = elm.getAttribute('class');
    cl = cl.replace("highlight", "")
    elm.setAttribute('class', cl);
    if(g_popup_table)
        g_popup_table.className = "info_table_hide";
    g_path_highlighted = 0;

}
function path_right_click( e) {
    if (!e) e = event;

    return false;
}

function route_reset() {
    $("#path_group").children().each(function (index) {
        path_set_selection(this, 0);

    });


}

function route_string_change() {
    route = document.getElementById("stat_route_string").value;
    route_string_set(route);

}
function route_string_set(route) {
    route_reset();
    var array_paths = route.split('.');



    for (var i = 0; i < array_paths.length;i++) {
        var path_set = array_paths[i];
        var path_pair=path_set.split(':');
        var path = document.getElementById(path_pair[0]);

        if (path) {
            path_set_selection(path, path_pair[1]);
        }
        
    }

}

function route_string_make() {
    var route = "";
    $("#path_group").children().each(function (index) {
        var s = this.getAttribute('data-trail-state');
        if (s > 0) {
            route = route + this.id + ':' + s + '.';
        }
    });
    return route;
}
function route_string_show() {

}

function savefile() {
    var path = window.location.hostname + window.location.pathname;

    var route = route_string_make();

    make_http_req()
    
    return route;

}
function path_set_selection(elm,count_new) {

    var count_old = Number(elm.getAttribute('data-trail-state'));
    var class_state_old = "sel" + count_old;
    var total_distance_elm = document.getElementById("total_distance");
    var distance = Number(elm.getAttribute('data-trail-length'));
    total_distance = Number(total_distance_elm.innerHTML);

    total_distance = total_distance +(count_new- count_old )* distance;


    var class_state_new = "sel" + count_new;

    total_distance_elm.innerHTML = Math.round(total_distance * 100) / 100;


    var cl = elm.getAttribute('class');
    cl = cl.replace(class_state_old, class_state_new)
    cl = cl.replace("highlight", "")

    elm.setAttribute('class', cl);

    elm.setAttribute('data-trail-state', count_new);
    var len = elm.getTotalLength();
    var point = elm.getPointAtLength(len / 2);
    var x = point.x;
    var route = route_string_make();
    if (console)
        console.log(route);
    $("#stat_route_string").innerHTML = route;
    
    document.getElementById("stat_route_string").value = route;

}
function toggle1(elm) {
    elm = get_target_path(elm);

    var state = Number(elm.getAttribute('data-trail-state'));
    state = state + 1;
    if (state > 2) {
        state = 0;
    }
    path_set_selection(elm, state);
}

function setCookie(c_name, value) {
    var exdays = 999;
    var exdate = new Date();
    exdate.setDate(exdate.getDate() + exdays);
    var c_value = escape(value) + ((exdays == null) ? "" : "; expires=" + exdate.toUTCString());
    document.cookie = c_name + "=" + c_value;
}
function getCookie(c_name) {
    var i, x, y, ARRcookies = document.cookie.split(";");
    for (i = 0; i < ARRcookies.length; i++) {
        x = ARRcookies[i].substr(0, ARRcookies[i].indexOf("="));
        y = ARRcookies[i].substr(ARRcookies[i].indexOf("=") + 1);
        x = x.replace(/^\s+|\s+$/g, "");
        if (x == c_name) {
            var val = unescape(y);
            if (val == 'false')
                val = 0;
            if (val == 'true')
                val = 1;
            return val;
        }
    }
    return null;
}

function routename_change(entry) {
    document.getElementById("routename").textContent = entry.value;
    setCookie("route_name", entry.value, 1000);


}
function make_http_req(url) {
    var request;

    // Create a request object. Try Mozilla / Safari method first.
    if (window.XMLHttpRequest) {
        request = new XMLHttpRequest();

        // If that doesn't work, try IE methods.
    } else if (window.ActiveXObject) {
        try {
            request = new ActiveXObject("Msxml2.XMLHTTP");
        } catch (e1) {
            try {
                request = new ActiveXObject("Microsoft.XMLHTTP");
            } catch (e2) {
            }
        }
    }
    // If we couldn't make one, abort.
    if (!request) {
        window.alert("No ajax support.");
        return false;
    }
    request.open("GET", url);
    request.send();
    return request.responseText;
}

function get_route_list() {
    var site = window.location.hostname + "/cgi-bin/trailmap.py?act=list";
    make_http_req(site);




}

