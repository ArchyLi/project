from anaconda_project.plugins.network_util import urlparse
from bs4 import BeautifulSoup
import re


def _get_new_urls(page_url, soup):
    new_urls = set()
    # href = "/proprice/pcshanxii.html"
    # title
    pa_url = 'http://www.creprice.cn/'
    links = soup.find_all('a', href=re.compile(r"/proprice/pcshanxii.ti\d+\.html"))
    for link in links:
        new_url = link['href']
        new_full_url = urlparse.urljoin(page_url, new_url)
        new_urls.add(new_full_url)
    #print (new_full_url)
    return new_urls


class HtmlParser(object):
    #解析数据
    def _get_new_datas (self, page_url, soup):
        res_data = {}#字典

        #url
        res_data['url'] = page_url
        #<div class="utitle"><h2 class="ranktit">2017年12月</h2>
        title_node = soup.find('div', class_ = "utitle").find("h2")
        #print (title_node)
        res_data['title'] = title_node.get_text()
        #print(res_data['title'])
        #数据
        datas = []
        summary_node = soup.find_all('td')
        #print (summary_node)
        for lin in summary_node:
            data = lin.get_text()
            data2 = ' '
            data += data2
            #datas.strip()
            res_data.setdefault('summary', []).append(data)
        #print("ssssssssssssssssssssssssssss")
        #print (res_data['summary'])
        #print(datas)
        return res_data


    def parse(self, page_url, html_cont):
        if page_url is None or html_cont is None:
            #print (page_url)
            #print (html_cont)
            return
        soup = BeautifulSoup(html_cont, 'html.parser', from_encoding='utf-8')

        new_urls = _get_new_urls(page_url, soup)
        new_data = self._get_new_datas(page_url, soup)
        return new_urls,new_data
