#pragma once
#include <string>
#include <unordered_map>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdilb.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
namespace server{

class ChatServer{
public:
    int Start(cosnt std::string& ip,short port);

    //接受消息
    int RecvMsg();
    //广播消息
    int BroadCast();
private:
    static void* Consume(void*);//消费者线程
    static void* Product(void*);//生产者线程

    //key内容用户身份表示，IP+昵称作为哈希表的key
    //value是IP地址+端口号(struct sockaddr_in标示了IP地址和端口号)
    std::unordered_map<std::string,sockaddr_in> online_frind_list;//在线好友列表

    //TODO 实现一个队列作为交易场所,此处需要用一个阻塞队列来作为生产者和消费者模型的交易场所
    //暂定数据类型是std::string 
    BlockQueue<std::string> queue;
    int sock_t; //服务器进行绑定的文件描述符
}；

}//end server0 
