#pragma once
namespace clinet{

class ChatClinet{
public:
    //初始化,因为要知道是否构造成功，所以不用构造函数初始化，所以要用Init进行初始化
    ChatClinet(const std::string& ip,short port);
    //客户端初始化,主要是socket初始化
    int Init();
    //登录，所以要设置用户信息
    //让客户端启动的时候提示用户输入姓名和学校
    //从标准输入中获取
    int SetUserINfo(const std::string& name,const std::string& school);
    //发送数据
    void SendMsg(const std::string& msg);
    //本质上接收的内容也是Json格式的内容
    void RecvMsg();
private:
    //这个文件描述符用来保存和服务器交互的socket
    //通过Init函数进行创建，交给SendMSg RecvMsg直接进行处理
    int sock_;
    sockaddr_in server_addr_;
    //
    std::string name_;
    std::string school_;
};

}//end clent 
