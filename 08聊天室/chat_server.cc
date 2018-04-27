#include <iostream>
#include "chat_server.h"
#include "api.hpp"
#include "../common/util.hpp"

namespace server{
int ChatServer::Start(const std::string& ip,short port){
    //启动服务器，并且进入事件循环
    //使用UDP作为我们的通讯协议
    sock = socket(AF_INET,SOCK_DGRAM,0);
    if(fd<0){
        perror("socket");
        return -1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addt(ip.C_str());
    addr.sin_port = htons(port);
    
    int ret = bind(sock,(sockaddr*)&addr,sizeof(addr));
    if(ret<0){
        perror("bind");
        return -1;
    }
    
    Log(INFO) << "Server Start OK!\n";
    
    pthread_t productor,comsumer;
    pthread_create(&productor, NULL, Product,this);//this当前对象,生产者线程
    pthread_create(&comsumer, NULL,Consume,this);//this当前对象,消费者线程
    pthread_join(productor,NULL);
    pthread_join(Consume,NULL);
    return 0;
}

void* ChatServer::Product(void* arg);//生产者线程
{
//生产者线程，主要做的事情就是读取socket中的数据，并且写入到BlockQueue
    ChatServer* server = reinterpret_cast<ChatServer*>(arg);
    while(true){
        //读取一次数据，并且写入BlockQueue中
        server->RecvMsg();
    }
    return NULL;
}
void* ChatServer::Consume(void*);//消费者线程
{
    ChatServer* server = reinterpret_cast<ChatServer*>(arg);
    while(true){
        //读取一次数据，并且写入BlockQueue中
        server->RecvMsg();
    }
    return NULL;
}

int ChatServer::RecvMsg(){
    //1、从socket中读取数据
    char buf[1024 * 5] = {0};
    sockaddr_in peer;
    socklen_t len = sizeof(peer);
    ssize_t read_size = recvfrom(sock, buf, sizeof(buf)-1,0,(socketaddr*)&peer, &len);
    
    if(read_size < 0){
        perrpr("recvfrom");
        return -1;
    }

    buf[read_size] = '0';
    
    //2、把数据插入到BlockQueue中
    Context context;
    context.str = buf;
    context.addr = peer;
    queue.Pushback(buf);
    return 0;
}

int ChatServer::BroadCast(){
    //1、从队列中读取一个元素
    Context context;
    queue.PopFront(&context);
    //2、对读取的字符串数据进行反序列化
    Data data;
    data.UnSerialize(context.str);
    //3、根据这个消息更新在线好友列表
    if(data.cmd == "quit"){
    // b)如果这个成员当前是一个下线消息（command是一个特殊的值），
    //  把这样的一个成员从好友列表中删除；
        //删除用户
        DelUser(context.addr, data.name);
    }else{
    // a)如果这个成员当前不再好友列表中，加入进来
    //准备使用[]方法来操作在线好友列表
        AddUser(context.addr, data.name);
    }
    //4、遍历在线好友列表，把这个数据依次发送给每一个客户端，
    // （由于发送消息的用户也存在于好友列表中，因此在遍历列表时也会给自己发送消息，从而达到发送者能够看到
    // 自己发送的消息的效果，但是这种实现方式比较蠢，完全可以控制客户端，不经过网络传输就实现这个功能，咋们此处还是用蠢方法来写
    // 更优越的方法交给自己来实现）
    for(auto it : online_frind_list){
        SendMsg(context.str, item)
    }
}

void ChatServer::DelUser(sockaddr_in addr, const std::string& name)
{
    std::stringstream ss;
    ss << name << ":" << addr.sin_addr.s_addr;
    online_frind_list.erase(ss.str());
    return;
}
void ChatServer::AddUser(sockaddr_in addr,const std::string& name)
{
    //key相当于IP地址和用户名拼接，如果光用IP地址可能会重复
    //如果N个客户端在同一个局域网中，那么服务器端看到的IP地址就是相同的，因为NAT
    std::stringstream ss;
    ss<< name << ":" << addr.sin_addr.s_addr;
    std::string key;
    socketaddr_in value = addr;
    //[] map unordered_map 
    //1、如果不存在，则插入
    //2、如果存在，则修改
    online_frind_list[key] = value;
}
void ChatServer::SendMsg(const std::string& data,sockaddr_in addr){
    //把这个数据通过sendto发送给客户端
    sendto(sock,data.data(),data.size(),0,(socketaddr*)addr,)
}

}//end server 
