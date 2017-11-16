#ifndef _API_H_
#define _API_H_

#define REAL_TIME_TASK_CYCLE_MS		50
#define XXX		-999
#define NULL 	0
#define TRUE 	1
#define FALSE 	0

void do_assert(const char* file, int line);
#define ASSERT(condition)	\
	do{                     \
	if(!(condition))do_assert(__FILE__, __LINE__);\
	}while(0)

void log_out(const char* log);

unsigned int GLT_RGB(unsigned char r, unsigned char g, unsigned char b);
unsigned char GLT_RGB_R(unsigned int color);
unsigned char GLT_RGB_G(unsigned int color);
unsigned char GLT_RGB_B(unsigned int color);

typedef struct _T_TIME
{
	unsigned short year;
	unsigned short month;
	unsigned short date;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
}T_TIME;

long get_time_in_second();
T_TIME second_to_day(long second);
T_TIME get_time();

void start_real_timer(void (*func)(void* arg));
void register_timer(int milli_second, void func(void* ptmr, void* parg));

unsigned int get_cur_thread_id();
void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg);
void thread_sleep(unsigned int milli_seconds);
int build_bmp(char *filename, unsigned int width, unsigned int height, unsigned char *data);
#endif
