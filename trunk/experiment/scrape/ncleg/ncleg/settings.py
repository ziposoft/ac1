# Scrapy settings for ncleg project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/en/latest/topics/settings.html
#

BOT_NAME = 'ncleg'

SPIDER_MODULES = ['ncleg.spiders']
NEWSPIDER_MODULE = 'ncleg.spiders'
LOG_LEVEL = 'ERROR'

# Crawl responsibly by identifying yourself (and your website) on the user-agent
#USER_AGENT = 'ncleg (+http://www.yourdomain.com)'
