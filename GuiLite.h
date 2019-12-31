#ifndef GUILITE_CORE_INCLUDE_API_H
#define GUILITE_CORE_INCLUDE_API_H

#define REAL_TIME_TASK_CYCLE_MS		50

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
#define ALIGN_TOP				0x00100000L
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
#endif
#ifndef GUILITE_CORE_INCLUDE_CMD_TARGET_H
#define GUILITE_CORE_INCLUDE_CMD_TARGET_H
class c_cmd_target;
#define MSG_TYPE_INVALID	0xFFFF
#define MSG_TYPE_WND		0x0001
#define MSG_TYPE_USR		0x0002
#define USR_MSG_MAX			32
typedef void (c_cmd_target::*msgCallback)(int, int);
struct GL_MSG_ENTRY
{
	unsigned int		msgType;
	unsigned int		msgId;
	c_cmd_target*		object;
	msgCallback			callBack;
};
#define ON_GL_USER_MSG(msgId, func)                    \
{MSG_TYPE_USR, msgId, 0, msgCallback(&func)},
#define GL_DECLARE_MESSAGE_MAP()						\
protected:												\
	virtual const GL_MSG_ENTRY* get_msg_entries() const;\
private:                                                \
	static const GL_MSG_ENTRY m_msg_entries[];
#define GL_BEGIN_MESSAGE_MAP(theClass)					\
const GL_MSG_ENTRY* theClass::get_msg_entries() const	\
{														\
	return theClass::m_msg_entries;						\
}														\
const GL_MSG_ENTRY theClass::m_msg_entries[] =     		\
{
#define GL_END_MESSAGE_MAP()                           \
{MSG_TYPE_INVALID, 0, 0, 0}};
class c_cmd_target
{
public:
	static int handle_usr_msg(int msg_id, int resource_id, int param);
protected:
	void load_cmd_msg();
	const GL_MSG_ENTRY* find_msg_entry(const GL_MSG_ENTRY *pEntry, int msgType, int msgId);
private:
	static GL_MSG_ENTRY ms_usr_map_entries[USR_MSG_MAX];
	static unsigned short ms_user_map_size;
	GL_DECLARE_MESSAGE_MAP()
};
#endif
#ifndef GUILITE_CORE_INCLUDE_RECT_H
#define GUILITE_CORE_INCLUDE_RECT_H
class c_rect
{
public:
	c_rect(){Empty();}
	c_rect(int left, int top, int right, int bottom){m_left = left;m_top = top;m_right = right;m_bottom = bottom;};
	void SetRect( int left, int top, int right, int bottom);
	c_rect(const c_rect&);
	c_rect& operator=(const c_rect&);
	void Empty();
	void Offset(int x, int y);
	int IsEmpty() const ;
	bool PtInRect(int x, int y) const ;
	int operator==(const c_rect& ) const;
	c_rect  operator&(const c_rect& aRect) const;
	int Width() const   {return m_right - m_left + 1;}
	int Height() const {return m_bottom - m_top + 1;}
	int	    m_left;
	int     m_top;
	int     m_right;
	int     m_bottom;
};
#endif
#ifndef  GUILITE_CORE_INCLUDE_RESOURCE_H
#define  GUILITE_CORE_INCLUDE_RESOURCE_H
//BITMAP
typedef struct struct_bitmap_info
{
	unsigned short width;
	unsigned short height;
	unsigned short color_bits;//support 16 bits only
	const unsigned short* pixel_color_array;
} BITMAP_INFO;
//FONT
typedef struct struct_lattice
{
	unsigned int			utf8_code;
	unsigned char			width;
	const unsigned char*	pixel_gray_array;
} LATTICE;
typedef struct struct_font_info
{
	unsigned char	height;
	unsigned int	count;
	LATTICE*		lattice_array;
} FONT_INFO;
#endif
#ifndef GUILITE_CORE_INCLUDE_THEME_H
#define GUILITE_CORE_INCLUDE_THEME_H
typedef struct struct_font_info		FONT_INFO;
typedef struct struct_color_rect	COLOR_RECT;
typedef struct struct_bitmap_info	BITMAP_INFO;
//Rebuild gui library once you change this file
enum FONT_TYPE
{
	FONT_NULL,
	FONT_DEFAULT,
	FONT_CUSTOM1,
	FONT_CUSTOM2,
	FONT_CUSTOM3,
	FONT_CUSTOM4,
	FONT_CUSTOM5,
	FONT_CUSTOM6,
	FONT_MAX
};
enum BITMAP_TYPE
{
	BITMAP_CUSTOM1,
	BITMAP_CUSTOM2,
	BITMAP_CUSTOM3,
	BITMAP_CUSTOM4,
	BITMAP_CUSTOM5,
	BITMAP_CUSTOM6,
	BITMAP_MAX
};
enum COLOR_TYPE
{
	COLOR_WND_FONT,
	COLOR_WND_NORMAL,
	COLOR_WND_PUSHED,
	COLOR_WND_FOCUS,
	COLOR_WND_BORDER,
	COLOR_CUSTOME1,
	COLOR_CUSTOME2,
	COLOR_CUSTOME3,
	COLOR_CUSTOME4,
	COLOR_CUSTOME5,
	COLOR_CUSTOME6,
	COLOR_MAX
};
class c_theme
{
public:
	static int add_font(FONT_TYPE index, const FONT_INFO* font);
	static const FONT_INFO* get_font(FONT_TYPE index);
	static int add_bitmap(BITMAP_TYPE index, const BITMAP_INFO* bmp);
	static const BITMAP_INFO* get_bmp(BITMAP_TYPE index);
	static int add_color(COLOR_TYPE index, const unsigned int color);
	static const unsigned int get_color(COLOR_TYPE index);
};
#endif
#ifndef GUILITE_CORE_INCLUDE_SURFACE_H
#define GUILITE_CORE_INCLUDE_SURFACE_H
class c_frame_layer
{
public:
	c_frame_layer() { fb = 0;}
	unsigned short*	fb;
	c_rect 	visible_rect;
};
typedef enum
{
	Z_ORDER_LEVEL_0,//view/wave/page
	Z_ORDER_LEVEL_1,//dialog
	Z_ORDER_LEVEL_2,//editbox/spinbox/listbox/keyboard
	Z_ORDER_LEVEL_MAX
}Z_ORDER_LEVEL;
struct EXTERNAL_GFX_OP
{
	void(*draw_pixel)(int x, int y, unsigned int rgb);
	void(*fill_rect)(int x0, int y0, int x1, int y1, unsigned int rgb);
};
class c_display;
class c_surface {
	friend class c_display; friend class c_bitmap;
public:
	int get_width() { return m_width; }
	int get_height() { return m_height; }
	unsigned int get_pixel(int x, int y, unsigned int z_order);
	void draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order);
	void fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
	void draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order);
	void draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order);
	void draw_line(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
	void draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order, unsigned int size = 1);
	inline void draw_rect(c_rect rect, unsigned int rgb, unsigned int size, unsigned int z_order)
	{
		draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order, size);
	}
	inline void fill_rect(c_rect rect, unsigned int rgb, unsigned int z_order)
	{
		fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order);
	}
	int flush_screen(int left, int top, int right, int bottom);
	bool is_valid(c_rect rect);
	bool is_active() { return m_is_active; }
	c_display* get_display() { return m_display; }
	int set_frame_layer_visible_rect(c_rect& rect, unsigned int z_order);
	void set_active(bool flag){m_is_active = flag;}
protected:
	virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb);
	virtual void draw_pixel_on_fb(int x, int y, unsigned int rgb);
	void set_surface(Z_ORDER_LEVEL max_z_order);
	c_surface(c_display* display, unsigned int width, unsigned int height, unsigned int color_bytes);
	int				m_width;		//in pixels
	int				m_height;		//in pixels
	int				m_color_bytes;	//16 bits, 32 bits only
	void* 			m_fb;			//Top frame buffer you could see
	c_frame_layer 	m_frame_layers[Z_ORDER_LEVEL_MAX];//Top layber fb always be 0
	bool			m_is_active;
	Z_ORDER_LEVEL	m_max_zorder;
	Z_ORDER_LEVEL	m_top_zorder;
	void*			m_phy_fb;
	int*			m_phy_write_index;
	c_display*		m_display;
};
class c_surface_no_fb : public c_surface {//No physical framebuffer, memory fb is 32 bits
	friend class c_display;
	c_surface_no_fb(c_display* display, unsigned int width, unsigned int height, unsigned int color_bytes, struct EXTERNAL_GFX_OP* gfx_op) :
		c_surface(display, width, height, color_bytes) {m_gfx_op = gfx_op;}
protected:
	virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb);
	virtual void draw_pixel_on_fb(int x, int y, unsigned int rgb);
	struct EXTERNAL_GFX_OP* m_gfx_op;//Rendering by external method
};
#endif
#ifndef GUILITE_CORE_INCLUDE_DISPLAY_H
#define GUILITE_CORE_INCLUDE_DISPLAY_H
#define SURFACE_CNT_MAX	6//root + pages
class c_hid_pipe;
class c_surface;
class c_display {
	friend class c_surface;
public:
	c_display(void* phy_fb, int display_width, int display_height, int surface_width, int surface_height, unsigned int color_bytes, int surface_cnt, EXTERNAL_GFX_OP* gfx_op = 0);
	c_surface* alloc_surface(Z_ORDER_LEVEL max_zorder);
	int swipe_surface(c_surface* s0, c_surface* s1, int x0, int x1, int y0, int y2, int offset);
	int get_width() { return m_width; }
	int get_height() { return m_height; }
	void* get_updated_fb(int* width, int* height, bool force_update = false);
	int snap_shot(const char* file_name);
private:
	int				m_width;		//in pixels
	int				m_height;		//in pixels
	int				m_color_bytes;	//16 bits, 32 bits only
	void*			m_phy_fb;
	int				m_phy_read_index;
	int				m_phy_write_index;
	c_surface* 		m_surface_group[SURFACE_CNT_MAX];
	int				m_surface_cnt;
	int				m_surface_index;
};
#endif
#ifndef GUILITE_CORE_INCLUDE_WORD_H
#define GUILITE_CORE_INCLUDE_WORD_H
class c_surface;
class c_word
{
public:
	static void draw_string(c_surface* surface, int z_order, const char *s, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void draw_string_in_rect(c_surface* surface, int z_order, const char *s, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void draw_value(c_surface* surface, int z_order, int value, int dot_position, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void draw_value_in_rect(c_surface* surface, int z_order, int value, int dot_position, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void value_2_string(int value, int dot_position, char* buf, int len);
	static int get_str_size(const char *s, const FONT_INFO* font, int& width, int& height);
private:
	static int draw_single_char(c_surface* surface, int z_order, unsigned int utf8_code, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color);
	static void draw_lattice(c_surface* surface, int z_order, int x, int y, int width, int height, const unsigned char* p_data, unsigned int font_color, unsigned int bg_color);
	
	static const LATTICE* get_lattice(const FONT_INFO* font, unsigned int utf8_code);
	static void get_string_pos(const char *s, const FONT_INFO* font, c_rect rect, unsigned int align_type, int &x, int &y);
};
#endif
#ifndef GUILITE_CORE_INCLUDE_BITMAP_H
#define GUILITE_CORE_INCLUDE_BITMAP_H
#define	DEFAULT_MASK_COLOR 0xFF080408
class c_surface;
class c_bitmap
{
public:
	static void draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, int x, int y, unsigned int mask_rgb = DEFAULT_MASK_COLOR);
	static void draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO* pBitmap, int x, int y, int src_x, int src_y, int width, int height, unsigned int mask_rgb = DEFAULT_MASK_COLOR);
};
#endif
#ifndef GUILITE_CORE_INCLUDE_WND_H
#define GUILITE_CORE_INCLUDE_WND_H
typedef struct struct_font_info		FONT_INFO;
typedef struct struct_color_rect	COLOR_RECT;
class c_wnd;
class c_surface;
typedef enum
{
	ATTR_VISIBLE	= 0x40000000L,
	ATTR_FOCUS		= 0x20000000L,
	ATTR_PRIORITY	= 0x10000000L// Handle touch action at high priority
}WND_ATTRIBUTION;
typedef enum
{
	STATUS_NORMAL,
	STATUS_PUSHED,
	STATUS_FOCUSED,
	STATUS_DISABLED
}WND_STATUS;
typedef enum
{
	KEY_FORWARD,
	KEY_BACKWARD,
	KEY_ENTER
}KEY_TYPE;
typedef enum
{
	TOUCH_DOWN,
	TOUCH_UP
}TOUCH_ACTION;
typedef struct struct_wnd_tree
{
	c_wnd*					p_wnd;
	unsigned int			resource_id;
	const char*				str;
	short   				x;
	short   				y;
	short   				width;
	short        			height;
	struct struct_wnd_tree*	p_child_tree;
}WND_TREE;
class c_wnd : public c_cmd_target
{
	friend class c_dialog;
public:
	c_wnd();
	virtual ~c_wnd() {};
	virtual int connect(c_wnd *parent, unsigned short resource_id, const char* str,
		short x, short y, short width, short height, WND_TREE* p_child_tree = 0);
	void disconnect();
	virtual void on_init_children() {}
	virtual void on_paint() {}
	virtual void show_window();
	unsigned short get_id() const { return m_id; }
	int get_z_order() { return m_z_order; }
	c_wnd* get_wnd_ptr(unsigned short id) const;
	unsigned int get_attr() const { return m_attr; }
	void set_str(const char* str) { m_str = str; }
	void set_attr(WND_ATTRIBUTION attr) { m_attr = attr; }
	bool is_focus_wnd() const;
	void set_font_color(unsigned int color) { m_font_color = color; }
	unsigned int get_font_color() { return m_font_color; }
	void set_bg_color(unsigned int color) { m_bg_color = color; }
	unsigned int get_bg_color() { return m_bg_color; }
	void set_font_type(const FONT_INFO *font_type) { m_font_type = font_type; }
	const FONT_INFO* get_font_type() { return m_font_type; }
	void set_wnd_pos(short x, short y, short width, short height);
	void get_wnd_rect(c_rect &rect) const;
	void get_screen_rect(c_rect &rect) const;
	c_wnd* set_child_focus(c_wnd *focus_child);
	c_wnd* get_parent() const { return m_parent; }
	c_wnd* get_last_child() const;
	int	unlink_child(c_wnd *child);
	c_wnd* get_prev_sibling() const { return m_prev_sibling; }
	c_wnd* get_next_sibling() const { return m_next_sibling; }
	void notify_parent(int msg_id, int param);
	virtual void on_touch(int x, int y, TOUCH_ACTION action);
	virtual void on_key(KEY_TYPE key);
	c_surface* get_surface() { return m_surface; }
	void set_surface(c_surface* surface) { m_surface = surface; }
protected:
	virtual void pre_create_wnd() {};
	void add_child_2_tail(c_wnd *child);
	void wnd2screen(int &x, int &y) const;
	void wnd2screen(c_rect &rect) const;
	int load_child_wnd(WND_TREE *p_child_tree);
	void set_active_child(c_wnd* child) { m_focus_child = child; }
	virtual void on_focus() {};
	virtual void on_kill_focus() {};
protected:
	WND_STATUS		m_status;
	WND_ATTRIBUTION	m_attr;
	c_rect			m_wnd_rect;// position relative to parent wnd.
	c_wnd*			m_parent;
	c_wnd*			m_top_child;
	c_wnd*			m_prev_sibling;
	c_wnd*			m_next_sibling;
	const char*		m_str;
	const FONT_INFO*	m_font_type;
	unsigned int		m_font_color;
	unsigned int		m_bg_color;
	unsigned short		m_id;
	int					m_z_order;
	c_wnd*				m_focus_child;//current focused wnd
	c_surface*			m_surface;
private:
	c_wnd(const c_wnd &win);
	c_wnd& operator=(const c_wnd &win);
};
#endif
#ifndef GUILITE_CORE_INCLUDE_AUDIO_H
#define GUILITE_CORE_INCLUDE_AUDIO_H
enum AUDIO_TYPE
{
	AUDIO_HEART_BEAT,
	AUDIO_ALARM,
	AUDIO_MAX
};
class c_audio
{
public:
	static int play(AUDIO_TYPE type);
private:
	static void init();
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_BUTTON_H
#define GUILITE_WIDGETS_INCLUDE_BUTTON_H
#define GL_BN_CLICKED							0x1111
#define ON_GL_BN_CLICKED(func)                                       \
{MSG_TYPE_WND, GL_BN_CLICKED, 0, msgCallback(&func)},
typedef struct struct_bitmap_info BITMAP_INFO;
class c_button : public c_wnd
{
protected:
	virtual void on_paint();
	virtual void on_focus();
	virtual void on_kill_focus();
	virtual void pre_create_wnd();
	virtual void on_touch(int x, int y, TOUCH_ACTION action);
	virtual void on_key(KEY_TYPE key);
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_DIALOG_H
#define GUILITE_WIDGETS_INCLUDE_DIALOG_H
class c_surface;
class c_dialog;
typedef struct
{
	c_dialog* 	dialog;
	c_surface*	surface;
} DIALOG_ARRAY;
class c_dialog : public c_wnd
{
public:
	static int open_dialog(c_dialog* p_dlg, bool modal_mode = true);
	static int close_dialog(c_surface* surface);
	static c_dialog* get_the_dialog(c_surface* surface);
protected:
	virtual const char* get_class_name(void) const {return "c_dialog";}
	virtual void pre_create_wnd();
	virtual void on_paint();
	static DIALOG_ARRAY ms_the_dialogs[SURFACE_CNT_MAX];
private:
	int set_me_the_dialog();
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_KEYBOARD_H
#define GUILITE_WIDGETS_INCLUDE_KEYBOARD_H
#define KEYBORAD_CLICK			0x5014
#define ON_KEYBORAD_UPDATE(func)  \
{MSG_TYPE_WND, KEYBORAD_CLICK, 0,  msgCallback(&func)},
typedef enum
{
	STATUS_UPPERCASE,
	STATUS_LOWERCASE
}KEYBOARD_STATUS;
typedef enum
{
	STYLE_ALL_BOARD,
	STYLE_NUM_BOARD
}KEYBOARD_STYLE;
typedef enum
{
	CLICK_CHAR,
	CLICK_ENTER,
	CLICK_ESC
}CLICK_STATUS;
class c_keyboard: public c_wnd
{
public:
	virtual int connect(c_wnd *user, unsigned short resource_id, KEYBOARD_STYLE style);
	KEYBOARD_STATUS get_cap_status(){return m_cap_status;}
	char* get_str() { return m_str; }
protected:
	virtual void pre_create_wnd();
	virtual void on_paint();
	void on_key_clicked(int id, int param);
	void on_char_clicked(int id, int param);
	void on_del_clicked(int id, int param);
	void on_caps_clicked(int id, int param);
	void on_enter_clicked(int id, int param);
	void on_esc_clicked(int id, int param);
	GL_DECLARE_MESSAGE_MAP()
private:
	char m_str[32];
	int	 m_str_len;
	KEYBOARD_STATUS m_cap_status;
};
class c_keyboard_button : public c_button
{
protected:
	virtual void on_paint();
};
#endif /* KEYBOARD_H_ */
#ifndef GUILITE_WIDGETS_INCLUDE_EDIT_H
#define GUILITE_WIDGETS_INCLUDE_EDIT_H
#define MAX_EDIT_STRLEN		32
class c_edit : public c_wnd
{
	friend class c_keyboard;
public:
	const char* get_text(){return m_str;}
	void set_text(const char* str);
	void set_keyboard_style(KEYBOARD_STYLE kb_sytle) { m_kb_style = kb_sytle; }
	
protected:
	virtual void pre_create_wnd();
	virtual void on_paint();
	virtual void on_focus();
	virtual void on_kill_focus();
	virtual void on_key(KEY_TYPE key);
	virtual void on_touch(int x, int y, TOUCH_ACTION action);
	
	void on_key_board_click(int id, int param);
	GL_DECLARE_MESSAGE_MAP()
private:
	void show_keyboard();
	void on_touch_down(int x, int y);
	void on_touch_up(int x, int y);
	KEYBOARD_STYLE m_kb_style;
	char m_str_input[MAX_EDIT_STRLEN];
	char m_str[MAX_EDIT_STRLEN];
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_GESTURE_H
#define GUILITE_WIDGETS_INCLUDE_GESTURE_H
typedef enum{
	TOUCH_MOVE,
	TOUCH_IDLE
}TOUCH_STATE;
class c_slide_group;
class c_gesture{
public:
	c_gesture(c_slide_group* group);
	bool handle_swipe(int x, int y, TOUCH_ACTION action);
private:
	bool on_move(int x);
	bool on_swipe(int x);
	int swipe_left();
	int swipe_right();
	void move_left();
	void move_right();
	int m_down_x;
	int m_down_y;
	int m_move_x;
	int m_move_y;
	TOUCH_STATE m_state;
	c_slide_group* 	m_slide_group;
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_LABEL_H
#define GUILITE_WIDGETS_INCLUDE_LABEL_H
class c_label : public c_wnd
{
public:
	virtual void on_paint();
protected:
	virtual void pre_create_wnd();
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_LIST_BOX_H
#define GUILITE_WIDGETS_INCLUDE_LIST_BOX_H
#define MAX_ITEM_NUM			4
#define GL_LIST_CONFIRM			0x1
#define ON_LIST_CONFIRM(func) \
{MSG_TYPE_WND, GL_LIST_CONFIRM, 0, msgCallback(&func)},
class c_list_box : public c_wnd
{
public:
	int add_item(char* str);
	void clear_item();
	short get_item_count() { return m_item_total; }
	void  select_item(short index);
	
protected:
	virtual void pre_create_wnd();
	virtual void on_paint();
	virtual void on_focus();
	virtual void on_kill_focus();
	virtual void on_key(KEY_TYPE key);
	virtual void on_touch(int x, int y, TOUCH_ACTION action);
	
private:
	void update_list_size();
	void show_list();
	void on_touch_down(int x, int y);
	void on_touch_up(int x, int y);
	short			m_selected_item;
	short			m_item_total;
	char*			m_item_array[MAX_ITEM_NUM];
	c_rect			m_list_wnd_rect;	//rect relative to parent wnd.
	c_rect			m_list_screen_rect;	//rect relative to physical screen(frame buffer)
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_SLIDE_GROUP_H
#define GUILITE_WIDGETS_INCLUDE_SLIDE_GROUP_H
#define MAX_PAGES	5
class c_gesture;
class c_slide_group : public c_wnd {
public:
	c_slide_group();
	int set_active_slide(int index, bool is_redraw = true);
	c_wnd* get_slide(int index){return m_slides[index];}
	c_wnd* get_active_slide(){return m_slides[m_active_slide_index];}
	int get_active_slide_index(){return m_active_slide_index;}
	int add_slide(c_wnd* slide, unsigned short resource_id, short x, short y,
			short width, short height, WND_TREE* p_child_tree = 0,
			Z_ORDER_LEVEL max_zorder =  Z_ORDER_LEVEL_0);
	void disabel_all_slide();
	virtual void on_touch(int x, int y, TOUCH_ACTION action);
	virtual void on_key(KEY_TYPE key);
protected:
	c_wnd* m_slides[MAX_PAGES];
	int m_active_slide_index;
	c_gesture* m_gesture;
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_SPINBOX_H
#define GUILITE_WIDGETS_INCLUDE_SPINBOX_H
#define	GL_SPIN_CHANGE				0x3333
#define ON_SPIN_CHANGE(func) \
{MSG_TYPE_WND, GL_SPIN_CHANGE, 0, msgCallback(&func)},
class c_spin_box;
class c_spin_button : public c_button
{
	friend class c_spin_box;
	virtual void on_touch(int x, int y, TOUCH_ACTION action);
	c_spin_box* m_spin_box;
};
class c_spin_box : public c_wnd
{
	friend class c_spin_button;
public:
	short get_value() { return m_value; }
	void set_value(unsigned short value) { m_value = m_cur_value = value; }
	void set_max_min(short max, short min) { m_max = max; m_min = min; }
	void set_step(short step) { m_step = step; }
	short get_min() { return m_min; }
	short get_max() { return m_max; }
	short get_step() { return m_step; }
	void set_value_digit(short digit) { m_digit = digit; }
	short get_value_digit() { return m_digit; }
protected:
	virtual void on_paint();
	virtual void pre_create_wnd();
	void on_arrow_up_bt_click();
	void on_arrow_down_bt_click();
	short			m_cur_value;
	short			m_value;
	short			m_step;
	short			m_max;
	short			m_min;
	short			m_digit;
	c_spin_button  	m_bt_up;
	c_spin_button  	m_bt_down;
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_TABLE_H
#define GUILITE_WIDGETS_INCLUDE_TABLE_H
#define  MAX_COL_NUM  30
#define  MAX_ROW_NUM  30
class c_table: public c_wnd
{
public:
	void set_sheet_align(unsigned int align_type){ m_align_type = align_type;}
	void set_row_num(unsigned int row_num){ m_row_num = row_num;}
	void set_col_num(unsigned int col_num){ m_col_num = col_num;}
	void set_row_height(unsigned int height);
	void set_col_width(unsigned int width);
	int set_row_height(unsigned int index, unsigned int height);
	int set_col_width(unsigned int index, unsigned int width);
	void set_item(int row, int col, char* str, unsigned int color);
	unsigned int get_row_num(){ return m_row_num;}
	unsigned int get_col_num(){ return m_col_num;}
	c_rect get_item_rect(int row, int col);
protected:
	virtual void pre_create_wnd();
	void draw_item(int row, int col, const char* str, unsigned int color);
	unsigned int m_align_type;	
	unsigned int m_row_num;
	unsigned int m_col_num;
	unsigned int m_row_height[MAX_ROW_NUM];
	unsigned int m_col_width[MAX_COL_NUM];
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_WAVE_BUFFER_H
#define GUILITE_WIDGETS_INCLUDE_WAVE_BUFFER_H
#define WAVE_BUFFER_LEN	1024
#define WAVE_READ_CACHE_LEN	8
#define BUFFER_EMPTY	-1111
#define BUFFER_FULL		-2222;
class c_wave_buffer
{
public:
	c_wave_buffer();
	int write_wave_data(short data);
	int read_wave_data_by_frame(short &max, short &min, short frame_len, unsigned int sequence, short offset);
	void reset();
	void clear_data();
	short get_cnt();
private:
	int read_data();
	short m_wave_buf[WAVE_BUFFER_LEN];
	short m_head;
	short m_tail;
	int m_min_old;
	int m_max_old;
	int m_min_older;
	int m_max_older;
	int m_last_data;
	short 	m_read_cache_min[WAVE_READ_CACHE_LEN];
	short 	m_read_cache_mid[WAVE_READ_CACHE_LEN];
	short 	m_read_cache_max[WAVE_READ_CACHE_LEN];
	short	m_read_cache_sum;
	unsigned int m_refresh_sequence;
};
#endif
#ifndef GUILITE_WIDGETS_INCLUDE_WAVE_CTRL_H
#define GUILITE_WIDGETS_INCLUDE_WAVE_CTRL_H
typedef enum
{
	FILL_MODE,
	SCAN_MODE
}E_WAVE_DRAW_MODE;
class c_wave_buffer;
class c_wave_ctrl : public c_wnd
{
public:
	c_wave_ctrl();
	virtual void on_init_children();
	virtual void on_paint();
	void set_wave_name(char* wave_name){ m_wave_name = wave_name;}
	void set_wave_unit(char* wave_unit){ m_wave_unit = wave_unit;}
	void set_wave_name_font(const FONT_INFO* wave_name_font_type){ m_wave_name_font = wave_name_font_type;}
	void set_wave_unit_font(const FONT_INFO* wave_unit_font_type){ m_wave_unit_font = wave_unit_font_type;}
	void set_wave_name_color(unsigned int wave_name_color){ m_wave_name_color = wave_name_color;}
	void set_wave_unit_color(unsigned int wave_unit_color){ m_wave_unit_color = wave_unit_color;}
	void set_wave_color(unsigned int color){ m_wave_color = color;}
	void set_wave_in_out_rate(unsigned int data_rate, unsigned int refresh_rate);
	void set_wave_speed(unsigned int speed);
	void set_max_min(short max_data, short min_data);
	void set_wave(c_wave_buffer* wave){m_wave = wave;}
	c_wave_buffer* get_wave(){return m_wave;}
	void clear_data();
	bool is_data_enough();
	void refresh_wave(unsigned char frame);
	void clear_wave();
	
protected:
	void draw_smooth_vline(int y_min, int y_max, int mid, unsigned int rgb);
	void restore_background();
	void save_background();
	char* m_wave_name;
	char* m_wave_unit;
	const FONT_INFO* m_wave_name_font;
	const FONT_INFO* m_wave_unit_font;
	unsigned int m_wave_name_color;
	unsigned int m_wave_unit_color;
	unsigned int m_wave_color;
	unsigned int m_back_color;
	int m_wave_left;
	int m_wave_right;
	int m_wave_top;
	int m_wave_bottom;
	short m_max_data;
	short m_min_data;
	
private:
	c_wave_buffer*	m_wave;
	unsigned int*	m_bg_fb;			//background frame buffer, could be used to draw scale line.
	int 			m_wave_cursor;
	int 			m_wave_speed;		//pixels per refresh
	unsigned int	m_wave_data_rate;	//data sample rate
	unsigned int	m_wave_refresh_rate;//refresh cycle in millisecond
	unsigned char 	m_frame_len_map[64];
	unsigned char 	m_frame_len_map_index;
};
#endif
