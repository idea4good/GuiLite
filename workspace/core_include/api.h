#ifndef GUILITE_CORE_INCLUDE_API_H
#define GUILITE_CORE_INCLUDE_API_H

#define REAL_TIME_TASK_CYCLE_MS		50
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#define GL_ARGB(a, r, g, b) ((((unsigned int)(a)) << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_ARGB_A(rgb) ((((unsigned int)(rgb)) >> 24) & 0xFF)

#define GL_RGB(r, g, b) ((0xFF << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_RGB_R(rgb) ((((unsigned int)(rgb)) >> 16) & 0xFF)
#define GL_RGB_G(rgb) ((((unsigned int)(rgb)) >> 8) & 0xFF)
#define GL_RGB_B(rgb) (((unsigned int)(rgb)) & 0xFF)
#define GL_RGB_32_to_16(rgb) (((((unsigned int)(rgb)) & 0xFF) >> 3) | ((((unsigned int)(rgb)) & 0xFC00) >> 5) | ((((unsigned int)(rgb)) & 0xF80000) >> 8))
#define GL_RGB_16_to_32(rgb) ((0xFF << 24) | ((((unsigned int)(rgb)) & 0x1F) << 3) | ((((unsigned int)(rgb)) & 0x7E0) << 5) | ((((unsigned int)(rgb)) & 0xF800) << 8))

#define ALIGN_HCENTER		0x00000000L
#define ALIGN_LEFT			0x01000000L
#define ALIGN_RIGHT			0x02000000L
#define ALIGN_HMASK			0x03000000L

#define ALIGN_VCENTER		0x00000000L
#define ALIGN_TOP			0x00100000L
#define ALIGN_BOTTOM		0x00200000L
#define ALIGN_VMASK			0x00300000L

typedef struct
{
	unsigned short year;
	unsigned short month;
	unsigned short date;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
}T_TIME;

void register_debug_function(void(*my_assert)(const char* file, int line), void(*my_log_out)(const char* log));
void _assert(const char* file, int line);
#define ASSERT(condition)	\
	do{                     \
	if(!(condition))_assert(__FILE__, __LINE__);\
	}while(0)
void log_out(const char* log);

long get_time_in_second();
T_TIME second_to_day(long second);
T_TIME get_time();

void start_real_timer(void (*func)(void* arg));
void register_timer(int milli_second, void func(void* ptmr, void* parg));

unsigned int get_cur_thread_id();
void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg);
void thread_sleep(unsigned int milli_seconds);
int build_bmp(const char *filename, unsigned int width, unsigned int height, unsigned char *data);

#define FIFO_BUFFER_LEN		1024
class c_fifo
{
public:
	c_fifo();
	int read(void* buf, int len);
	int write(void* buf, int len);
private:
	unsigned char 	m_buf[FIFO_BUFFER_LEN];
	int		m_head;
	int		m_tail;
	void* m_read_sem;
	void* m_write_mutex;
};

class c_rect
{
public:
	c_rect(){ m_left = m_top = m_right = m_bottom = -1; }
	c_rect(int left, int top, int width, int height)
	{
		set_rect(left, top, width, height);
	}
	void set_rect(int left, int top, int width, int height)
	{
		ASSERT(width > 0 && height > 0);
		m_left = left;
		m_top = top;
		m_right = left + width - 1;
		m_bottom = top + height -1;
	}
	bool pt_in_rect(int x, int y) const
	{
		return x >= m_left && x <= m_right && y >= m_top && y <= m_bottom;
	}
	int operator==(const c_rect& rect) const
	{
		return (m_left == rect.m_left) && (m_top == rect.m_top) && (m_right == rect.m_right) && (m_bottom == rect.m_bottom);
	}
	int width() const { return m_right - m_left + 1; }
	int height() const { return m_bottom - m_top + 1 ; }

	int	    m_left;
	int     m_top;
	int     m_right;
	int     m_bottom;
};
#endif
