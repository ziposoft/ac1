pjs.addSuite({
    url: 'http://localhost/',
    scrapers: [
        function() {
            var items = [];
            items.push($('h1').first().text());
            $('li a').each(function() {
                items.push($(this).text());
            });
            return items;
        }
    ]
});