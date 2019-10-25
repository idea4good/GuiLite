#include "../../core_include/api.h"
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/times.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TIMER_CNT 10
#define TIMER_UNIT 50//ms

static void(*do_assert)(const char* file, int line);
static void(*do_log_out)(const char* log);
void register_debug_function(void(*my_assert)(const char* file, int line), void(*my_log_out)(const char* log))
{
	do_assert = my_assert;
	do_log_out = my_log_out;
}

void _assert(const char* file, int line)
{
	if(do_assert)
	{
		do_assert(file, line);
	}
	else
	{
		printf("assert@ file:%s, line:%d, error no: %d\n", file, line, errno);
	} 
}

void log_out(const char* log)
{
	if (do_log_out)
	{
		do_log_out(log);
	}
	else
	{
		printf("%s", log);
		fflush(stdout);
	}
}

typedef struct _timer_manage
{
    struct  _timer_info
    {
        int state; /* on or off */
        int interval;
        int elapse; /* 0~interval */
        void (* timer_proc) (void* ptmr, void* parg);
    }timer_info[MAX_TIMER_CNT];

    void (* old_sigfunc)(int);
    void (* new_sigfunc)(int);
}_timer_manage_t;
static struct _timer_manage timer_manage;

static void* timer_routine(void*)
{
    int i;
    while(true)
    {
    	for(i = 0; i < MAX_TIMER_CNT; i++)
		{
			if(timer_manage.timer_info[i].state == 0)
			{
				continue;
			}
			timer_manage.timer_info[i].elapse++;
			if(timer_manage.timer_info[i].elapse == timer_manage.timer_info[i].interval)
			{
				timer_manage.timer_info[i].elapse = 0;
				timer_manage.timer_info[i].timer_proc(0, 0);
			}
		}
    	usleep(1000 * TIMER_UNIT);
    }
    return NULL;
}

static int init_mul_timer()
{
	static bool s_is_init = false;
	if(s_is_init == true)
	{
		return 0;
	}
    memset(&timer_manage, 0, sizeof(struct _timer_manage));
    pthread_t pid;
    pthread_create(&pid, NULL, timer_routine, NULL);
    s_is_init = true;
    return 1;
}

static int set_a_timer(int interval, void (* timer_proc) (void* ptmr, void* parg))
{
	init_mul_timer();

	int i;
    if(timer_proc == NULL || interval <= 0)
    {
        return (-1);
    }

    for(i = 0; i < MAX_TIMER_CNT; i++)
    {
        if(timer_manage.timer_info[i].state == 1)
        {
            continue;
        }
        memset(&timer_manage.timer_info[i], 0, sizeof(timer_manage.timer_info[i]));
        timer_manage.timer_info[i].timer_proc = timer_proc;
        timer_manage.timer_info[i].interval = interval;
        timer_manage.timer_info[i].elapse = 0;
        timer_manage.timer_info[i].state = 1;
        break;
    }

    if(i >= MAX_TIMER_CNT)
    {
    	ASSERT(false);
        return (-1);
    }
    return (i);
}

typedef void (*EXPIRE_ROUTINE)(void* arg);
EXPIRE_ROUTINE s_expire_function;
static c_fifo s_real_timer_fifo;

static void* real_timer_routine(void*)
{
	char dummy;
	while(1)
	{
		if(s_real_timer_fifo.read(&dummy, 1) > 0)
		{
			if(s_expire_function)s_expire_function(0);
		}
		else
		{
			ASSERT(false);
		}
	}
	return 0;
}

static void expire_real_timer(int sigo)
{
	char dummy = 0x33;
	if(s_real_timer_fifo.write(&dummy, 1) <= 0)
	{
		ASSERT(false);
	}
}

void start_real_timer(void (*func)(void* arg))
{
	if(NULL == func)
	{
		return;
	}

	s_expire_function = func;
	signal(SIGALRM, expire_real_timer);
	struct itimerval value, ovalue;
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = REAL_TIME_TASK_CYCLE_MS * 1000;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = REAL_TIME_TASK_CYCLE_MS * 1000;
	setitimer(ITIMER_REAL, &value, &ovalue);

	static pthread_t s_pid;
	if(s_pid == 0)
	{
		pthread_create(&s_pid, NULL, real_timer_routine, NULL);
	}
}

unsigned int get_cur_thread_id()
{
	return (unsigned long)pthread_self();
}

void register_timer(int milli_second,void func(void* ptmr, void* parg))
{
	set_a_timer(milli_second/TIMER_UNIT,func);
}

long get_time_in_second()
{
	return time(NULL);         /* + 8*60*60*/
}

T_TIME get_time()
{
	T_TIME ret = {0};
	struct tm *fmt;
	time_t timer;

	timer = get_time_in_second();
	fmt = localtime(&timer);
	ret.year   = fmt->tm_year + 1900;
	ret.month  = fmt->tm_mon + 1;
	ret.day    = fmt->tm_mday;
	ret.hour   = fmt->tm_hour;
	ret.minute = fmt->tm_min;
	ret.second = fmt->tm_sec;
	return ret;
}

T_TIME second_to_day(long second)
{
	T_TIME ret = {0};
	struct tm *fmt;
	fmt = localtime(&second);
	ret.year   = fmt->tm_year + 1900;
	ret.month  = fmt->tm_mon + 1;
	ret.day    = fmt->tm_mday;
	ret.hour   = fmt->tm_hour;
	ret.minute = fmt->tm_min;
	ret.second = fmt->tm_sec;
	return ret;
}

void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg)
{
    pthread_create((pthread_t*)thread_id, (pthread_attr_t const*)attr, start_routine, arg);
}

void thread_sleep(unsigned int milli_seconds)
{
	usleep(milli_seconds * 1000);
}

typedef struct {
	unsigned short	bfType;
	unsigned int   	bfSize;
	unsigned short  bfReserved1;
	unsigned short  bfReserved2;
	unsigned int   	bfOffBits;
}__attribute__((packed))FileHead;

typedef struct{
	unsigned int  	biSize;
	int 			biWidth;
	int       		biHeight;
	unsigned short	biPlanes;
	unsigned short  biBitCount;
	unsigned int    biCompress;
	unsigned int    biSizeImage;
	int       		biXPelsPerMeter;
	int       		biYPelsPerMeter;
	unsigned int 	biClrUsed;
	unsigned int    biClrImportant;
	unsigned int 	biRedMask;
	unsigned int 	biGreenMask;
	unsigned int 	biBlueMask;
}__attribute__((packed))Infohead;

int build_bmp(const char *filename, unsigned int width, unsigned int height, unsigned char *data)
{
	FileHead bmp_head;
	Infohead bmp_info;
	int size = width * height * 2;

	//initialize bmp head.
	bmp_head.bfType = 0x4d42;
	bmp_head.bfSize = size + sizeof(FileHead) + sizeof(Infohead);
	bmp_head.bfReserved1 = bmp_head.bfReserved2 = 0;
	bmp_head.bfOffBits = bmp_head.bfSize - size;

	//initialize bmp info.
	bmp_info.biSize = 40;
	bmp_info.biWidth = width;
	bmp_info.biHeight = height;
	bmp_info.biPlanes = 1;
	bmp_info.biBitCount = 16;
	bmp_info.biCompress = 3;
	bmp_info.biSizeImage = size;
	bmp_info.biXPelsPerMeter = 0;
	bmp_info.biYPelsPerMeter = 0;
	bmp_info.biClrUsed = 0;
	bmp_info.biClrImportant = 0;

	//RGB565
	bmp_info.biRedMask = 0xF800;
	bmp_info.biGreenMask = 0x07E0;
	bmp_info.biBlueMask = 0x001F;

	//copy the data
	FILE *fp;
	if(!(fp=fopen(filename,"wb")))
	{
		return -1;
	}

	fwrite(&bmp_head, 1, sizeof(FileHead),fp);
	fwrite(&bmp_info, 1, sizeof(Infohead),fp);

	//fwrite(data, 1, size, fp);//top <-> bottom
	for (int i = (height - 1); i >= 0; --i)
	{
		fwrite(&data[i * width * 2], 1, width * 2, fp);
	}
	
	fclose(fp);
	return 0;
}

c_fifo::c_fifo()
{
	m_head = m_tail = 0;
	m_read_sem = malloc(sizeof(sem_t));
	m_write_mutex = malloc(sizeof(pthread_mutex_t));
	
	sem_init((sem_t*)m_read_sem, 0, 0);
	pthread_mutex_init((pthread_mutex_t*)m_write_mutex, 0);
}

int c_fifo::read(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	while(i < len)
	{
		if (m_tail == m_head)
		{//empty
			sem_wait((sem_t*)m_read_sem);
			continue;
		}
		*pbuf++ = m_buf[m_head];
		m_head = (m_head + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	if(i != len)
	{
		ASSERT(false);
	}
	return i;
}

int c_fifo::write(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	int tail = m_tail;

	pthread_mutex_lock((pthread_mutex_t*)m_write_mutex);
	while(i < len)
	{
		if ((m_tail + 1) % FIFO_BUFFER_LEN == m_head)
		{//full, clear data has been written;
			m_tail = tail;
			log_out("Warning: fifo full\n");
			pthread_mutex_unlock((pthread_mutex_t*)m_write_mutex);
			return 0;
		}
		m_buf[m_tail] = *pbuf++;
		m_tail = (m_tail + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	pthread_mutex_unlock((pthread_mutex_t*)m_write_mutex);

	if(i != len)
	{
		ASSERT(false);
	}
	else
	{
		sem_post((sem_t*)m_read_sem);
	}
	return i;
}
