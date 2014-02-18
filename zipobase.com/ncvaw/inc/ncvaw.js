function ncvaw_init_your_district_menu() {

	var dist_state_senate = getCookie('dist_state_senate');
	var dist_state_house = getCookie('dist_state_house');

	if (dist_state_senate) {
		var i=$('#z_mi_dist_senate');
		i.removeClass('zs_menu_hide');
		$('#z_mi_dist_senate > a').text(
				'Your Senate District #' + dist_state_senate);
		$('#z_mi_dist_senate > a').attr('href',
				'/district.php?ch=S&dist=' + dist_state_senate);
	} else {

		$('#z_mi_dist_senate').addClass('zs_menu_hide');
	}
}
function ncvaw_init() {

	ncvaw_init_your_district_menu();
}

function setCookie(c_name, value) {
	var exdays = 999;
	var exdate = new Date();
	exdate.setDate(exdate.getDate() + exdays);
	var c_value = escape(value)
			+ ((exdays == null) ? "" : "; expires=" + exdate.toUTCString());
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

var drag_startX = 0;
function dragstart(ev) {
	// ev.dataTransfer.setData("Text",ev.target.id);

	drag_startX = ev.offsetX;

	if (console)
		console.log("offsetX %d Y %d", ev.offsetX, ev.offsetY);
}

function dragend(ev) {
	var offset = Math.abs(drag_startX - ev.offsetX);
	var perc = offset * 100 / screen.width;

	// ev.dataTransfer.setData("Text",ev.target.id);
	if (console)
		console.log("offsetX %d  %d", offset, perc);
	if (perc > 20) {

		window.location = "/"
	}
}