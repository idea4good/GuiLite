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

#define GL_ARGB(a, r, g, b) ((((unsigned int)(a)) << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_ARGB_A(rgb) ((((unsigned int)(rgb)) >> 24) & 0xFF)

#define GL_RGB(r, g, b) ((0xFF << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_RGB_R(rgb) ((((unsigned int)(rgb)) >> 16) & 0xFF)
#define GL_RGB_G(rgb) ((((unsigned int)(rgb)) >> 8) & 0xFF)
#define GL_RGB_B(rgb) (((unsigned int)(rgb)) & 0xFF)
#define GL_RGB_32_to_16(rgb) (((((unsigned int)(rgb)) & 0xFF) >> 3) | ((((unsigned int)(rgb)) & 0xFC00) >> 5) | ((((unsigned int)(rgb)) & 0xF80000) >> 8))
#define GL_RGB_16_to_32(rgb) (((((unsigned int)(rgb)) & 0x1F) << 3) | ((((unsigned int)(rgb)) & 0x7E0) << 5) | ((((unsigned int)(rgb)) & 0xF800) << 8))

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
