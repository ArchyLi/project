#pragma once
//主要通过这个结构体实现多路复用的选择，本质上是用指针实现了多态
struct eventop
{
    const char *name;
    void* (*init)(struct event_base*);
    int (*add)(void *, struct event*);
    int (*del)(void *, struct event*);
    int (*dispatch)(struct event_base*,void *,struct timeval*);
    void (*dealloc)(struct event_base*,void *);
}

//事件
struct event_base
{
///指定某个eventop结构体，它决定了该event_base使用哪一种I/O复用技术
	//指向了全局变量static const struct eventop* eventops[]中的一个，里面包含了select\poll\epoll\kequeue等等若干全局实例对象
	const struct eventop *evsel;
	//实际上是eventop实例对象
	void *evbase;
	//总事件个数
	int event_count;		/* counts number of total events */
	//活跃事件个数
	int event_count_active;	/* counts number of active events */

	//处理完所有的事件之后退出
	int event_gotterm;		/* Set to terminate loop */
	//立刻退出
	int event_break;		/* Set to terminate loop immediately */

	//activequeues[priority]是一个链表，链表的每一个结点指向一个优先级为priority的就绪事件event
	/* active event management */
	struct event_list **activequeues;
	int nactivequeues;

	/* signal handling info */
	struct evsignal_info sig;//管理信号的结构体
	//事件链表，保存了所有注册事件的event指针
	struct event_list eventqueue;
	//事件时间
	struct timeval event_tv;
	//管理定时事件的最小堆
	struct min_heap timeheap;
	//同时间管理变量
	struct timeval tv_cache;
}
