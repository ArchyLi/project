from house_spider import url_manager, html_downloader, html_parser, html_outputer

#总调度程序和入口程序
class SpiderMain(object):
    def __init__(self):
        self.urls = url_manager.UrlManager()# url管理器
        self.downloader = html_downloader.HtmlDownloader()# 下载器
        self.parser = html_parser.HtmlParser()# 解析器
        self.outputer = html_outputer.HtmlOutputer()# 输出器
    # 爬虫调度程序,如果有url则爬取所有有关的url页面

    def craw(self, root_url):
        count = 1
        #放入带爬取的url
        self.urls.add_new_url(root_url)
        #此处找到待爬取的url
        while self.urls.has_new_url():
            try:
                #获取一个带爬取的url
                new_url = self.urls.get_new_url()
                print ('craw %d: %s' % (count, new_url))
                #下载器下载页面，得到下载好的数据
                html_cont = self.downloader.download(new_url)
                #解析器解析新的url以及新的数据
                new_urls, new_data = self.parser.parse(new_url, html_cont)
                #得出的url和数据分别进行处理,添加批量url和收集数据
                self.urls.add_new_urls(new_urls)
                self.outputer.collect_data(new_data)
                count = count + 1
            except:
                print ('craw failed')

        #输出爬取好的数据
        self.outputer.output_html()

if __name__ == "__main__":
    root_url = "http://www.creprice.cn/proprice/pcshanxii.html"
    obj_spider = SpiderMain()
    obj_spider.craw(root_url)
