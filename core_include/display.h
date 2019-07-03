#ifndef DISPLAY_H
#define DISPLAY_H

#define SURFACE_CNT_MAX	6//root + pages

class c_hid_pipe;
class c_surface;

class c_display {
	friend class c_surface;
public:
	c_display(void* phy_fb, unsigned int display_width, unsigned int display_height,
					unsigned int surface_width, unsigned int surface_height,
					unsigned int color_bytes, unsigned int surface_cnt, EXTERNAL_GFX_OP* gfx_op = 0);
	c_surface* alloc_surface(void* usr, Z_ORDER_LEVEL max_zorder);
	int merge_surface(c_surface* s1, c_surface* s2, int x0, int x1, int y0, int y2, int offset);
	unsigned int get_width() { return m_width; }
	unsigned int get_height() { return m_height; }

	void* get_updated_fb(int* width, int* height, bool force_update = false);
	int snap_shot(const char* file_name);
private:
	unsigned int	m_width;		//in pixels
	unsigned int	m_height;		//in pixels
	unsigned int	m_color_bytes;	//16 bits, 32 bits only
	void*			m_phy_fb;
	int				m_phy_read_index;
	int				m_phy_write_index;
	c_surface* 		m_surface_group[SURFACE_CNT_MAX];
	unsigned int	m_surface_cnt;
};
#endif
