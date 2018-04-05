
class UrlManager(object):
    # 维护两个列表，一个待爬取的列表，一个是已经爬取过的url
    #构造函数中初始化
    def __init__(self):
        self.new_urls = set()
        self.old_urls = set()

    def add_new_url(self,url):
        if url is None:
            return
        if url not in self.new_urls and url not in self.old_urls:
            self.new_urls.add(url)

    def add_new_urls(self, urls):
        if urls is None or len(urls) == 0:
            return
        for url in urls:
            self.add_new_url(url)

    #判断是否有新的url
    def has_new_url(self):
        return len(self.new_urls) != 0

    #在url管理器中获取一个新的url
    def get_new_url(self):
        #首先获取一个
        #pop方法会从列表中获取一个url,并且移除url
        new_url = self.new_urls.pop()
        #添加这个url并且返回
        self.old_urls.add(new_url)
        return new_url


