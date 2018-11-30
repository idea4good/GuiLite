#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define MAX_DISPLAY		9
#define SURFACE_CNT_MAX	6//root + pages

class c_hid_pipe;
class c_surface;

class c_display {
public:
	c_display(void* phy_fb, unsigned int display_width, unsigned int display_height,
					unsigned int surface_width, unsigned int surface_height,
					unsigned int color_bytes, unsigned int surface_cnt);
	c_surface* alloc_surface(void* usr, Z_ORDER_LEVEL max_zorder);
	int merge_surface(c_surface* s1, c_surface* s2, int x0, int x1, int y0, int y2, int offset);
	c_hid_pipe* get_hid_pipe() { return m_hid_pipe; }
	unsigned int get_width() { return m_width; }
	unsigned int get_height() { return m_height; }

	static void* get_frame_buffer(unsigned int display_id, int* width, int* height);
	static int snap_shot(unsigned int display_id);
private:
	unsigned int	m_width;		//in pixels
	unsigned int	m_height;		//in pixels
	unsigned int	m_color_bytes;	//16 bits, 32 bits only
	void*			m_phy_fb;
	c_hid_pipe*		m_hid_pipe;
	c_surface* 		m_surface_group[SURFACE_CNT_MAX];
	unsigned int	m_surface_cnt;

	static c_display* ms_displays[MAX_DISPLAY];
};
#endif