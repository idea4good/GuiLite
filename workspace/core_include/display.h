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
