function init() {

   zs.menu.option_set_callback = on_menu_set;
}
function on_menu_set(opt_name, opt_val) {
    var doc = window.document;
    var el = doc.getElementById("elm1");
    if (el) {
        el.innerHTML = el.innerHTML+ "<p> " +opt_name + "=" + opt_val+"</p>";
    }
    console.log(opt_val);
}