#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/msg.h"
#include "../core_include/surface.h"
#include "../core_include/display.h"

#include <string.h>
#include <stdio.h>

c_display* c_display::ms_displays[MAX_DISPLAY];

c_display::c_display(void* phy_fb, unsigned int display_width, unsigned int display_height,
						unsigned int surface_width, unsigned int surface_height,
						unsigned int color_bytes, unsigned int surface_cnt)
{
	if (color_bytes != 2 && color_bytes != 4)
	{
		log_out("Support 16 bits, 32 bits color only!");
		ASSERT(FALSE);
	}

	m_width = display_width;
	m_height = display_height;
	m_color_bytes = color_bytes;
	m_phy_fb = phy_fb;
	m_hid_pipe = new c_hid_pipe(NULL);
	for (int i = 0; i < MAX_DISPLAY; i++)
	{
		if (!ms_displays[i])
		{
			ms_displays[i] = this;
			break;
		}
	}

	m_surface_cnt = surface_cnt;
	if (m_surface_cnt > SURFACE_CNT_MAX)
	{
		ASSERT(FALSE);
	}
	memset(m_surface_group, 0, sizeof(m_surface_group));
	for (int i = 0; i < m_surface_cnt; i++)
	{
		m_surface_group[i] = (color_bytes == 4) ? new c_surface(this, m_phy_fb, surface_width, surface_height, color_bytes) :
												  new c_surface_16bits(this, m_phy_fb, surface_width, surface_height, color_bytes);
	}
}

c_surface* c_display::alloc_surface(void* usr, Z_ORDER_LEVEL max_zorder)
{
	int i = 0;
	if (max_zorder >= Z_ORDER_LEVEL_MAX)
	{
		ASSERT(FALSE);
	}

	while (i < m_surface_cnt)
	{
		if (m_surface_group[i]->m_usr == usr)
		{
			//repeat register
			ASSERT(FALSE);
			return m_surface_group[i];
		}
		i++;
	}

	i = 0;
	while (i < m_surface_cnt)
	{
		if (m_surface_group[i]->m_usr == NULL)
		{	
			m_surface_group[i]->set_surface(usr, max_zorder);
			return m_surface_group[i];
		}
		i++;
	}
	//no surface for use
	ASSERT(FALSE);
	return NULL;
}

int c_display::merge_surface(c_surface* s0, c_surface* s1, int x0, int x1, int y0, int y1, int offset)
{
	int surface_width = s0->get_width();
	int surface_height = s0->get_height();

	if (offset < 0 || offset >= surface_width || y0 < 0 || y0 >= surface_height ||
		y1 < 0 || y1 >= surface_height || x0 < 0 || x0 >= surface_width || x1 < 0 || x1 >= surface_width)
	{
		ASSERT(FALSE);
		return -1;
	}

	int width = (x1 - x0 + 1);
	if (width < 0 || width >= surface_width || width < offset)
	{
		ASSERT(FALSE);
		return -1;
	}

	x0 = (x0 >= m_width) ? (m_width - 1) : x0;
	x1 = (x1 >= m_width) ? (m_width - 1) : x1;
	y0 = (y0 >= m_height) ? (m_height - 1) : y0;
	y1 = (y1 >= m_height) ? (m_height - 1) : y1;
	for (int y = y0; y <= y1; y++)
	{
		//Left surface
		char* addr_s = ((char*)(s0->m_fb) + (y * (s0->get_width()) + x0 + offset) * m_color_bytes);
		char* addr_d = ((char*)(m_phy_fb) + (y * m_width + x0) * m_color_bytes);
		memcpy(addr_d, addr_s, (width - offset) * m_color_bytes);
		//Right surface
		addr_s = ((char*)(s1->m_fb) + (y * (s1->get_width()) + x0) * m_color_bytes);
		addr_d = ((char*)(m_phy_fb) + (y * m_width + x0 + (width - offset)) * m_color_bytes);
		memcpy(addr_d, addr_s, offset * m_color_bytes);
	}
	return 0;
}

void* c_display::get_frame_buffer(unsigned int display_id, int* width, int* height)
{
	if (MAX_DISPLAY <= display_id)
	{
		ASSERT(FALSE);
		return NULL;
	}
	if (ms_displays[display_id])
	{
		if (width && height) 
		{
			*width = ms_displays[display_id]->get_width();
			*height = ms_displays[display_id]->get_height();
		}
		return ms_displays[display_id]->m_phy_fb;
	}
	return NULL;
}

int c_display::snap_shot(unsigned int display_id)
{
	if (MAX_DISPLAY <= display_id)
	{
		ASSERT(FALSE);
		return -1;
	}

	if (!ms_displays[display_id] || !ms_displays[display_id]->m_phy_fb)
	{
		return -2;
	}

	char path[32];
	memset(path, 0, sizeof(path));
	sprintf(path, "snapshot_%d.bmp", display_id);

	unsigned int width = ms_displays[display_id]->get_width();
	unsigned int height = ms_displays[display_id]->get_height();

	//16 bits framebuffer
	if (ms_displays[display_id]->m_color_bytes == 2)
	{
		return build_bmp(path, width, height, (unsigned char*)ms_displays[display_id]->m_phy_fb);
	}

	//32 bits framebuffer
	unsigned short* p_bmp565_data = new unsigned short[width * height];
	unsigned int* p_raw_data = (unsigned int*)ms_displays[display_id]->m_phy_fb;

	for (int i = 0; i < width * height; i++)
	{
		unsigned int rgb = *p_raw_data++;
		p_bmp565_data[i] = GL_RGB_32_to_16(rgb);
	}

	int ret = build_bmp(path, width, height, (unsigned char*)p_bmp565_data);
	delete []p_bmp565_data;
	return ret;
}