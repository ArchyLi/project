#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <sys/queue.h>

#include "event.h"

//此处是全局的变量
struct event_base* current_base = NULL;
//使用monotonic
static int use_monotonic;

static void check_monotonic()
{
//检测系统是否支持CLOCK_MONOTONIC来获取时间
#if defined(HAVE_CLOCK_GETTIME) && defined(CLOCK_MONOTONIC)
    struct timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        use_monotonic = 1;
#endif
}

static int gettime(struct event_base* base, struct timeval *tp)
{
    //如果存在tv_cache时间，那么就用，不存在就走下面
    if(base->tv_cache.tv_sec){
        *tp = base->tv_cache;
        return (0);
    }
//首先判断是否支持这个时间，支持就获取这个时间
#if defined(HAVE_CLOCK_GETTIME) && defined(CLOCK_MONOTONIC)
	if (use_monotonic) {
		struct timespec	ts;

		if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1)
			return (-1);

        //更新时间
		tp->tv_sec = ts.tv_sec;
		tp->tv_usec = ts.tv_nsec / 1000;
		return (0);
	}
#endif
    //如果都不支持，则获取当前系统时间
    //准确来说是调用gettimeofday()
    return (gettimeofday(tp, NULL));
}
struct event_base* event_init(void)
{
    struct event_base* base = event_base_new();
    if(base != NULL)
        current_base = base;
    return (base);
}

struct event_base* event_base_new(void)
{
    struct event_base* base;
    if(base = calloc(1,sizeof(event_base)) == NULL)
        perror("calloc_perror"),exit(1);

    //检查是否支持monotonic time
    check_monotonic();

    //获取时间
    gettime(base, &base->event_tv);

    //初始化定时事件定小堆
    min_heap_ctor(&base->timeheap);

    //初始化注册时间队列，头文件在sys/queue.h中
    TAILQ_INIT(&base->eventqueue);
    
    //
    base->sig.ev_signal_pair[0] = -1;
    base->sig.ev_signal_pair[1] = -1;
}
