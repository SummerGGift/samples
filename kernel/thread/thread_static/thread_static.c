/*
 * 程序清单：初始化静态线程
 *
 * 这个程序会初始化2个静态线程，它们拥有共同的入口函数，但参数不相同
 */
#include <rtthread.h>

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* 线程1控制块 */
static struct rt_thread thread1;
/* 线程1栈 */
ALIGN(4)
static rt_uint8_t thread1_stack[THREAD_STACK_SIZE];
/* 线程2控制块 */
static struct rt_thread thread2;
/* 线程2栈 */
ALIGN(4)
static rt_uint8_t thread2_stack[THREAD_STACK_SIZE];

/* 线程入口 */
static void thread_entry(void* parameter)
{
    rt_uint32_t count = 0;
    rt_uint32_t no = (rt_uint32_t) parameter; /* 获得正确的入口参数 */

    while (1)
    {
        /* 打印线程计数值输出 */
        rt_kprintf("thread%d count: %d\n", no, count ++);

        /* 休眠10个OS Tick */
        rt_thread_delay(10);
    }
}

/* 静态线程示例的初始化 */
int thread_static_init()
{
    rt_err_t result;

    /* 初始化线程1 */
    result = rt_thread_init(&thread1, "t1", /* 线程名：t1 */
                            thread_entry, (void*)1, /* 线程的入口是thread_entry，入口参数是1 */
                            &thread1_stack[0], sizeof(thread1_stack), /* 线程栈是thread1_stack */
                            THREAD_PRIORITY, 10);
    if (result == RT_EOK) /* 如果返回正确，启动线程1 */
        rt_thread_startup(&thread1);
    else
        return -1;

    /* 初始化线程2 */
    result = rt_thread_init(&thread2, "t2", /* 线程名：t2 */
                            thread_entry, (void*)2, /* 线程的入口是thread_entry，入口参数是2 */
                            &thread2_stack[0], sizeof(thread2_stack), /* 线程栈是thread2_stack */
                            THREAD_PRIORITY + 1, 10);
    if (result == RT_EOK) /* 如果返回正确，启动线程2 */
        rt_thread_startup(&thread2);
    else
        return -1;

    return 0;
}
/* 如果设置了RT_SAMPLES_AUTORUN，则加入到初始化线程中自动运行 */
#if defined (RT_SAMPLES_AUTORUN) && defined(RT_USING_COMPONENTS_INIT)
	INIT_APP_EXPORT(thread_static_init);
#endif
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_static_init, thread static init);
