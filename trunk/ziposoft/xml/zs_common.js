
; (function (zs, window, document, undefined) {
    var swapClass = function (elm, oldclass, newclass) {
        var cl = elm.getAttribute('class');
        cl = cl.replace(oldclass, newclass)
        elm.setAttribute('class', cl);
    };
    var otherFunc = function () {
    };
    return {
        swapClass: swapClass,
        otherFunc: otherFunc
    }
}(window.zs = window.zs || {}, window, document));





