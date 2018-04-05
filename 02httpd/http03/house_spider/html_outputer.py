import string

class HtmlOutputer(object):
    #列表维护收集的数据
    def __init__(self):
        self.datas = []

    #收集数据
    def collect_data(self, data):
        if data is None:
            return
        self.datas.append(data)


    #写入html文件
    def output_html(self):
        fout = open('output.html','w')
        fout.write("<html>")
        fout.write("<body>")
        fout.write("<table>")

        for data in self.datas:
            fout.write("<tr>")
            #输出单元格内容
            #fout.write("<td>%s</td>"% data['url'])
            fout.write("<td>%s</td>" % data['title'])

            for i in data['summary']:
                fout.write("<td>%s</td>" % i)

        fout.write("</table>")
        fout.write("</body>")
        fout.write("</html>")

        fout.close()
