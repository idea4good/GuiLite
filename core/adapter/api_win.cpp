#include "../../core_include/api.h"
#include "../../core_include/msg.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <assert.h>

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
	static char s_buf[192];
	if (do_assert) 
	{
		do_assert(file, line);
	}
	else
	{
		memset(s_buf, 0, sizeof(s_buf));
		sprintf_s(s_buf, sizeof(s_buf), "vvvvvvvvvvvvvvvvvvvvvvvvvvvv\n\nAssert@ file = %s, line = %d\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n", file, line);
		OutputDebugStringA(s_buf);
		printf(s_buf);
		fflush(stdout);
		assert(false);
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
		printf(log);
		fflush(stdout);
		OutputDebugStringA(log);
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

DWORD WINAPI timer_routine(LPVOID lpParam)
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
		Sleep(TIMER_UNIT);
    }
    return 0;
}

static int init_mul_timer()
{
	static bool s_is_init = false;
	if(s_is_init == true)
	{
		return 0;
	}
    memset(&timer_manage, 0, sizeof(struct _timer_manage));
    DWORD pid;
	CreateThread(0, 0, timer_routine, 0, 0, &pid);
    s_is_init = true;
    return 1;
}

static int set_a_timer(int interval, void (* timer_proc) (void* ptmr, void* parg))
{
	init_mul_timer();

	int i;
    if(timer_proc == 0 || interval <= 0)
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

static DWORD WINAPI fire_real_timer(LPVOID lpParam)
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

/*Win32 desktop only
static void CALLBACK trigger_real_timer(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)
{
	char dummy = 0x33;
	s_real_timer_fifo.write(&dummy, 1);
}
*/

static DWORD WINAPI trigger_real_timer(LPVOID lpParam)
{
	char dummy = 0x33;
	while (1)
	{
		s_real_timer_fifo.write(&dummy, 1);
		Sleep(REAL_TIME_TASK_CYCLE_MS);
	}
	return 0;
}

void start_real_timer(void (*func)(void* arg))
{
	if(0 == func)
	{
		return;
	}

	s_expire_function = func;
	//timeSetEvent(REAL_TIME_TASK_CYCLE_MS, 0, trigger_real_timer, 0, TIME_PERIODIC);//Win32 desktop only

	static DWORD s_pid;
	if(s_pid == 0)
	{
		CreateThread(0, 0, trigger_real_timer, 0, 0, &s_pid);
		CreateThread(0, 0, fire_real_timer, 0, 0, &s_pid);
	}
}

unsigned int get_cur_thread_id()
{
	return GetCurrentThreadId();
}

void register_timer(int milli_second,void func(void* ptmr, void* parg))
{
	set_a_timer(milli_second/TIMER_UNIT,func);
}

long get_time_in_second()
{
	return time(0);
}

T_TIME get_time()
{
	T_TIME ret = {0};
	
	SYSTEMTIME time;
	GetLocalTime(&time);
	ret.year = time.wYear;
	ret.month = time.wMonth;
	ret.day = time.wDay;
	ret.hour = time.wHour;
	ret.minute = time.wMinute;
	ret.second = time.wSecond;
	return ret;
}

T_TIME second_to_day(long second)
{
	T_TIME ret;
	ret.year = 1999;
	ret.month = 10;
	ret.date = 1;

	ret.second = second % 60;
	second /= 60;
	ret.minute = second % 60;
	second /= 60;
	ret.hour = (second + 8) % 24;//China time zone.
	return ret;
}

void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg)
{
	DWORD pid = 0;
	CreateThread(0, 0, LPTHREAD_START_ROUTINE(start_routine), arg, 0, &pid);
	*thread_id = pid;
}

void thread_sleep(unsigned int milli_seconds)
{
	Sleep(milli_seconds);
}

#pragma pack(push,1)
typedef struct {
	unsigned short	bfType;
	unsigned int   	bfSize;
	unsigned short  bfReserved1;
	unsigned short  bfReserved2;
	unsigned int   	bfOffBits;
}FileHead;

typedef struct {
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
}Infohead;
#pragma pack(pop)

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
	if (!(fp = fopen(filename, "wb")))
	{
		return -1;
	}

	fwrite(&bmp_head, 1, sizeof(FileHead), fp);
	fwrite(&bmp_info, 1, sizeof(Infohead), fp);

	//fwrite(data, 1, size, fp);//top <-> bottom
	for (int i = (height - 1); i >= 0; --i)
	{
		fwrite(&data[i * width * 2], 1, width * 2, fp);
	}

	fclose(fp);
	return 0;
}