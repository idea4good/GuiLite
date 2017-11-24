#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/msg.h"
#include "../core_include/surface.h"
#include "../core_include/display.h"

#include <string.h>
#include <stdio.h>

c_display* c_display::ms_displays[MAX_DISPLAY];

c_display::c_display(void* phy_fb, unsigned int width, unsigned int height, unsigned int slides_cnt)
{
	m_width = width;
	m_height = height;
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

	m_surface_cnt = slides_cnt + 1;//slides + root
	if (m_surface_cnt > SURFACE_CNT_MAX)
	{
		ASSERT(FALSE);
	}
	memset(m_surface_group, 0, sizeof(m_surface_group));
	for (int i = 0; i < m_surface_cnt; i++)
	{
		m_surface_group[i] = new c_surface(this, m_phy_fb, width, height);
	}
}

c_surface* c_display::create_surface(void* usr, Z_ORDER_LEVEL max_zorder)
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
	ASSERT(FALSE);
	return NULL;
}

int c_display::merge_surface(c_surface* s1, c_surface* s2, int x1, int x2, int y1, int y2, int offset)
{
	if (offset < 0 || offset >= m_width)
	{
		ASSERT(FALSE);
		return -1;
	}
	if (y1 < 0 || y1 >= m_height)
	{
		ASSERT(FALSE);
		return -1;
	}
	if (y2 < 0 || y2 >= m_height)
	{
		ASSERT(FALSE);
		return -1;
	}
	if (x1 < 0 || x1 >= m_width)
	{
		ASSERT(FALSE);
		return -1;
	}
	if (x2 < 0 || x2 >= m_width)
	{
		ASSERT(FALSE);
		return -1;
	}

	int width = (x2 - x1 + 1);
	if (width < 0 || width >= m_width || width < offset)
	{
		ASSERT(FALSE);
		return -1;
	}

	for (int y = y1; y <= y2; y++)
	{
		//Left surface
		char* addr_s = ((char*)(s1->m_fb) + (y * m_width + x1 + offset) * 2);
		char* addr_d = ((char*)(m_phy_fb) + (y * m_width + x1) * 2);
		memcpy(addr_d, addr_s, (width - offset) * 2);
		//Right surface
		addr_s = ((char*)(s2->m_fb) + (y * m_width + x1) * 2);
		addr_d = ((char*)(m_phy_fb) + (y * m_width + x1 + (width - offset)) * 2);
		memcpy(addr_d, addr_s, offset * 2);
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
	return build_bmp(path, width, height, (unsigned char*)ms_displays[display_id]->m_phy_fb);
}