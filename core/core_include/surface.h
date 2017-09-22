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

#define INVALID_RGN 0xFFFFFF
#define COLOR_USERDEF GLT_RGB(41,49,49)
#define DUMMY_COLOR	GLT_RGB(0,0,0)

typedef struct struct_custom_shape
{
	int l;			 
	int t;			 
	int r;			 
	int b;
	unsigned int color;
}CUSTOM_SHAPE;

class c_display;
class c_surface {
	friend class c_display;
public:
	c_surface(c_display* display, void* phy_fb, unsigned int width, unsigned int height);

	void set_pixel(int x, int y, unsigned int rgb, unsigned int z_order);
	unsigned int get_pixel(int x, int y, unsigned int z_order);
	void draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order);
	void draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order);
	void draw_line(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
	void draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
	void fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
	void draw_custom_shape(int l, int t, int r, int b, unsigned int color, const CUSTOM_SHAPE pRgn[], int z_order);
	int flush_scrren(int left, int top, int right, int bottom);

	bool is_valid(c_rect rect);
	bool is_active() { return m_is_active; }
	c_display* get_display() { return m_display; }

	int set_frame_layer(c_rect& rect, unsigned int z_order);
	void set_active(bool flag){m_is_active = flag;}
private:
	void set_surface(void* wnd_root, Z_ORDER_LEVEL max_z_order);
	int copy_layer_pixel_2_fb(int x, int y, unsigned int z_order);
	void do_quick_set_pixel(int x,int y,unsigned int rgb);
	void do_quick_fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb);

	unsigned int			m_width;	//in pixels
	unsigned int			m_height;	//in pixels
	void* 					m_fb;
	struct FRAME_LAYER 		m_frame_layers[Z_ORDER_LEVEL_MAX];
	void*					m_usr;
	bool					m_is_active;
	Z_ORDER_LEVEL			m_max_zorder;
	Z_ORDER_LEVEL			m_top_zorder;
	void*					m_phy_fb;
	c_display*				m_display;
};

#endif
