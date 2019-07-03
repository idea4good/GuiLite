#ifndef GAL_H
#define GAL_H

class c_frame_layer
{
public:
	c_frame_layer() { fb = 0;}
	unsigned short*	fb;
	c_rect 	rect;
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

	int flush_scrren(int left, int top, int right, int bottom);
	bool is_valid(c_rect rect);
	bool is_active() { return m_is_active; }
	c_display* get_display() { return m_display; }

	int set_frame_layer(c_rect& rect, unsigned int z_order);
	void set_active(bool flag){m_is_active = flag;}
protected:
	virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb);
	virtual void draw_pixel_on_fb(int x, int y, unsigned int rgb);
	void set_surface(void* wnd_root, Z_ORDER_LEVEL max_z_order);
	c_surface(c_display* display, unsigned int width, unsigned int height, unsigned int color_bytes);
	int						m_width;		//in pixels
	int						m_height;		//in pixels
	int						m_color_bytes;	//16 bits, 32 bits only
	void* 					m_fb;			//Top frame buffer you could see
	c_frame_layer 			m_frame_layers[Z_ORDER_LEVEL_MAX];//Top layber fb always be 0
	void*					m_usr;
	bool					m_is_active;
	Z_ORDER_LEVEL			m_max_zorder;
	Z_ORDER_LEVEL			m_top_zorder;
	void*					m_phy_fb;
	int*					m_phy_write_index;
	c_display*				m_display;
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
