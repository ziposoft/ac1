phantom.injectJs('inc/godiva.js');
var s = new zipo.scrape.page("fred", "http://localhost");
zipo.con.out(s.timer_timeout)
var f = new zipo.File("test.txt");
f.out("hello?");
f.close();
phantom_exit(0);