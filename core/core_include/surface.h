#ifndef _GAL_H_
#define _GAL_H_

struct FRAME_LAYER
{
	void*	fb;
	c_rect 	rect;
};

typedef enum
{
	Z_ORDER_LEVEL_0,//view/wave/page
	Z_ORDER_LEVEL_1,//dialog
	Z_ORDER_LEVEL_2,//editbox/spinbox/listbox/keyboard
	Z_ORDER_LEVEL_MAX
}Z_ORDER_LEVEL;

typedef struct struct_color_rect COLOR_RECT;

class c_display;
class c_surface {
	friend class c_display;
public:
	virtual void draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order);
	virtual void fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
	void fill_rect_ex(int l, int t, int r, int b, unsigned int color, const COLOR_RECT* extend_rects, int z_order);
	virtual unsigned int get_pixel(int x, int y, unsigned int z_order);

	int get_width() { return m_width; }
	int get_height() { return m_height; }
	void draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order);
	void draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order);
	void draw_line(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
	void draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
	int flush_scrren(int left, int top, int right, int bottom);

	bool is_valid(c_rect rect);
	bool is_active() { return m_is_active; }
	c_display* get_display() { return m_display; }

	int set_frame_layer(c_rect& rect, unsigned int z_order);
	void set_active(bool flag){m_is_active = flag;}
protected:
	virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb);
	virtual void set_pixel(int x, int y, unsigned int rgb);

	void set_surface(void* wnd_root, Z_ORDER_LEVEL max_z_order);
	int copy_layer_pixel_2_fb(int x, int y, unsigned int z_order);
	c_surface(c_display* display, void* phy_fb, unsigned int width, unsigned int height, unsigned int color_bytes);
	int						m_width;		//in pixels
	int						m_height;		//in pixels
	int						m_color_bytes;	//16 bits, 32 bits only
	void* 					m_fb;
	struct FRAME_LAYER 		m_frame_layers[Z_ORDER_LEVEL_MAX];
	void*					m_usr;
	bool					m_is_active;
	Z_ORDER_LEVEL			m_max_zorder;
	Z_ORDER_LEVEL			m_top_zorder;
	void*					m_phy_fb;
	c_display*				m_display;
};

class c_surface_16bits : public c_surface {
	friend class c_display;
	c_surface_16bits(c_display* display, void* phy_fb, unsigned int width, unsigned int height, unsigned int color_bytes) :
					c_surface(display, phy_fb, width, height, color_bytes) {};
	virtual void draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order);
	virtual void fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
	virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb);
	virtual unsigned int get_pixel(int x, int y, unsigned int z_order);
protected:
	virtual void set_pixel(int x, int y, unsigned int rgb);
};
#endif
