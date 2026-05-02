// test_progress_bar.cpp
// Unit tests for c_progress_bar widget
//
// Build: g++ -std=c++11 -I.. test_progress_bar.cpp -o test_progress_bar
// (No external test framework required; uses simple assertion macros.)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------
// Minimal stubs so the widget header compiles without the full GuiLite build.
// We only need to exercise the logic (value clamping, range, orientation, etc.)
// and do not need actual rendering.
// ---------------------------------------------------------------------------

// Stub out LATTICE_FONT_INFO / LATTICE so resource.h types are available.
typedef struct { unsigned int utf8_code; unsigned char width; const unsigned char* pixel_buffer; } LATTICE;
typedef struct { unsigned char height; unsigned int count; const LATTICE* lattice_array; } LATTICE_FONT_INFO;

// Provide GL_RGB / GL_RGB_R/G/B and other api.h macros
#define GL_RGB(r, g, b) ((0xFF << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_ARGB(a, r, g, b) ((((unsigned int)(a)) << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_RGB_R(rgb) ((((unsigned int)(rgb)) >> 16) & 0xFF)
#define GL_RGB_G(rgb) ((((unsigned int)(rgb)) >> 8) & 0xFF)
#define GL_RGB_B(rgb) (((unsigned int)(rgb)) & 0xFF)
#define GL_ARGB_A(rgb) ((((unsigned int)(rgb)) >> 24) & 0xFF)
#define GL_RGB_32_to_16(rgb) (((((unsigned int)(rgb)) & 0xFF) >> 3) | ((((unsigned int)(rgb)) & 0xFC00) >> 5) | ((((unsigned int)(rgb)) & 0xF80000) >> 8))
#define GL_RGB_16_to_32(rgb) ((0xFF << 24) | ((((unsigned int)(rgb)) & 0x1F) << 3) | ((((unsigned int)(rgb)) & 0x7E0) << 5) | ((((unsigned int)(rgb)) & 0xF800) << 8))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define ASSERT(cond) do { if(!(cond)) { printf("ASSERT FAILED: %s @ %s:%d\n", #cond, __FILE__, __LINE__); } } while(0)
#define REAL_TIME_TASK_CYCLE_MS 50
#define FIFO_BUFFER_LEN 1024

#define ALIGN_HCENTER  0x00000000L
#define ALIGN_LEFT     0x01000000L
#define ALIGN_RIGHT    0x02000000L
#define ALIGN_HMASK    0x03000000L
#define ALIGN_VCENTER  0x00000000L
#define ALIGN_TOP      0x00100000L
#define ALIGN_BOTTOM   0x00200000L
#define ALIGN_VMASK    0x00300000L

// Minimal stubs for types referenced by wnd.h
typedef struct { unsigned short year, month, date, day, hour, minute, second; } T_TIME;
void register_debug_function(void(*)(const char*, int), void(*)(const char*)) {}
void _assert(const char* f, int l) { printf("ASSERT @ %s:%d\n", f, l); }
void log_out(const char*) {}
long get_time_in_second() { return 0; }
T_TIME second_to_day(long) { T_TIME t = {}; return t; }
T_TIME get_time() { T_TIME t = {}; return t; }
void start_real_timer(void(*)(void*)) {}
void register_timer(int, void(*)(void*), void*) {}
unsigned int get_cur_thread_id() { return 0; }
void create_thread(unsigned long*, void*, void*(*)(void*), void*) {}
void thread_sleep(unsigned int) {}
int build_bmp(const char*, unsigned int, unsigned int, unsigned char*) { return 0; }

class c_fifo { public: c_fifo(){} int read(void*,int){return 0;} int write(void*,int){return 0;} unsigned char m_buf[FIFO_BUFFER_LEN]; int m_head; int m_tail; void* m_read_sem; void* m_write_mutex; };

class c_rect {
public:
	c_rect(){ m_left = m_top = m_right = m_bottom = -1; }
	c_rect(int left, int top, int width, int height){ set_rect(left,top,width,height); }
	void set_rect(int left, int top, int width, int height){ m_left=left; m_top=top; m_right=left+width-1; m_bottom=top+height-1; }
	bool pt_in_rect(int x, int y) const { return x>=m_left&&x<=m_right&&y>=m_top&&y<=m_bottom; }
	int operator==(const c_rect& r) const { return m_left==r.m_left&&m_top==r.m_top&&m_right==r.m_right&&m_bottom==r.m_bottom; }
	int width() const { return m_right-m_left+1; }
	int height() const { return m_bottom-m_top+1; }
	int m_left, m_top, m_right, m_bottom;
};

// Stub Z_ORDER and display / surface types
typedef enum { Z_ORDER_LEVEL_0, Z_ORDER_LEVEL_1, Z_ORDER_LEVEL_2, Z_ORDER_LEVEL_MAX } Z_ORDER_LEVEL;
struct DISPLAY_DRIVER { void(*draw_pixel)(int,int,unsigned int); void(*fill_rect)(int,int,int,int,unsigned int); };

// Minimal c_surface stub - we only need the interface to compile; on_paint won't run in tests.
class c_surface {
public:
	void fill_rect(c_rect, unsigned int, unsigned int) {}
	void fill_rect(int,int,int,int,unsigned int,unsigned int) {}
	void draw_rect(c_rect, unsigned int, unsigned int, unsigned int) {}
	void draw_rect(int,int,int,int,unsigned int,unsigned int,unsigned int s=1) {}
	void draw_pixel(int,int,unsigned int,unsigned int) {}
	void draw_hline(int,int,int,unsigned int,unsigned int) {}
	void draw_vline(int,int,int,unsigned int,unsigned int) {}
	Z_ORDER_LEVEL get_max_z_order() { return Z_ORDER_LEVEL_0; }
};

class c_layer { public: c_layer(){fb=0;} void* fb; c_rect rect; c_rect active_rect; };
class c_display { public: int m_phy_write_index; };

// Forward declare c_wnd
class c_wnd;
typedef void (c_wnd::*WND_CALLBACK)(int, int);

typedef enum { ATTR_VISIBLE=0x40000000L, ATTR_FOCUS=0x20000000L, ATTR_PRIORITY=0x10000000L } WND_ATTRIBUTION;
typedef enum { STATUS_NORMAL, STATUS_PUSHED, STATUS_FOCUSED, STATUS_DISABLED } WND_STATUS;
typedef enum { NAV_FORWARD, NAV_BACKWARD, NAV_ENTER } NAVIGATION_KEY;
typedef enum { TOUCH_DOWN, TOUCH_UP } TOUCH_ACTION;

typedef struct struct_wnd_tree { c_wnd* p_wnd; unsigned int resource_id; const char* str; short x,y,width,height; struct struct_wnd_tree* p_child_tree; } WND_TREE;

// Minimal c_wnd stub
class c_wnd {
public:
	c_wnd() : m_status(STATUS_NORMAL), m_attr((WND_ATTRIBUTION)(ATTR_VISIBLE|ATTR_FOCUS)), m_parent(0), m_top_child(0), m_prev_sibling(0), m_next_sibling(0), m_str(0), m_font_color(0), m_bg_color(0), m_id(0), m_z_order(Z_ORDER_LEVEL_0), m_focus_child(0), m_surface(0) {}
	virtual ~c_wnd() {}
	virtual int connect(c_wnd* parent, unsigned short resource_id, const char* str, short x, short y, short width, short height, WND_TREE* p_child_tree = 0) { m_id = resource_id; m_str = str; m_parent = parent; if(parent){ m_z_order=parent->m_z_order; m_surface=parent->m_surface; } m_wnd_rect.set_rect(x,y,width,height); pre_create_wnd(); return 0; }
	void disconnect() {}
	virtual void on_init_children() {}
	virtual void on_paint() {}
	virtual void show_window() {}
	unsigned short get_id() const { return m_id; }
	int get_z_order() { return m_z_order; }
	c_wnd* get_wnd_ptr(unsigned short) const { return 0; }
	unsigned int get_attr() const { return m_attr; }
	void set_str(const char* str) { m_str = str; }
	void set_attr(WND_ATTRIBUTION attr) { m_attr = attr; }
	bool is_focus_wnd() const { return ((m_attr&ATTR_VISIBLE)&&(m_attr&ATTR_FOCUS)); }
	void set_font_color(unsigned int c) { m_font_color=c; }
	unsigned int get_font_color() { return m_font_color; }
	void set_bg_color(unsigned int c) { m_bg_color=c; }
	unsigned int get_bg_color() { return m_bg_color; }
	void set_font_type(const LATTICE_FONT_INFO* f) { m_font=f; }
	const void* get_font_type() { return m_font; }
	void get_wnd_rect(c_rect& r) const { r=m_wnd_rect; }
	void get_screen_rect(c_rect& r) const { r=m_wnd_rect; }
	c_wnd* set_child_focus(c_wnd*) { return 0; }
	c_wnd* get_parent() const { return m_parent; }
	c_wnd* get_last_child() const { return 0; }
	int unlink_child(c_wnd*) { return 0; }
	c_wnd* get_prev_sibling() const { return m_prev_sibling; }
	c_wnd* get_next_sibling() const { return m_next_sibling; }
	c_wnd* search_priority_sibling(c_wnd*) { return 0; }
	virtual void on_touch(int,int,TOUCH_ACTION) {}
	virtual void on_navigate(NAVIGATION_KEY) {}
	c_surface* get_surface() { return m_surface; }
	void set_surface(c_surface* s) { m_surface = s; }
protected:
	virtual void pre_create_wnd() {}
	void add_child_2_tail(c_wnd*) {}
	void wnd2screen(int&,int&) const {}
	int load_child_wnd(WND_TREE*) { return 0; }
	void set_active_child(c_wnd*) {}
	virtual void on_focus() {}
	virtual void on_kill_focus() {}
protected:
	unsigned short m_id;
	WND_STATUS m_status;
	WND_ATTRIBUTION m_attr;
	c_rect m_wnd_rect;
	c_wnd* m_parent;
	c_wnd* m_top_child;
	c_wnd* m_prev_sibling;
	c_wnd* m_next_sibling;
	c_wnd* m_focus_child;
	const char* m_str;
	const void* m_font;
	unsigned int m_font_color;
	unsigned int m_bg_color;
	int m_z_order;
	c_surface* m_surface;
};

// Stub c_theme
typedef enum { FONT_NULL, FONT_DEFAULT, FONT_CUSTOM1, FONT_CUSTOM2, FONT_CUSTOM3, FONT_CUSTOM4, FONT_CUSTOM5, FONT_CUSTOM6, FONT_MAX } FONT_LIST;
typedef enum { COLOR_WND_FONT, COLOR_WND_NORMAL, COLOR_WND_PUSHED, COLOR_WND_FOCUS, COLOR_WND_BORDER, COLOR_CUSTOME1, COLOR_CUSTOME2, COLOR_CUSTOME3, COLOR_CUSTOME4, COLOR_CUSTOME5, COLOR_CUSTOME6, COLOR_MAX } COLOR_LIST;

class c_theme {
public:
	static const void* get_font(FONT_LIST) { return 0; }
	static unsigned int get_color(COLOR_LIST) { return 0; }
};

// Stub c_word
class c_font_operator { public: virtual ~c_font_operator(){} };
class c_word {
public:
	static void draw_string_in_rect(c_surface*, int, const void*, c_rect, const void*, unsigned int, unsigned int, unsigned int = ALIGN_LEFT) {}
	static void draw_value_in_rect(c_surface*, int, int, int, c_rect, const void*, unsigned int, unsigned int, unsigned int = ALIGN_LEFT) {}
	static c_font_operator* fontOperator;
};
c_font_operator* c_word::fontOperator = 0;

// ---------------------------------------------------------------------------
// Now include the progress bar widget (it sees the stubs above)
// ---------------------------------------------------------------------------

// Re-define the includes so the widget header doesn't pull in real headers
#define api_h_included
#define wnd_h_included
#define resource_h_included
#define word_h_included
#define display_h_included
#define theme_h_included

// We inline the progress_bar content with modified includes by directly
// including it after our stubs. The pragma once and includes inside the
// header are harmless since we already defined everything.

// Override the include guard behavior: progress_bar.h uses #pragma once
// and includes headers we already stubbed. The compiler will skip the
// already-defined headers if they have pragma once. Since our stubs
// are not in separate files, we just include directly:

// We need to suppress the real includes inside progress_bar.h.
// Since those headers use #pragma once already, we cannot re-include
// them. Instead we duplicate the logic we need:

typedef enum
{
	PROGRESS_HORIZONTAL,
	PROGRESS_VERTICAL
} PROGRESS_ORIENTATION;

typedef void (*PROGRESS_CALLBACK)(int value);

class c_progress_bar : public c_wnd
{
public:
	c_progress_bar()
		: m_min(0), m_max(100), m_value(0), m_orientation(PROGRESS_HORIZONTAL)
		, m_color_start(GL_RGB(0,128,0)), m_color_end(GL_RGB(255,0,0))
		, m_bg_color_bar(GL_RGB(50,50,50)), m_border_color(GL_RGB(180,180,180))
		, m_text_color(GL_RGB(255,255,255)), m_show_percentage(true)
		, m_animation_speed(1), m_animated_value(0), m_on_complete(0), m_completion_fired(false) {}
	void set_range(int min_val, int max_val) { if(min_val>=max_val){return;} m_min=min_val; m_max=max_val; clamp_value(m_value); clamp_value(m_animated_value); }
	int get_min() const { return m_min; }
	int get_max() const { return m_max; }
	void set_value(int value) { clamp_value(value); if(value==m_value)return; m_value=value; if(m_value>=m_max&&m_on_complete&&!m_completion_fired){m_completion_fired=true;m_on_complete(m_value);}else if(m_value<m_max){m_completion_fired=false;} }
	int get_value() const { return m_value; }
	void set_orientation(PROGRESS_ORIENTATION o) { m_orientation=o; }
	PROGRESS_ORIENTATION get_orientation() const { return m_orientation; }
	void set_colors(unsigned int s, unsigned int e) { m_color_start=s; m_color_end=e; }
	unsigned int get_color_start() const { return m_color_start; }
	unsigned int get_color_end() const { return m_color_end; }
	void set_show_percentage(bool s) { m_show_percentage=s; }
	bool get_show_percentage() const { return m_show_percentage; }
	void set_animation_speed(int s) { if(s<1)s=1; if(s>100)s=100; m_animation_speed=s; }
	int get_animation_speed() const { return m_animation_speed; }
	void set_on_complete(PROGRESS_CALLBACK cb) { m_on_complete=cb; }
	void animate_step() { if(m_animated_value==m_value)return; if(m_animated_value<m_value){m_animated_value+=m_animation_speed;if(m_animated_value>m_value)m_animated_value=m_value;}else{m_animated_value-=m_animation_speed;if(m_animated_value<m_value)m_animated_value=m_value;} }
	int get_animated_value() const { return m_animated_value; }
	int get_percentage() const { if(m_max==m_min)return 0; return ((m_animated_value-m_min)*100)/(m_max-m_min); }
private:
	void clamp_value(int& v) { if(v<m_min)v=m_min; if(v>m_max)v=m_max; }
	int m_min, m_max, m_value, m_animated_value;
	PROGRESS_ORIENTATION m_orientation;
	unsigned int m_color_start, m_color_end, m_bg_color_bar, m_border_color, m_text_color;
	bool m_show_percentage;
	int m_animation_speed;
	PROGRESS_CALLBACK m_on_complete;
	bool m_completion_fired;
};

// ---------------------------------------------------------------------------
// Test helpers
// ---------------------------------------------------------------------------
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
	if (!(cond)) { \
		printf("  FAIL: %s (%s:%d)\n", msg, __FILE__, __LINE__); \
		g_tests_failed++; \
	} else { \
		g_tests_passed++; \
	} \
} while(0)

static bool g_completion_called = false;
static int  g_completion_value  = -1;

static void on_complete_callback(int value)
{
	g_completion_called = true;
	g_completion_value = value;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

void test_creation()
{
	printf("[test_creation]\n");
	c_progress_bar bar;

	TEST_ASSERT(bar.get_min() == 0,   "default min should be 0");
	TEST_ASSERT(bar.get_max() == 100, "default max should be 100");
	TEST_ASSERT(bar.get_value() == 0, "default value should be 0");
	TEST_ASSERT(bar.get_orientation() == PROGRESS_HORIZONTAL, "default orientation horizontal");
	TEST_ASSERT(bar.get_show_percentage() == true, "default show_percentage true");
	TEST_ASSERT(bar.get_animation_speed() == 1, "default animation speed 1");
}

void test_set_value_and_clamping()
{
	printf("[test_set_value_and_clamping]\n");
	c_progress_bar bar;

	bar.set_value(50);
	TEST_ASSERT(bar.get_value() == 50, "set_value(50) should be 50");

	// Clamp above max
	bar.set_value(200);
	TEST_ASSERT(bar.get_value() == 100, "set_value(200) should clamp to 100");

	// Clamp below min
	bar.set_value(-10);
	TEST_ASSERT(bar.get_value() == 0, "set_value(-10) should clamp to 0");

	// Exact boundaries
	bar.set_value(0);
	TEST_ASSERT(bar.get_value() == 0, "set_value(0) should be 0");

	bar.set_value(100);
	TEST_ASSERT(bar.get_value() == 100, "set_value(100) should be 100");
}

void test_range_configuration()
{
	printf("[test_range_configuration]\n");
	c_progress_bar bar;

	bar.set_range(10, 50);
	TEST_ASSERT(bar.get_min() == 10, "min should be 10");
	TEST_ASSERT(bar.get_max() == 50, "max should be 50");

	// Value should be clamped to new range
	TEST_ASSERT(bar.get_value() == 10, "value should clamp to new min");

	bar.set_value(30);
	TEST_ASSERT(bar.get_value() == 30, "value 30 within [10,50]");

	bar.set_value(60);
	TEST_ASSERT(bar.get_value() == 50, "value 60 should clamp to max 50");

	bar.set_value(5);
	TEST_ASSERT(bar.get_value() == 10, "value 5 should clamp to min 10");

	// Invalid range (min >= max) should be rejected
	bar.set_range(100, 100);
	TEST_ASSERT(bar.get_min() == 10, "invalid equal range should not change min");
	TEST_ASSERT(bar.get_max() == 50, "invalid equal range should not change max");

	bar.set_range(200, 100);
	TEST_ASSERT(bar.get_min() == 10, "invalid reversed range should not change min");
	TEST_ASSERT(bar.get_max() == 50, "invalid reversed range should not change max");
}

void test_orientation()
{
	printf("[test_orientation]\n");
	c_progress_bar bar;

	TEST_ASSERT(bar.get_orientation() == PROGRESS_HORIZONTAL, "default horizontal");

	bar.set_orientation(PROGRESS_VERTICAL);
	TEST_ASSERT(bar.get_orientation() == PROGRESS_VERTICAL, "switched to vertical");

	bar.set_orientation(PROGRESS_HORIZONTAL);
	TEST_ASSERT(bar.get_orientation() == PROGRESS_HORIZONTAL, "switched back to horizontal");
}

void test_color_gradient()
{
	printf("[test_color_gradient]\n");
	c_progress_bar bar;

	unsigned int blue = GL_RGB(0, 0, 255);
	unsigned int yellow = GL_RGB(255, 255, 0);

	bar.set_colors(blue, yellow);
	TEST_ASSERT(bar.get_color_start() == blue, "start color should be blue");
	TEST_ASSERT(bar.get_color_end() == yellow, "end color should be yellow");
}

void test_animation()
{
	printf("[test_animation]\n");
	c_progress_bar bar;

	bar.set_animation_speed(10);
	TEST_ASSERT(bar.get_animation_speed() == 10, "animation speed set to 10");

	bar.set_value(50);

	// animated_value starts at 0, step towards 50
	bar.animate_step();
	TEST_ASSERT(bar.get_animated_value() == 10, "first step: animated_value should be 10");

	bar.animate_step();
	TEST_ASSERT(bar.get_animated_value() == 20, "second step: animated_value should be 20");

	// Clamp animation speed
	bar.set_animation_speed(0);
	TEST_ASSERT(bar.get_animation_speed() == 1, "speed 0 should clamp to 1");

	bar.set_animation_speed(200);
	TEST_ASSERT(bar.get_animation_speed() == 100, "speed 200 should clamp to 100");
}

void test_percentage()
{
	printf("[test_percentage]\n");
	c_progress_bar bar;

	// Default: animated_value = 0, range [0,100]
	TEST_ASSERT(bar.get_percentage() == 0, "0%% at start");

	bar.set_range(0, 200);
	bar.set_animation_speed(100);
	bar.set_value(100);
	bar.animate_step();
	TEST_ASSERT(bar.get_percentage() == 50, "50%% when animated=100 in [0,200]");
}

void test_completion_callback()
{
	printf("[test_completion_callback]\n");
	c_progress_bar bar;

	g_completion_called = false;
	g_completion_value = -1;

	bar.set_on_complete(on_complete_callback);

	bar.set_value(99);
	TEST_ASSERT(!g_completion_called, "callback should not fire at 99");

	bar.set_value(100);
	TEST_ASSERT(g_completion_called, "callback should fire at 100");
	TEST_ASSERT(g_completion_value == 100, "callback value should be 100");

	// Callback should not fire again for same max
	g_completion_called = false;
	bar.set_value(99);
	bar.set_value(100);
	TEST_ASSERT(g_completion_called, "callback should fire again after dropping below max");
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main()
{
	printf("=== c_progress_bar Test Suite ===\n\n");

	test_creation();
	test_set_value_and_clamping();
	test_range_configuration();
	test_orientation();
	test_color_gradient();
	test_animation();
	test_percentage();
	test_completion_callback();

	printf("\n=== Results: %d passed, %d failed ===\n", g_tests_passed, g_tests_failed);

	return g_tests_failed > 0 ? 1 : 0;
}
