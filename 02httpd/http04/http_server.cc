#include "http_server.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

namespace http_server{

int Start(const std::string& ip, short port){
    //创建TCP服务器
    int listen_sock = sock(AF_INET, SOCK_STREAM, 0);
    if(listen_sock < 0){
        perror("socket");
        return -1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    int ret =bind(listen_sock, (sockaddr*)&addr, sizeof(addr));
    if(ret < 0){
        perrro("bind");
        return -1;
    }

    ret = listen(listen_sock, 5);
    LOG(INFO)<<"ServerStart OK!\n";

    return 0;
}


}//end http_server

