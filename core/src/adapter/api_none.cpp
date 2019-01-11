#include "../../core_include/api.h"
#include <stdio.h>

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
	while(1);
}

void log_out(const char* log)
{
	if (do_log_out)
	{
		do_log_out(log);
	}
}

long get_time_in_second()
{
    return 0;
}

T_TIME second_to_day(long second)
{
    T_TIME ret = {0};
    return ret;
}

T_TIME get_time()
{
    T_TIME ret = {0};
    return ret;
}

void start_real_timer(void (*func)(void* arg))
{
    log_out("Not support now");
}

void register_timer(int milli_second, void func(void* ptmr, void* parg))
{
    log_out("Not support now");
}

unsigned int get_cur_thread_id()
{
    log_out("Not support now");
    return 0;
}

void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg)
{
    log_out("Not support now");
}

extern "C" void delay_ms(unsigned short nms);
void thread_sleep(unsigned int milli_seconds)
{//MCU alway implemnet driver code in APP.
		delay_ms(milli_seconds);
}

int build_bmp(const char *filename, unsigned int width, unsigned int height, unsigned char *data)
{
    log_out("Not support now");
    return 0;
}
