
//此处定义了双向链表
#define TAILQ_ENTRY(type)\
struct {								\
	struct type *tqe_next;			\
	struct type **tqe_prev;		\
}
struct event{
    TAILQ_ENTRY (event) ev_IO_next;//
    TAILQ_ENTRY (event) ev_signal_next;
    TAILQ_ENTRY (event) ev_active_next;
    
    unsigned int min_heap_idx;

    struct event_base* ev_base;
}
