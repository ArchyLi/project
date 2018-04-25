#pragma once
#include <vector>
#include <semaphore.h>

namespace server{

}
//我们约定[head,tail)
template<typename T>
class BlockQueue{
public:
    BlockQueue(size_t s = 1024)
        :data(s),
        head(0),
        tail(0),
        size(0)
    {
        sem_init(&sem_data,0,0);//第二个参数添0表示同进程内线程间通讯，非0表示进程同步
        sem_init(&sem_blank,0,s);//空格个数为S个
    }

    ~BlockQueue(){
        sem_destroy(&sem_data);
        sem_destroy(&sem_blank);
    }
    void PushBack(const T& value){
        //先申请资源，P操作，申请一个空白资源
        sem_wait(&sem_blank);
        data[tail++] = vale;
        //超过队尾置最前 
        if(tail >= data.size()){
            tail = 0;
        }
        ++size;
        //V操作，可用元素个数
        sem_post(&sem_data);
    }
    void PopFront(T* value){
        sem_wait(&sem_data);
        *value = data[head++];
        if(head >= data.size()){
            head = 0;
        }
        --size;
        //空白资源进行V操作
        sem_post(&sem_blank);
    }
private:
    ste::vector<T> data;//采用vector作为存储数据的介质，互斥锁加条件变量，或者信号量 
    sem_t sem_data;//用来同步
    sem_t sem_blank;//用来同步
        //用来互斥,但是由于此处只实现一个单生产者单消费者的一个Blockqueue所以可以不加互斥锁
    //sem_t sem_lock;
    
    //描述数组头尾长度
    size_t head;
    size_t tail;
    size_t size;
};
