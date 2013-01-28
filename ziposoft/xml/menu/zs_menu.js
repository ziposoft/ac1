


(function (zs, window, document, undefined) {
    var menu = zs.menu = zs.menu || (function () {
        var option_set_callback=0;
        var options = {};

        function hide(elm, ev) {
            if (
                (ev.target.className == 'zs_menutop') ||
                (ev.target.className == 'zs_menu') ||
                (ev.target.className == 'zs_menu_sub') ||
                (ev.target.className == 'zs_menu_item_sub') ||
                (ev.target.className == 'zs_menu_item_sub_hi') ||
                (ev.target.className == 'zs_menu_link')
            ) {
                var lists = elm.getElementsByTagName('table');

                if (lists.length > 0) {
                    lists[0].style.display = "none";
                    //  if(console)  console.log("hide %s %d %s ",elm.className,count++, ev.target.className);
                }
            }
            else {
                if (console) console.log("hide %s %s", ev.target, ev.target.className);
            }
        }
        function show(elm, ev) {

            var othermenus = document.getElementsByClassName('menu');

            

            var oElement;
            for (var i = 0; i < othermenus.length; i++) {
                othermenus[i].style.display = "none";
            }
            var lists = elm.getElementsByTagName('table');
            if (lists.length > 0) {
                lists[0].style.display = "inline";
            }
        }
        function hide_sub(elm, ev) {
            elm.className = 'zs_menu_item_sub';
            hide(elm, ev);
        }
        function show_sub(elm, ev) {
            elm.className = 'zs_menu_item_sub_hi';
            show(elm, ev);
        }
        function bool_toggle(elm, ev) {

            if (elm.boolval) {
                elm.boolval = 0;
                elm.firstElementChild.style.display = "none";
            }
            else {
                elm.boolval = 1;
                elm.firstElementChild.style.display = "inline";
            }
            if(this.option_set_callback)
                this.option_set_callback(elm.id, elm.boolval);
            options[elm.id] = 0;
            options[elm.id] = elm.boolval;
       
        }
        return {
            option_set_callback: option_set_callback,
            bool_toggle: bool_toggle,
            bool_toggle: bool_toggle,
            hide_sub: hide_sub,
            show_sub: show_sub,
            options: options,
            hide: hide,
            show: show
        }
    }());
}(window.zs = window.zs || {}, window, document));

