(function (zs, window, document, undefined) {
    var product = zs.product;
    var shoppingCart = zs.shoppingCart = zs.shoppingCart || (function () {
        var items = [];
        var priceTotal = 0;

        var addProduct = function (product) {
            items.push(product);
            updatePriceTotal();
        };

        var removeProduct = function (product) {
            //remove product from items
            updatePriceTotal();
        };

        var updatePriceTotal = function () {
            //logic to update the priceTotal
            //use public functions on product to get the price of products
        };

        return {
            addProduct: addProduct,
            removeProduct: removeProduct
        }
    }());
}(window.zs = window.zs || {}, window, document));



(function (zs, window, document, undefined) {
    var product = zs.product;
    var dogcrate = zs.dogcrate = zs.dogcrate || (function () {
        var items = [];
        var priceTotal = 0;

        var addDog = function (product) {
            items.push(product);
            updatePriceTotal();
        };

        var removeDog = function (product) {
            //remove product from items
        };
        return {
            addDog: addDog,
            removeDog: removeDog
        }
    }());
}(window.zs = window.zs || {}, window, document));



function init() {
    zs.shoppingCart.removeProduct("fred");
   
    zs.dogcrate.addDog("jeffery");
}