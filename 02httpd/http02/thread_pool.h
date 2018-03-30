#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
#include "httpd.h"
int pool_add_worker (void *(*process) (void *arg), void *arg);
void *thread_routine (void *arg);
void pool_init (int max_thread_num);
int pool_add_worker (void *(*process) (void *arg), void *arg);
int pool_destroy();
void* thread_routine (void *arg);
void* myprocess (void *arg);
