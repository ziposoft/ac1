function call_zipo(request) {
	var url;
	var ref = document.referrer;
	var refs = ref.split('?');
	if(window.location.hostname=='localhost')
		return;
	url = "http://www.zipobase.com/cgi-bin/zb.cgi?T=logger&db=ziposoft&requested="
			+ escape(document.URL) + "&referer=" + escape(refs[0]);
	$.get(url);
	return;
}