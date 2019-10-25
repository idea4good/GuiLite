#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/surface.h"
#include "../core_include/display.h"

#include <string.h>
#include <stdio.h>

c_display::c_display(void* phy_fb, unsigned int display_width, unsigned int display_height,
						unsigned int surface_width, unsigned int surface_height,
						unsigned int color_bytes, unsigned int surface_cnt, EXTERNAL_GFX_OP* gfx_op)
{
	if (color_bytes != 2 && color_bytes != 4)
	{
		log_out("Support 16 bits, 32 bits color only!");
		ASSERT(false);
	}

	m_width = display_width;
	m_height = display_height;
	m_color_bytes = color_bytes;
	m_phy_fb = phy_fb;
	m_phy_read_index = m_phy_write_index = 0;
	memset(m_surface_group, 0, sizeof(m_surface_group));
	m_surface_index = 0;
	m_surface_cnt = surface_cnt;
	ASSERT(m_surface_cnt <= SURFACE_CNT_MAX);
	
	for (int i = 0; i < m_surface_cnt; i++)
	{
		m_surface_group[i] = phy_fb ? new c_surface(this, surface_width, surface_height, color_bytes) : new c_surface_no_fb(this, surface_width, surface_height, color_bytes, gfx_op);
	}
}

c_surface* c_display::alloc_surface(Z_ORDER_LEVEL max_zorder)
{
	if(max_zorder >= Z_ORDER_LEVEL_MAX || m_surface_index >= m_surface_cnt)
	{
		ASSERT(false);
		return 0;
	}
	int i = m_surface_index++;
	m_surface_group[i]->set_surface(max_zorder);
	return m_surface_group[i];
}

int c_display::swipe_surface(c_surface* s0, c_surface* s1, int x0, int x1, int y0, int y1, int offset)
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
	else if(m_color_bytes == 4)
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

void* c_display::get_updated_fb(int* width, int* height, bool force_update)
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

int c_display::snap_shot(const char* file_name)
{
	if (!m_phy_fb)
	{
		return -1;
	}

	unsigned int width = get_width();
	unsigned int height = get_height();

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
	delete []p_bmp565_data;
	return ret;
}
