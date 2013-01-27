


(function (zs, window, document, undefined) {
    var menu = zs.menu = zs.menu || (function () {


        function hide(elm, ev) {
            if (
                (ev.target.className == 'zs_menutop') ||
                (ev.target.className == 'zs_menu') ||
                (ev.target.className == 'zs_menu_sub') ||
                (ev.target.className == 'zs_menu_item_sub') ||
                (ev.target.className == 'zs_menu_item_sub_hi') ||
                (ev.target.className == 'zs_menu_item')
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
        return {
            hide_sub: hide_sub,
            show_sub: show_sub,
            hide: hide,
            show: show
        }
    }());
}(window.zs = window.zs || {}, window, document));

