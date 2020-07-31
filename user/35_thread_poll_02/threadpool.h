#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define DEFAULT_TIME 10                 /*10s检测一次*/
#define MIN_WAIT_TASK_NUM 10            /*如果queue_size > MIN_WAIT_TASK_NUM 添加新的线程到线程池*/ 
#define DEFAULT_THREAD_VARY 10          /*每次创建和销毁线程的个数*/
#define true 1
#define false 0

typedef struct
{
    void *(*function)(void *);          /*函数指针，回调函数*/
    void *arg;                          /*上面函数的参数*/
} threadpool_task_t;                    /*任务结构体*/

typedef struct threadpool_t
{
    pthread_mutex_t lock;               /*用于锁住当前这个结构体体taskpoll*/    
    pthread_mutex_t thread_counter;     /*记录忙状态线程个数*/
    pthread_cond_t queue_not_full;      /*当任务队列满时，添加任 务的线程阻塞，等待此条件变量*/
    pthread_cond_t queue_not_empty;     /*任务队列里不为空时，通知等待任务的线程*/
    pthread_t *threads;                 /*保存工作线程tid的数组*/
    pthread_t adjust_tid;               /*管理线程tid*/
    threadpool_task_t *task_queue;      /*任务队列*/
    int min_thr_num;                    /*线程组内默认最小线程数*/
    int max_thr_num;                    /*线程组内默认最大线程数*/
    int live_thr_num;                   /*当前存活线程个数*/
    int busy_thr_num;                   /*忙状态线程个数*/
    int wait_exit_thr_num;              /*要销毁的线程个数*/
    int queue_front;                    /*队头索引下标*/
    int queue_rear;                     /*队未索引下标*/
    int queue_size;                     /*队中元素个数*/
    int queue_max_size;                 /*队列中最大容纳个数*/
    int shutdown;                       /*线程池使用状态，true或false*/
}threadpool_t;

/**
 * @function void *threadpool_thread(void *threadpool)
 * @desc the worker thread
 * @param threadpool the pool which own the thread
 */
void *threadpool_thread(void *threadpool);
/**
 * @function void *adjust_thread(void *threadpool);
 * @desc manager thread
 * @param threadpool the threadpool
 */
void *adjust_thread(void *threadpool);
/**
 * check a thread is alive
 */
int is_thread_alive(pthread_t tid);

int threadpool_free(struct threadpool_t *pool);

struct threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);

int threadpool_add(struct threadpool_t *pool, void*(*function)(void *arg), void *arg);

int threadpool_destroy(struct threadpool_t *pool);

#endif