#pragma once

#include "../core_include/api.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SURFACE_CNT_MAX	6//root + pages

typedef enum
{
	Z_ORDER_LEVEL_0,//lowest graphic level
	Z_ORDER_LEVEL_1,//middle graphic level
	Z_ORDER_LEVEL_2,//highest graphic level
	Z_ORDER_LEVEL_MAX
}Z_ORDER_LEVEL;

struct EXTERNAL_GFX_OP
{
	void(*draw_pixel)(int x, int y, unsigned int rgb);
	void(*fill_rect)(int x0, int y0, int x1, int y1, unsigned int rgb);
};

class c_surface;
class c_display {
	friend class c_surface;
public:
	inline c_display(void* phy_fb, int display_width, int display_height, int surface_width, int surface_height, unsigned int color_bytes, int surface_cnt, EXTERNAL_GFX_OP* gfx_op = 0);//multiple surface or surface_no_fb
	inline c_display(void* phy_fb, int display_width, int display_height, c_surface* surface);//single custom surface
	inline c_surface* alloc_surface(Z_ORDER_LEVEL max_zorder, c_rect layer_rect = c_rect());//for multiple surfaces
	inline int swipe_surface(c_surface* s0, c_surface* s1, int x0, int x1, int y0, int y1, int offset);
	int get_width() { return m_width; }
	int get_height() { return m_height; }

	void* get_updated_fb(int* width, int* height, bool force_update = false)
	{
		if (width && height)
		{
			*width = get_width();
			*height = get_height();
		}
		if (force_update)
		{
			return m_phy_fb;
		}
		if (m_phy_read_index == m_phy_write_index)
		{//No update
			return 0;
		}
		m_phy_read_index = m_phy_write_index;
		return m_phy_fb;
	}

	int snap_shot(const char* file_name)
	{
		if (!m_phy_fb || (m_color_bytes !=2 && m_color_bytes != 4))
		{
			return -1;
		}

		int width = get_width();
		int height = get_height();

		//16 bits framebuffer
		if (m_color_bytes == 2)
		{
			return build_bmp(file_name, width, height, (unsigned char*)m_phy_fb);
		}

		//32 bits framebuffer
		unsigned short* p_bmp565_data = new unsigned short[width * height];
		unsigned int* p_raw_data = (unsigned int*)m_phy_fb;

		for (int i = 0; i < width * height; i++)
		{
			unsigned int rgb = *p_raw_data++;
			p_bmp565_data[i] = GL_RGB_32_to_16(rgb);
		}

		int ret = build_bmp(file_name, width, height, (unsigned char*)p_bmp565_data);
		delete[]p_bmp565_data;
		return ret;
	}

private:
	int				m_width;		//in pixels
	int				m_height;		//in pixels
	int				m_color_bytes;	//16 bits, 32 bits only
	void*			m_phy_fb;		//physical framebuffer
	int				m_phy_read_index;
	int				m_phy_write_index;
	c_surface*		m_surface_group[SURFACE_CNT_MAX];
	int				m_surface_cnt;	//surface count
	int				m_surface_index;
};

class c_layer
{
public:
	c_layer() { fb = 0; }
	void* fb;		//framebuffer
	c_rect 	rect;	//framebuffer area
};

class c_surface {
	friend class c_display; friend class c_bitmap;
public:
	c_surface(unsigned int width, unsigned int height, unsigned int color_bytes, Z_ORDER_LEVEL max_zorder = Z_ORDER_LEVEL_0, c_rect overlpa_rect = c_rect()) : m_width(width), m_height(height), m_color_bytes(color_bytes), m_fb(0), m_is_active(false), m_top_zorder(Z_ORDER_LEVEL_0), m_phy_fb(0), m_phy_write_index(0), m_display(0)
	{
		(overlpa_rect == c_rect()) ? set_surface(max_zorder, c_rect(0, 0, width - 1, height - 1)) : set_surface(max_zorder, overlpa_rect);
	}

	int get_width() { return m_width; }
	int get_height() { return m_height; }

	unsigned int get_pixel(int x, int y, unsigned int z_order)
	{
		if (x >= m_width || y >= m_height || x < 0 || y < 0 || z_order >= Z_ORDER_LEVEL_MAX)
		{
			ASSERT(false);
			return 0;
		}
		if (m_layers[z_order].fb)
		{
			return (m_color_bytes == 4) ? ((unsigned int*)(m_layers[z_order].fb))[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)(m_layers[z_order].fb))[y * m_width + x]);
		}
		else if (m_fb)
		{
			return (m_color_bytes == 4) ? ((unsigned int*)m_fb)[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)m_fb)[y * m_width + x]);
		}
		else if (m_phy_fb)
		{
			return (m_color_bytes == 4) ? ((unsigned int*)m_phy_fb)[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)m_phy_fb)[y * m_width + x]);
		}
		return 0;
	}

	virtual void draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order)
	{
		if (x >= m_width || y >= m_height || x < 0 || y < 0)
		{
			return;
		}
		if (z_order > (unsigned int)m_max_zorder)
		{
			ASSERT(false);
			return;
		}

		if (z_order == m_max_zorder)
		{
			return draw_pixel_on_fb(x, y, rgb);
		}
		
		if (z_order > (unsigned int)m_top_zorder)
		{
			m_top_zorder = (Z_ORDER_LEVEL)z_order;
		}

		if (m_layers[z_order].rect.pt_in_rect(x, y))
		{
			c_rect layer_rect = m_layers[z_order].rect;
			if (m_color_bytes == 4)
			{
				((unsigned int*)(m_layers[z_order].fb))[(x - layer_rect.m_left) + (y - layer_rect.m_top) * layer_rect.width()] = rgb;
			}
			else
			{
				((unsigned short*)(m_layers[z_order].fb))[(x - layer_rect.m_left) + (y - layer_rect.m_top) * layer_rect.width()] = GL_RGB_32_to_16(rgb);
			}
		}
		
		if (z_order == m_top_zorder)
		{
			return draw_pixel_on_fb(x, y, rgb);
		}

		bool be_overlapped = false;
		for (unsigned int tmp_z_order = Z_ORDER_LEVEL_MAX - 1; tmp_z_order > z_order; tmp_z_order--)
		{
			if (m_layers[tmp_z_order].rect.pt_in_rect(x, y))
			{
				be_overlapped = true;
				break;
			}
		}

		if (!be_overlapped)
		{
			draw_pixel_on_fb(x, y, rgb);
		}
	}

	virtual void fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order)
	{
		x0 = (x0 < 0) ? 0 : x0;
		y0 = (y0 < 0) ? 0 : y0;
		x1 = (x1 > (m_width - 1)) ? (m_width - 1) : x1;
		y1 = (y1 > (m_height - 1)) ? (m_height - 1) : y1;

		if (z_order == m_max_zorder)
		{
			return fill_rect_on_fb(x0, y0, x1, y1, rgb);
		}

		if (z_order == m_top_zorder)
		{
			int x, y;
			c_rect layer_rect = m_layers[z_order].rect;
			unsigned int rgb_16 = GL_RGB_32_to_16(rgb);
			for (y = y0; y <= y1; y++)
			{
				for (x = x0; x <= x1; x++)
				{
					if (layer_rect.pt_in_rect(x, y))
					{
						if (m_color_bytes == 4)
						{
							((unsigned int*)m_layers[z_order].fb)[(y - layer_rect.m_top) * layer_rect.width() + (x - layer_rect.m_left)] = rgb;
						}
						else
						{
							((unsigned short*)m_layers[z_order].fb)[(y - layer_rect.m_top) * layer_rect.width() + (x - layer_rect.m_left)] = rgb_16;
						}
					}
				}
			}
			return fill_rect_on_fb(x0, y0, x1, y1, rgb);
		}

		for (; y0 <= y1; y0++)
		{
			draw_hline(x0, x1, y0, rgb, z_order);
		}
	}

	void draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order)
	{
		for (; x0 <= x1; x0++)
		{
			draw_pixel(x0, y, rgb, z_order);
		}
	}

	void draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order)
	{
		for (; y0 <= y1; y0++)
		{
			draw_pixel(x, y0, rgb, z_order);
		}
	}

	void draw_line(int x1, int y1, int x2, int y2, unsigned int rgb, unsigned int z_order)
	{
		int dx, dy, x, y, e;

		(x1 > x2) ? (dx = x1 - x2) : (dx = x2 - x1);
		(y1 > y2) ? (dy = y1 - y2) : (dy = y2 - y1);

		if (((dx > dy) && (x1 > x2)) || ((dx <= dy) && (y1 > y2)))
		{
			x = x2; y = y2;
			x2 = x1; y2 = y1;
			x1 = x; y1 = y;
		}
		x = x1; y = y1;

		if (dx > dy)
		{
			e = dy - dx / 2;
			for (; x1 <= x2; ++x1, e += dy)
			{
				draw_pixel(x1, y1, rgb, z_order);
				if (e > 0) { e -= dx; (y > y2) ? --y1 : ++y1; }
			}
		}
		else
		{
			e = dx - dy / 2;
			for (; y1 <= y2; ++y1, e += dx)
			{
				draw_pixel(x1, y1, rgb, z_order);
				if (e > 0) { e -= dy; (x > x2) ? --x1 : ++x1; }
			}
		}
	}

	void draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order, unsigned int size = 1)
	{
		for (unsigned int offset = 0; offset < size; offset++)
		{
			draw_hline(x0 + offset, x1 - offset, y0 + offset, rgb, z_order);
			draw_hline(x0 + offset, x1 - offset, y1 - offset, rgb, z_order);
			draw_vline(x0 + offset, y0 + offset, y1 - offset, rgb, z_order);
			draw_vline(x1 - offset, y0 + offset, y1 - offset, rgb, z_order);
		}
	}

	void draw_rect(c_rect rect, unsigned int rgb, unsigned int size, unsigned int z_order)
	{
		draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order, size);
	}

	void fill_rect(c_rect rect, unsigned int rgb, unsigned int z_order)
	{
		fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order);
	}

	int flush_screen(int left, int top, int right, int bottom)
	{
		if (left < 0 || left >= m_width || right < 0 || right >= m_width ||
			top < 0 || top >= m_height || bottom < 0 || bottom >= m_height)
		{
			ASSERT(false);
		}

		if (!m_is_active || (0 == m_phy_fb) || (0 == m_fb))
		{
			return -1;
		}

		int display_width = m_display->get_width();
		int display_height = m_display->get_height();

		left = (left >= display_width) ? (display_width - 1) : left;
		right = (right >= display_width) ? (display_width - 1) : right;
		top = (top >= display_height) ? (display_height - 1) : top;
		bottom = (bottom >= display_height) ? (display_height - 1) : bottom;

		for (int y = top; y < bottom; y++)
		{
			void* s_addr = (char*)m_fb + ((y * m_width + left) * m_color_bytes);
			void* d_addr = (char*)m_phy_fb + ((y * display_width + left) * m_color_bytes);
			memcpy(d_addr, s_addr, (right - left) * m_color_bytes);
		}
		*m_phy_write_index = *m_phy_write_index + 1;
		return 0;
	}

	bool is_active() { return m_is_active; }
	c_display* get_display() { return m_display; }

	int show_layer(c_rect& rect, unsigned int z_order)
	{
		ASSERT(z_order >= Z_ORDER_LEVEL_0 && z_order < Z_ORDER_LEVEL_MAX);

		c_rect layer_rect = m_layers[z_order].rect;
		ASSERT(rect.m_left >= layer_rect.m_left && rect.m_right <= layer_rect.m_right &&
			rect.m_top >= layer_rect.m_top && rect.m_bottom <= layer_rect.m_bottom);

		void* fb = m_layers[z_order].fb;
		int width = layer_rect.width();
		for (int y = rect.m_top; y <= rect.m_bottom; y++)
		{
			for (int x = rect.m_left; x <= rect.m_right; x++)
			{
				unsigned int rgb = (m_color_bytes == 4) ? ((unsigned int*)fb)[(x - layer_rect.m_left) + (y - layer_rect.m_top) * width] : GL_RGB_16_to_32(((unsigned short*)fb)[(x - layer_rect.m_left) + (y - layer_rect.m_top) * width]);
				draw_pixel_on_fb(x, y, rgb);
			}
		}
		return 0;
	}
	void set_active(bool flag) { m_is_active = flag; }
protected:
	virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb)
	{
		int display_width = m_display->get_width();
		int display_height = m_display->get_height();

		if (m_color_bytes == 4)
		{
			int x;
			unsigned int* fb, * phy_fb;
			for (; y0 <= y1; y0++)
			{
				x = x0;
				fb = m_fb ? &((unsigned int*)m_fb)[y0 * m_width + x] : 0;
				phy_fb = &((unsigned int*)m_phy_fb)[y0 * display_width + x];
				*m_phy_write_index = *m_phy_write_index + 1;
				for (; x <= x1; x++)
				{
					if (fb)
					{
						*fb++ = rgb;
					}
					if (m_is_active && (x < display_width) && (y0 < display_height))
					{
						*phy_fb++ = rgb;
					}
				}
			}
		}
		else if (m_color_bytes == 2)
		{
			int x;
			unsigned short* fb, * phy_fb;
			rgb = GL_RGB_32_to_16(rgb);
			for (; y0 <= y1; y0++)
			{
				x = x0;
				fb = m_fb ? &((unsigned short*)m_fb)[y0 * m_width + x] : 0;
				phy_fb = &((unsigned short*)m_phy_fb)[y0 * display_width + x];
				*m_phy_write_index = *m_phy_write_index + 1;
				for (; x <= x1; x++)
				{
					if (fb)
					{
						*fb++ = rgb;
					}
					if (m_is_active && (x < display_width) && (y0 < display_height))
					{
						*phy_fb++ = rgb;
					}
				}
			}
		}
	}

	virtual void draw_pixel_on_fb(int x, int y, unsigned int rgb)
	{
		if (m_fb)
		{
			(m_color_bytes == 4) ? ((unsigned int*)m_fb)[y * m_width + x] = rgb : ((unsigned short*)m_fb)[y * m_width + x] = GL_RGB_32_to_16(rgb);
		}

		if (m_is_active && (x < m_display->get_width()) && (y < m_display->get_height()))
		{
			if (m_color_bytes == 4)
			{
				((unsigned int*)m_phy_fb)[y * (m_display->get_width()) + x] = rgb;
			}
			else
			{
				((unsigned short*)m_phy_fb)[y * (m_display->get_width()) + x] = GL_RGB_32_to_16(rgb);
			}
			*m_phy_write_index = *m_phy_write_index + 1;
		}
	}

	void attach_display(c_display* display)
	{
		ASSERT(display);
		m_display = display;
		m_phy_fb = display->m_phy_fb;
		m_phy_write_index = &display->m_phy_write_index;
	}

	void set_surface(Z_ORDER_LEVEL max_z_order, c_rect layer_rect)
	{
		m_max_zorder = max_z_order;
		if (m_display && (m_display->m_surface_cnt > 1))
		{
			m_fb = calloc(m_width * m_height, m_color_bytes);
		}

		for (int i = Z_ORDER_LEVEL_0; i < m_max_zorder; i++)
		{//Top layber fb always be 0
			ASSERT(m_layers[i].fb = calloc(layer_rect.width() * layer_rect.height(), m_color_bytes));
			m_layers[i].rect = layer_rect;
		}
	}

	int				m_width;		//in pixels
	int				m_height;		//in pixels
	int				m_color_bytes;	//16 bits, 32 bits only
	void*			m_fb;			//frame buffer you could see
	c_layer 		m_layers[Z_ORDER_LEVEL_MAX];//all graphic layers
	bool			m_is_active;	//active flag
	Z_ORDER_LEVEL	m_max_zorder;	//the highest graphic layer the surface will have
	Z_ORDER_LEVEL	m_top_zorder;	//the current highest graphic layer the surface have
	void*			m_phy_fb;		//physical framebufer
	int*			m_phy_write_index;
	c_display*		m_display;
};

class c_surface_no_fb : public c_surface {//No physical framebuffer, render with external graphic interface
	friend class c_display;
public:
	c_surface_no_fb(unsigned int width, unsigned int height, unsigned int color_bytes, struct EXTERNAL_GFX_OP* gfx_op, Z_ORDER_LEVEL max_zorder = Z_ORDER_LEVEL_0, c_rect overlpa_rect = c_rect()) : c_surface(width, height, color_bytes, max_zorder, overlpa_rect), m_gfx_op(gfx_op) {}
protected:
	virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb)
	{
		if (!m_gfx_op)
		{
			return;
		}
		if (m_gfx_op->fill_rect)
		{
			return m_gfx_op->fill_rect(x0, y0, x1, y1, rgb);
		}

		if (m_gfx_op->draw_pixel && m_is_active)
		{
			for (int y = y0; y <= y1; y++)
			{
				for (int x = x0; x <= x1; x++)
				{
					m_gfx_op->draw_pixel(x, y, rgb);
				}
			}
		}

		if (!m_fb) { return; }
		if (m_color_bytes == 4)
		{
			unsigned int* fb;
			for (int y = y0; y <= y1; y++)
			{
				fb = &((unsigned int*)m_fb)[y0 * m_width + x0];
				for (int x = x0; x <= x1; x++)
				{
					*fb++ = rgb;
				}
			}
		}
		else if (m_color_bytes == 2)
		{
			unsigned short* fb;
			rgb = GL_RGB_32_to_16(rgb);
			for (int y = y0; y <= y1; y++)
			{
				fb = &((unsigned short*)m_fb)[y0 * m_width + x0];
				for (int x = x0; x <= x1; x++)
				{
					*fb++ = rgb;
				}
			}
		}
	}

	virtual void draw_pixel_on_fb(int x, int y, unsigned int rgb)
	{
		if (m_gfx_op && m_gfx_op->draw_pixel && m_is_active)
		{
			m_gfx_op->draw_pixel(x, y, rgb);
		}

		if (!m_fb) { return; }
		if (m_color_bytes == 4)
		{
			((unsigned int*)m_fb)[y * m_width + x] = rgb;
		}
		else if (m_color_bytes == 2)
		{
			((unsigned short*)m_fb)[y * m_width + x] = GL_RGB_32_to_16(rgb);
		}
	}
	struct EXTERNAL_GFX_OP* m_gfx_op;//Rendering by external method
};

inline c_display::c_display(void* phy_fb, int display_width, int display_height, int surface_width, int surface_height, unsigned int color_bytes, int surface_cnt, EXTERNAL_GFX_OP* gfx_op) : m_width(display_width), m_height(display_height), m_color_bytes(color_bytes), m_phy_fb(phy_fb), m_phy_read_index(0), m_phy_write_index(0), m_surface_cnt(surface_cnt), m_surface_index(0)
{
	ASSERT(color_bytes == 2 || color_bytes == 4);
	ASSERT(m_surface_cnt <= SURFACE_CNT_MAX);
	memset(m_surface_group, 0, sizeof(m_surface_group));
	
	for (int i = 0; i < m_surface_cnt; i++)
	{
		m_surface_group[i] = (phy_fb) ? new c_surface(surface_width, surface_height, color_bytes) : new c_surface_no_fb(surface_width, surface_height, color_bytes, gfx_op);
		m_surface_group[i]->attach_display(this);
	}
}

inline c_display::c_display(void* phy_fb, int display_width, int display_height, c_surface* surface) : m_width(display_width), m_height(display_height), m_phy_fb(phy_fb), m_phy_read_index(0), m_phy_write_index(0), m_surface_cnt(1), m_surface_index(0)
{
	m_color_bytes = surface->m_color_bytes;
	surface->m_is_active = true;
	(m_surface_group[0] = surface)->attach_display(this);
}

inline c_surface* c_display::alloc_surface(Z_ORDER_LEVEL max_zorder, c_rect layer_rect)
{
	ASSERT(max_zorder < Z_ORDER_LEVEL_MAX && m_surface_index < m_surface_cnt);
	(layer_rect == c_rect()) ? m_surface_group[m_surface_index]->set_surface(max_zorder, c_rect(0, 0, m_width - 1, m_height - 1)) : m_surface_group[m_surface_index]->set_surface(max_zorder, layer_rect);
	return m_surface_group[m_surface_index++];
}

inline int c_display::swipe_surface(c_surface* s0, c_surface* s1, int x0, int x1, int y0, int y1, int offset)
{
	int surface_width = s0->get_width();
	int surface_height = s0->get_height();

	if (offset < 0 || offset > surface_width || y0 < 0 || y0 >= surface_height ||
		y1 < 0 || y1 >= surface_height || x0 < 0 || x0 >= surface_width || x1 < 0 || x1 >= surface_width)
	{
		ASSERT(false);
		return -1;
	}

	int width = (x1 - x0 + 1);
	if (width < 0 || width > surface_width || width < offset)
	{
		ASSERT(false);
		return -1;
	}

	x0 = (x0 >= m_width) ? (m_width - 1) : x0;
	x1 = (x1 >= m_width) ? (m_width - 1) : x1;
	y0 = (y0 >= m_height) ? (m_height - 1) : y0;
	y1 = (y1 >= m_height) ? (m_height - 1) : y1;

	if (m_phy_fb)
	{
		for (int y = y0; y <= y1; y++)
		{
			//Left surface
			char* addr_s = ((char*)(s0->m_fb) + (y * (s0->get_width()) + x0 + offset) * m_color_bytes);
			char* addr_d = ((char*)(m_phy_fb)+(y * m_width + x0) * m_color_bytes);
			memcpy(addr_d, addr_s, (width - offset) * m_color_bytes);
			//Right surface
			addr_s = ((char*)(s1->m_fb) + (y * (s1->get_width()) + x0) * m_color_bytes);
			addr_d = ((char*)(m_phy_fb)+(y * m_width + x0 + (width - offset)) * m_color_bytes);
			memcpy(addr_d, addr_s, offset * m_color_bytes);
		}
	}
	else if (m_color_bytes == 4)
	{
		void(*draw_pixel)(int x, int y, unsigned int rgb) = ((c_surface_no_fb*)s0)->m_gfx_op->draw_pixel;
		for (int y = y0; y <= y1; y++)
		{
			//Left surface
			for (int x = x0; x <= (x1 - offset); x++)
			{
				draw_pixel(x, y, ((unsigned int*)s0->m_fb)[y * m_width + x + offset]);
			}
			//Right surface
			for (int x = x1 - offset; x <= x1; x++)
			{
				draw_pixel(x, y, ((unsigned int*)s1->m_fb)[y * m_width + x + offset - x1 + x0]);
			}
		}
	}
	else if (m_color_bytes == 2)
	{
		void(*draw_pixel)(int x, int y, unsigned int rgb) = ((c_surface_no_fb*)s0)->m_gfx_op->draw_pixel;
		for (int y = y0; y <= y1; y++)
		{
			//Left surface
			for (int x = x0; x <= (x1 - offset); x++)
			{
				draw_pixel(x, y, GL_RGB_16_to_32(((unsigned short*)s0->m_fb)[y * m_width + x + offset]));
			}
			//Right surface
			for (int x = x1 - offset; x <= x1; x++)
			{
				draw_pixel(x, y, GL_RGB_16_to_32(((unsigned short*)s1->m_fb)[y * m_width + x + offset - x1 + x0]));
			}
		}
	}

	m_phy_write_index++;
	return 0;
}
