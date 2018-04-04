#include "httpd.h"
#include <stdio.h>
#include <pthread.h>
#include "thread_pool.h"
static void usage(const char * proc)
{
    printf("usage: %s,[ip][prot]\n",proc);
}
int startup(const char* ip, int port)
{
	assert(ip);

	int sock=socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		print_log("socket failed!  ",FATAL);
		return 1;
	}

	int opt=1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);
	socklen_t len = sizeof(local);

	if(bind(sock, (struct sockaddr*)&local, len) < 0)
	{
		print_log("bind_failed!  ",FATAL);
		return 2;
	}

	if(listen(sock,5) < 0)
	{
		print_log("listen failed!  ",FATAL);
		return 3;
	}
	return sock;
}

void* accept_request(void* arg)
{
	int sock = (int)arg;
	pthread_detach(pthread_self());
	return (void*)handle(sock);
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        return 1;
    }
    int listen_sock = startup(argv[1],atoi(argv[2]));
    
    while(1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);

        int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
        if(new_sock < 0)
        {
            print_log(strerror(errno),NOTICE);
            continue;
        }

        printf("get client[%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

        pthread_t tid;
        pthread_create(&tid, NULL, accept_request, (void*)new_sock);        
    }
    //bu hui zhi xing
    close(listen_sock);

    return 0;
}
