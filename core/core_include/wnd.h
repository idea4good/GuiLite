#ifndef WND_H
#define WND_H

//Window attribution
#define GL_ATTR_VISIBLE		0x80000000L
#define GL_ATTR_DISABLED	0x40000000L
#define GL_ATTR_FOCUS		0x20000000L

typedef struct struct_font_info		FONT_INFO;
typedef struct struct_color_rect	COLOR_RECT;

class c_wnd;
class c_surface;

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
	virtual const char* get_class_name() const { return "c_wnd"; }
	virtual int connect(c_wnd *parent, unsigned short resource_id, const char* str,
		short x, short y, short width, short height, WND_TREE* p_child_tree = NULL);
	virtual c_wnd* connect_clone(c_wnd *parent, unsigned short resource_id, const char* str,
		short x, short y, short width, short height, WND_TREE* p_child_tree = NULL);
	void disconnect();
	virtual c_wnd* clone() = 0;
	virtual void on_init_children() {}
	virtual void on_paint() {}
	virtual void show_window();

	unsigned short get_id() const { return m_resource_id; }
	int get_z_order() { return m_z_order; }
	c_wnd* get_wnd_ptr(unsigned short id) const;
	unsigned int get_style() const { return m_style; }
	virtual void modify_style(unsigned int add_style = 0, unsigned int remove_style = 0);

	void set_str(const char* str) { m_str = str; }
	int is_focus_wnd() const;

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

	void notify_parent(unsigned short msg_id, unsigned int w_param, long l_param);
	virtual int	on_notify(unsigned short notify_code, unsigned short ctrl_id, long l_param);

	virtual bool on_touch(int x, int y, TOUCH_ACTION action);
	virtual bool on_key(KEY_TYPE key);

	c_surface* get_surface() { return m_surface; }
	void set_surface(c_surface* surface) { m_surface = surface; }
protected:
	virtual void pre_create_wnd() {};
	void add_child_2_tail(c_wnd *child);

	void wnd2screen(int &x, int &y) const;
	void wnd2screen(c_rect &rect) const;
	void screen2wnd(short &x, short &y) const;
	void screen2wnd(c_rect &rect) const;

	int load_child_wnd(WND_TREE *p_child_tree);
	int load_clone_child_wnd(WND_TREE *p_child_tree);
	void set_active_child(c_wnd* child) { m_focus_child = child; }

	virtual void on_focus() {};
	virtual void on_kill_focus() {};
protected:
	WND_STATUS		m_status;
	unsigned int	m_style;
	c_rect			m_wnd_rect;//position relative to parent wnd.
	c_wnd*			m_parent;
	c_wnd*			m_top_child;
	c_wnd*			m_prev_sibling;
	c_wnd*			m_next_sibling;
	const char*		m_str;

	const FONT_INFO*	m_font_type;
	unsigned int		m_font_color;
	unsigned int		m_bg_color;

	unsigned short		m_resource_id;

	int					m_z_order;
	c_wnd*				m_focus_child;//current focused wnd
	c_surface*			m_surface;
private:
	c_wnd(const c_wnd &win);
	c_wnd& operator=(const c_wnd &win);
};
#endif
