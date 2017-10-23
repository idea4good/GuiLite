#ifndef XT_GUI_WND_H
#define XT_GUI_WND_H

//Window attribution
#define GLT_WIN_SHOW			0x00000001
#define GLT_WIN_HIDE			0x00000002

#define GLT_ATTR_VISIBLE		0x80000000L
#define GLT_ATTR_DISABLED		0x40000000L
#define GLT_ATTR_FOCUS			0x20000000L

typedef struct struct_gui_bitmap GUI_BITMAP;
typedef struct struct_gui_font	GUI_FONT;
typedef struct struct_custom_shape	CUSTOM_SHAPE;
typedef const GUI_FONT* (*PTR_FUNC_GET_FONT)();

class c_wnd;
class c_surface;

typedef enum
{
	STATUS_NORMAL,
	STATUS_PUSHED,
	STATUS_FOCUSED,
	STATUS_DISABLED
}WND_STATUS;

typedef struct struct_wnd_tree
{
	c_wnd*					p_wnd;
	unsigned int			resource_id;
	unsigned int			caption_id;
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
	virtual ~c_wnd();
	virtual const char* get_class_name() const { return "c_wnd"; }
	virtual int connect(c_wnd *parent, unsigned short resource_id, unsigned short str_id,
		short x, short y, short width, short height, WND_TREE* p_child_tree = NULL);
	virtual c_wnd* connect_clone(c_wnd *parent, unsigned short resource_id, unsigned short str_id,
		short x, short y, short width, short height, WND_TREE* p_child_tree = NULL);
	void disconnect();
	virtual c_wnd* clone() = 0;
	virtual void on_init_children() {}
	virtual void on_paint() {}
	void show_window(int show_type = GLT_WIN_SHOW);

	unsigned short get_id() const { return m_resource_id; }
	int get_z_order() { return m_z_order; }
	c_wnd* get_wnd_ptr(unsigned short id) const;
	unsigned int get_style() const { return m_style; }
	virtual void modify_style(unsigned int add_style = 0, unsigned int remove_style = 0);

	void set_str_id(unsigned short str_id) { m_str_id = str_id; }
	unsigned short get_str_id() const { return m_str_id; }

	void set_bitmap(const GUI_BITMAP *pBitmap) { m_bitmap = pBitmap; }
	void set_focus_bitmap(const GUI_BITMAP *pBitmap) { m_bitmap_focus = pBitmap; }
	void set_pushed_bitmap(const GUI_BITMAP *pBitmap) { m_bitmap_pushed = pBitmap; }
	void set_disable_bitmap(const GUI_BITMAP *pBitmap) { m_bitmap_disable = pBitmap; }

	bool is_visible() const { return m_is_visible_now; }
	bool is_foreground();
	void set_visible(bool visible);
	virtual void enable_wnd(int enable = TRUE);
	int is_wnd_enable() const;
	void enable_focus(int enable = TRUE);
	int is_focus_wnd() const;
	const int is_active_wnd() const;

	void set_font_color(unsigned int color) { m_font_color = color; }
	unsigned int get_font_color() { return m_font_color; }
	void set_bg_color(unsigned int color) { m_bg_color = color; }
	unsigned int get_bg_color() { return m_bg_color; }
	void set_font_type(const GUI_FONT *font_type) { m_font_type = font_type; }
	const GUI_FONT* get_font_type() { return m_font_type; }

	void set_wnd_pos(short x, short y, short width, short height);
	void get_wnd_rect(c_rect &rect) const;
	void get_screen_rect(c_rect &rect) const;

	c_wnd* set_focus(c_wnd *new_active_child);

	c_wnd* get_parent() const { return m_parent; }
	c_wnd* get_last_child() const;
	int	unlink_child(c_wnd *child);
	c_wnd* get_prev_sibling() const { return m_prev_sibling; }
	c_wnd* get_next_sibling() const { return m_next_sibling; }

	void notify_parent(unsigned short msg_id, unsigned int w_param, long l_param);
	virtual int	on_notify(unsigned short notify_code, unsigned short ctrl_id, long l_param);

	virtual void on_touch_up(int x, int y);
	virtual void on_touch_down(int x, int y);

	c_wnd* get_active_child() const { return m_active_child; }

	void modify_status(WND_STATUS status) { m_status = status; }
	WND_STATUS get_status() { return m_status; }

	c_surface* get_surface() { return m_surface; }
	void set_surface(c_surface* gal) { m_surface = gal; }
protected:
	virtual void pre_create_wnd();
	virtual void display_window();
	void hide_widow();
	void add_child_2_head(c_wnd *child);
	void add_child_2_tail(c_wnd *child);

	void wnd2screen(int &x, int &y) const;
	void wnd2screen(c_rect &rect) const;
	void screen2wnd(short &x, short &y) const;
	void screen2wnd(c_rect &rect) const;

	int load_child_wnd(WND_TREE *p_child_tree);
	int load_clone_child_wnd(WND_TREE *p_child_tree);
	void set_active_child(c_wnd* child) { m_active_child = child; }

	virtual void on_focus();
	virtual void on_kill_focus();

	void set_pixel(int x, int y, unsigned int rgb);
	void draw_hline(int x0, int x1, int y, unsigned int rgb);
	void draw_vline(int x, int y0, int y1, unsigned int rgb);
	void draw_line(int x0, int y0, int x1, int y1, unsigned int rgb);
	void draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb);
	void draw_rect(c_rect rect, unsigned int rgb);
	void fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb);
	void fill_rect(c_rect rect, unsigned int rgb);
	void draw_custom_shape(int l, int t, int r, int b, unsigned int color, const CUSTOM_SHAPE pRgn[]);
protected:
	WND_STATUS		m_status;
	unsigned int	m_style;
	c_rect			m_wnd_rect;//position relative to parent wnd.
	c_wnd*			m_parent;
	c_wnd*			m_top_child;
	c_wnd*			m_prev_sibling;
	c_wnd*			m_next_sibling;
	unsigned short	m_str_id;

	const GUI_BITMAP*	m_bitmap;
	const GUI_BITMAP*	m_bitmap_focus;
	const GUI_BITMAP*	m_bitmap_pushed;
	const GUI_BITMAP*	m_bitmap_disable;
	const GUI_FONT*		m_font_type;
	unsigned int		m_font_color;
	unsigned int		m_bg_color;

	bool				m_is_visible_now;
	unsigned short		m_resource_id;

	int					m_z_order;
	c_wnd*				m_active_child;
	c_surface*			m_surface;
private:
	c_wnd(const c_wnd &win);
	c_wnd& operator=(const c_wnd &win);
};
#endif
