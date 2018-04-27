#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "chat_client.h"

namespace clinet{
//IP和PORT是通过命令行参数传递，如果优化则可以从配置文件中读取IP和端口号
//例如可以使用json来组织配置格式
//{
//  ip: "127.0.0.1"
//  port:9090
//}
int::ChatClinet(const std::string& ip,short port){
    sock_ = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_ < 0){
        perror("socket");
        return -1;
    }
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server_addr_.sin_port = htons(server_port);
    return 0;
}

int ChatClinet::SetUserINfo(const std::string& name, const std::string& school)
{
    name_ = name;
    school_ = school;
    return 0;
}

void ChatClinet::SendMsg(const std::string& msg){
    //按照下面这个方式来发送，不对，因为客户端和服务器交互的接口要严格
    //按照服务器提供的api来操作
    //这样的约定就相当于自定制了一个应用层协议
    // sendto(sock_,msg.c_str(),msg.size(),0,
    //      (sockaddr*)&server_addr_, sizeof(server_addr_));
          
    
    //数据的准备工作
    setver::Data data;
    data.name = name_;
    data.school = school_;
    data.msg = msg;
    //规定用户发送消息退出，相当于约定了用户输入什么样的内容表示要进行下线
    if(data.msg == "quit" || data.msg == "Q" || data.msg == "exit"){
        data.cmd = "quit";
    }
    std::string std;
    data.Serialize(&str);

    //正真的发送数据
    //发送的数据是Json序列化后的数据
    sendto(sock_, str.c_str(), str.size(), 0,
           (sockaddr*)&server_addr_, sizeof(server_addr_));
    return;
}

void ChatClinet::RecvMsg(server::Data* data){
    char buf[1024 * 10] = {0};
    //由于对端的IP端口号就是服务器的IP和端口号，所以此处没有必要再获取一遍对端的IP
    //这部分内容已经都知道了
    ssize_t read_size = recvfrom(sock_, buf, sizeof(buf)-1,
                                 0, NULL, NULL);
    if(read_size < 0)
    {
        perror("recvfrom");
        return -1;
    }
}

/////////////////////////////////////////
//
/////////////////////////////////
//b

}



