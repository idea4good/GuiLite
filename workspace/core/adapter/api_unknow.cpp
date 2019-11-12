#if (!defined _WIN32) && (!defined WIN32) && (!defined _WIN64) && (!defined WIN64) && (!defined __linux__) && (!defined __APPLE__)

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

c_fifo::c_fifo()
{
	m_head = m_tail = 0;
	m_read_sem = m_write_mutex = 0;
}

int c_fifo::read(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	while(i < len)
	{
		if (m_tail == m_head)
		{//empty
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

	while(i < len)
	{
		if ((m_tail + 1) % FIFO_BUFFER_LEN == m_head)
		{//full, clear data has been written;
			m_tail = tail;
			log_out("Warning: fifo full\n");
			return 0;
		}
		m_buf[m_tail] = *pbuf++;
		m_tail = (m_tail + 1) % FIFO_BUFFER_LEN;
		i++;
	}

	if(i != len)
	{
		ASSERT(false);
	}
	return i;
}

#endif
