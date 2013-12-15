from scrapy.contrib.spiders import CrawlSpider, Rule
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor
from scrapy.selector import Selector
from scrapy.item import Item


class houseSpider(CrawlSpider):
    name = "ncleg"
    allowed_domains = ["ncleg.net"]
    start_urls = [
        "http://www.ncleg.net/gascripts/members/memberList.pl?sChamber=House",
		"http://www.ncleg.net/gascripts/members/memberList.pl?sChamber=senate"
        
    ]
    rules = (Rule(SgmlLinkExtractor(allow=('viewMember\.pl', )), callback='parse_item'), )



    def parse_item(self, response):
        sel = Selector(response)
        sites = sel.xpath('//td[@class="memberTabBar"]')
        t = sel.xpath('//div[@id="title"]')
        print t.extract()



        for site in sites:
            title = site.xpath('a/text()').extract()
            link = site.xpath('a/@href').extract()
            desc = site.xpath('text()').extract()
            print title, link, desc



#    def parse(self, response):
#        filename = response.url.split("/")[-2]
#        open(filename, 'wb').write(response.body)