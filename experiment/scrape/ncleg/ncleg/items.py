# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/en/latest/topics/items.html

from scrapy.item import Item, Field

class NclegItem(Item):
    # define the fi Field() elds for your item here like:
    chamber= Field() 
    uid=   Field() 
    district = Field()      
    name = Field()
    phone = Field()

    county = Field()        
    email = Field()
    url = Field()        
    address = Field() 
    
    pass


class VoteItem(Item):
    # define the fields for your item here like:
    mid=Field()
    vid = Field()
    doc = Field()
    vote = Field()
    nid=Field()
    
    
      
    pass
