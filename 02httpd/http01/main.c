#include "httpd.h"
#include <stdio.h>
#include <pthread.h>
#include "thread_pool.h"
static void usage(const char * proc)
{
    printf("usage: %s,[ip][prot]\n",proc);
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        usage(argv[0]);
        return 1;
    }
    int listen_sock = startup(argv[1],atoi(argv[2]));
    
    pool_init (3);/*线程池中最多3个活动线程*/
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


        /*连续向池中投入10个任务*/
        int *workingnum = (int *) malloc (sizeof (int) * 10);
        int i;
        for (i = 0; i < 10; i++)
        {
            workingnum[i] = i;
            pool_add_worker (hander_request, &workingnum[i]);
        }
        
        
    }
    //bu hui zhi xing
    close(listen_sock);

    pool_destroy();
    return 0;
}
