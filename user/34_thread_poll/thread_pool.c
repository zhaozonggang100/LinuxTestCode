#include "thread_pool.h"

void handler(void *arg)
{
    printf("[%u] is ended.\n",
        (unsigned)pthread_self());

    //解锁！
    pthread_mutex_unlock((pthread_mutex_t *)arg);
}

void *routine(void *arg)
{
    //接住线程池的地址
    thread_pool *pool = (thread_pool *)arg;
    struct task *p;

    while(1)
    {
        //取消例程函数，将来线程上锁了，如果收到取消请求，那么先解锁，再退出
        pthread_cleanup_push(handler, (void *)&pool->lock);
        
        //任务队列是属于临界资源。
        //访问任务队列之前都必须上锁。
        pthread_mutex_lock(&pool->lock);
    
        //如果当前线程池未被关闭并且线程池中没有需要处理的任务时：
        while(pool->waiting_tasks == 0 && !pool->shutdown)
        {
            //那么就进入条件变量中等待！
            pthread_cond_wait(&pool->cond, &pool->lock);
        }
        
        //如果线程等待任务为0，并且线程池已经关闭了。
        if(pool->waiting_tasks == 0 && pool->shutdown == true)
        {    
            //解锁
            pthread_mutex_unlock(&pool->lock);    
            
            //走人
            pthread_exit(NULL); 
        }

        //有任务做，代表肯定不是空链表，拿任务p
        p = pool->task_list->next;
        pool->task_list->next = p->next;
        
        //当前等待的任务的个数-1
        pool->waiting_tasks--;

        //解锁
        pthread_mutex_unlock(&pool->lock);
        
        //删除线程取消例程函数
        pthread_cleanup_pop(0);
        
        //设置线程不可以响应取消。
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        
        //执行任务节点中函数过程中，不希望被别人取消掉。
        (p->do_task)(p->arg);
        
        //设置为可以响应取消
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

        //释放任务节点p的内存空间
        free(p);
    }

    pthread_exit(NULL);
}

bool init_pool(thread_pool *pool, unsigned int threads_number)
{
    //1. 初始化互斥锁
    pthread_mutex_init(&pool->lock, NULL);
    
    //2. 初始化条件变量
    pthread_cond_init(&pool->cond, NULL);

    //3. 线程池关闭标志为未关闭
    pool->shutdown = false;
    
    //4. 为任务队列头节点申请空间
    pool->task_list = malloc(sizeof(struct task));
    
    //5. 为线程TID号申请空间
    pool->tids = malloc(sizeof(pthread_t) * MAX_ACTIVE_THREADS);

    //错误判断
    if(pool->task_list == NULL || pool->tids == NULL)
    {
        perror("allocate memory error");
        return false;
    }

    //为任务队列头节点的指针域赋值NULL
    pool->task_list->next = NULL;
    
    //设置最大等待任务个数为1000
    pool->max_waiting_tasks = MAX_WAITING_TASKS;
    
    //设置当前等待任务的个数为0
    pool->waiting_tasks = 0;
    
    //设置当前线程池线程的个数
    pool->active_threads = threads_number;

    int i;
    //创建线程池中的子线程
    for(i=0; i<pool->active_threads; i++)
    {
        if(pthread_create(&((pool->tids)[i]), NULL,routine, (void *)pool) != 0)
        {
            perror("create threads error");
            return false;
        }
    }

    //初始化成功
    return true;
}

bool add_task(thread_pool *pool,void *(*do_task)(void *arg), void *arg)
{
    //为新节点申请内存空间
    struct task *new_task = malloc(sizeof(struct task));
    if(new_task == NULL)
    {
        perror("allocate memory error");
        return false;
    }
    
    //为新节点的数据域赋值
    new_task->do_task = do_task;  //函数
    new_task->arg = arg; //函数的参数
    
    //为新节点的指针域赋值
    new_task->next = NULL;  

    //访问任务队列前，先上锁！
    pthread_mutex_lock(&pool->lock);
    
    //如果当前等待任务个数>=1000，则添加任务失败！
    if(pool->waiting_tasks >= MAX_WAITING_TASKS)
    {
        //解锁
        pthread_mutex_unlock(&pool->lock);

        //输出错误信息
        fprintf(stderr, "too many tasks.\n");
        
        //释放刚刚初始化过的新节点
        free(new_task);

        return false;
    }
    
    //寻找任务队列的最后一个节点
    struct task *tmp = pool->task_list;
    while(tmp->next != NULL)
        tmp = tmp->next;
    
    //tmp->next = NULL;

    //把新节点尾插进去任务队列中
    tmp->next = new_task;
    
    //当前最大的等待任务的个数+1
    pool->waiting_tasks++;

    //解锁
    pthread_mutex_unlock(&pool->lock);

    //随机唤醒条件变量中其中一个线程起来就可以了。
    pthread_cond_signal(&pool->cond);
    
    return true;
}

int add_thread(thread_pool *pool, unsigned additional_threads)
{
    //如果新增0个线程
    if(additional_threads == 0)
        return 0; //直接返回0

    //添加后线程总数
    unsigned total_threads = 
            pool->active_threads + additional_threads;
                        
    int i, actual_increment = 0;
    
    //创建线程
    for(i = pool->active_threads;  
          i < total_threads && i < MAX_ACTIVE_THREADS;  
          i++) 
       {
        if(pthread_create(&((pool->tids)[i]),NULL, routine, (void *)pool) != 0)
        {
            perror("add threads error");
            if(actual_increment == 0) 
                return -1;

            break;
        }
        actual_increment++;  //真正创建的线程个数
    }

    //当前活跃的线程数 = 原来活跃的线程数 + 新实际创建的线程数
    pool->active_threads += actual_increment;
    
    return actual_increment;
}

     
int remove_thread(thread_pool *pool, unsigned int removing_threads)
{
    //如果需要删除0条线程
    if(removing_threads == 0)
        return pool->active_threads; //当前线程池活跃的线程个数

    //剩余的线程数 = 当前活跃的线程数 - 需要删除的线程。
    int remaining_threads = pool->active_threads - removing_threads;

    //线程池中至少有1条线程
    remaining_threads = remaining_threads > 0 ? remaining_threads : 1;

    int i;  
    for(i=pool->active_threads-1; i>remaining_threads-1; i--)
    {    
        errno = pthread_cancel(pool->tids[i]);
        if(errno != 0)
            break;
    }

    //如果取消失败，则函数返回-1
    if(i == pool->active_threads-1)
        return -1;
    else
    {
        //计算当前剩余实际的个数
        pool->active_threads = i+1;
        return i+1; //返回当前线程剩余的个数
    }
}


bool destroy_pool(thread_pool *pool)
{
    pool->shutdown = true; //当前线程池标志是关闭状态
    pthread_cond_broadcast(&pool->cond);
    int i;
    for(i=0; i<pool->active_threads; i++)
    {
        errno = pthread_join(pool->tids[i], NULL);

        if(errno != 0)
        {
            printf("join tids[%d] error: %s\n",
                    i, strerror(errno));
        }
    
        else
            printf("[%u] is joined\n", (unsigned)pool->tids[i]);
        
    }


    free(pool->task_list);
    free(pool->tids);
    free(pool);

    return true;
}