from scrapy.contrib.spiders import CrawlSpider, Rule

from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor
from scrapy.selector import Selector
from ncleg.items import NclegItem,VoteItem

from scrapy.exceptions import CloseSpider
from urlparse import urlparse,parse_qsl

#bills = [ 'HB66','HB33','HB612','HB930','HB905','HB956','SB626','SB689','SB648','SB744']
bills = ['SB744']
class voteSpider(CrawlSpider):
	name = "votes"
	count = 0
	allowed_domains = ["ncleg.net"]
	start_urls = [
		 "http://www.ncleg.net/gascripts/members/memberList.pl?sChamber=House",     
		"http://www.ncleg.net/gascripts/members/memberList.pl?sChamber=senate",
	  # "http://www.ncleg.net/gascripts/voteHistory/MemberVoteHistory.pl?sSession=2013&sChamber=S",
	  # "http://www.ncleg.net/gascripts/voteHistory/MemberVoteHistory.pl?sSession=2013&sChamber=H",
	  # "http://www.ncleg.net/gascripts/voteHistory/MemberVoteHistory.pl?sSession=2011&sChamber=S",
	  # "http://www.ncleg.net/gascripts/voteHistory/MemberVoteHistory.pl?sSession=2011&sChamber=H",
	#   "http://www.ncleg.net/gascripts/voteHistory/MemberVoteHistory.pl?sSession=2013&sChamber=H"
	 #   "http://www.ncleg.net/gascripts/members/memberList.pl?sChamber=senate"
	  #  "http://www.ncleg.net/gascripts/voteHistory/MemberVoteHistory.pl?sSession=2013&nMMUserID=18&sChamber=H"
		
	]
	rules = (
			 
			  Rule(SgmlLinkExtractor(allow=('viewMember\.pl', ))),  
			  Rule(SgmlLinkExtractor(allow=('introducedBills\.pl',)), callback='parse_bills', follow= True),
			  Rule(SgmlLinkExtractor(allow=('MemberVoteHistory\.pl',)), callback='parse_votes', follow= True),

			 )
	

	def stop(self):
		raise CloseSpider('stop')
		return
	def parse_member(self, response):
	
		return response

	def parse_bills(self, response):
		
		params= dict(parse_qsl(urlparse(response.url).query))
		chamber=params['Chamber']
		member_id=params['nUserID']
		year=2013
		sel = Selector(response)
		tbl=sel.xpath('//th[.="Bill"]/../..')     
		rows=tbl.select('./tr')
		votes=[]
		v=''
		for row in rows:
			
			doc = row.xpath('./td[1]/a/text()');
			if(doc) :
				doc=doc.extract()[0]
				v='sp'
			else:
				doc = row.xpath('./td[1]/b/a/text()');
				if(doc):
				   doc=doc.extract()[0]
				   v='psp'

			if(doc):
				doc=doc.replace("S", "SB")
				doc=doc.replace("H", "HB")
		
			vid=''
			mid=chamber+member_id;            
			if(v=='Vote'):
				continue
			if(year=='2013'):
				vid='13'
			if(year=='2011'):
				vid='11'
				continue;
			
	
			if(doc in bills):
				vote=VoteItem()    
				vote['mid'] =  mid                
				vote['doc'] =  doc
				vote['vote'] = v               
				print (doc,mid,vid,v)
				votes.append(vote)  
		self.count+=1       
		return votes
		  
	def parse_votes(self, response):
	  
		params= dict(parse_qsl(urlparse(response.url).query))
		chamber=params['sChamber']
		year=params['sSession']
		if(chamber=='H'):
			member_id=params['nMMUserID']
		if(chamber=='S'):
			member_id=params['nMMUserID']   
			
		sel = Selector(response)
		tbl=sel.xpath('//td[.="Doc."]/../..')     
		rows=tbl.select('./tr')
		votes=[]
		for row in rows:
		   
			rcs = row.xpath('./td[1]/text()').extract()[0]
			doc = row.xpath('./td[2]/a/text()');
			if(doc) :
				doc=doc.extract()[0]
				doc=doc.replace(" ", "")
			
			v = row.xpath('./td[5]/text()').extract()[0]
			vid=''
			mid=chamber+member_id;            
			if(v=='Vote'):
				continue
			if(year=='2013'):
				vid='13'
			if(year=='2011'):
				vid='11'
				continue;
			
			vid+=chamber
			vid+=rcs
			if(doc in bills):
				vote=VoteItem()    
				vote['mid'] =  mid                
				vote['vid'] =  vid
				vote['doc'] =  doc
				vote['vote'] = v               
				print (doc,mid,vid,v)
				votes.append(vote)  
		self.count+=1       
		return votes
	
	def parse_item(self, response):
		
		member_id=-1
		if(self.count>1):
			self.stop()
		sel = Selector(response)
		tool= NclegItem()
		tool['name'] = sel.xpath('//div[@id="title"]/text()').extract()
		tbl=sel.xpath('//span[.="Office:"]/../../..')
		tool['phone'] = tbl.xpath('./tr[2]/td[2]/span/text()').extract()
		tool['email'] = tbl.xpath('./tr[3]/td[2]/span/a/text()').extract()
		tool['district'] = tbl.xpath('./tr[6]/td[2]/span/text()').extract()
		tool['county'] = tbl.xpath('./tr[7]/td[2]/a/text()').extract()
		tool['address'] = tbl.xpath('./tr[4]/td[2]/span/text()').extract()
		tool['url'] = response.url

		
		params= dict(parse_qsl(urlparse(response.url).query))
		chamber=params['sChamber']
		if(chamber=='H'):
			member_id=params['nMMUserID']
		if(chamber=='senate'):
			member_id=params['nUserID']           
		
		print (member_id    )
		
		self.count+=1
		print self.count
		return tool
		#sites = sel.xpath('//td[@class="memberTabBar"]')

		#exit()



#        for site in sites:
#            title = site.xpath('a/text()').extract()
#            link = site.xpath('a/@href').extract()
#            desc = site.xpath('text()').extract()
#            print title, link, desc



#    def parse(self, response):
#        filename = response.url.split("/")[-2]
#        open(filename, 'wb').write(response.body)


class houseSpider(CrawlSpider):
	name = "ncleg"
	count = 0
	allowed_domains = ["ncleg.net"]
	start_urls = [
		"http://www.ncleg.net/gascripts/members/memberList.pl?sChamber=House",
		"http://www.ncleg.net/gascripts/members/memberList.pl?sChamber=senate"
	  #  "http://www.ncleg.net/gascripts/voteHistory/MemberVoteHistory.pl?sSession=2013&nMMUserID=18&sChamber=H"
		
	]
	rules = (Rule(SgmlLinkExtractor(allow=('viewMember\.pl',)), callback='parse_item'),
			# Rule(SgmlLinkExtractor(allow=('MemberVoteHistory\.pl',)), callback='parse_votes'),
			 )
	

	def stop(self):
		raise CloseSpider('stop')
		return
		 
	def parse_votes(self, response):       
		return
	def parse_item(self, response):
		
		member_id=-1
		#if(self.count>1):
		#    self.stop()
		sel = Selector(response)
		tool= NclegItem()
		tool['name'] = sel.xpath('//div[@id="title"]/text()').extract()
	   
		tbl=sel.xpath('//span[.="Office:"]/../../..')
		tool['phone'] = tbl.xpath('./tr[2]/td[2]/span/text()').extract()
		tool['email'] = tbl.xpath('./tr[3]/td[2]/span/a/text()').extract()
		tool['district'] = tbl.xpath('./tr[6]/td[2]/span/text()').extract()
		tool['county'] = tbl.xpath('./tr[7]/td[2]/a/text()').extract()
		tool['address'] = tbl.xpath('./tr[4]/td[2]/span/text()').extract()
		tool['url'] = response.url
		
		params= dict(parse_qsl(urlparse(response.url).query))
		

		
	   
		chamber=params['sChamber']
		if(chamber=='senate'):
			tool['chamber'] ='S'
		if(chamber=='House'):
			tool['chamber'] ='H'   
		member_id=     params['nUserID']
		tool['uid'] =  member_id
		print member_id
		self.count+=1
		print self.count
		return tool
		#sites = sel.xpath('//td[@class="memberTabBar"]')

		#exit()



#        for site in sites:
#            title = site.xpath('a/text()').extract()
#            link = site.xpath('a/@href').extract()
#            desc = site.xpath('text()').extract()
#            print title, link, desc



#    def parse(self, response):
#        filename = response.url.split("/")[-2]
#        open(filename, 'wb').write(response.body)
