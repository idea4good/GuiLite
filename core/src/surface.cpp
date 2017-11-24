#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/surface.h"
#include <string.h>
#include <stdlib.h>

c_surface::c_surface(c_display* display, void* phy_fb, unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
	m_display = display;
	m_phy_fb = phy_fb;
	m_fb = m_usr = NULL;
	m_top_zorder = m_max_zorder = Z_ORDER_LEVEL_0;
	m_is_active = false;

	m_fb = calloc(m_width * m_height * 2, 1);
	m_frame_layers[Z_ORDER_LEVEL_0].rect = c_rect(0,0,m_width,m_height);
}

void c_surface::set_surface(void* wnd_root, Z_ORDER_LEVEL max_z_order)
{
	m_usr = wnd_root;
	m_max_zorder = max_z_order;
	for(int i = 0; i <= m_max_zorder; i++)
	{
		m_frame_layers[i].fb = calloc(m_width * m_height * 2, 1);
		ASSERT(NULL != m_frame_layers[i].fb);
	}
}

void c_surface::do_quick_set_pixel(int x, int y, unsigned int rgb)
{
	((short*)m_fb)[y * m_width + x] = rgb;
	if(m_is_active)
	{
		((short*)m_phy_fb)[y * m_width + x] = rgb;
	}
}

void c_surface::do_quick_fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb)
{
	if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0)
	{
		ASSERT(FALSE);
	}

	if (x0 >= m_width || x1 >= m_width || y0 >= m_height || y1 >= m_height)
	{
		ASSERT(FALSE);
	}

	int x;
	short *fb, *phy_fb;
	for(;y0 <= y1; y0++)
	{
		x = x0;
		fb = &((short*)m_fb)[y0 * m_width + x];
		phy_fb = &((short*)m_phy_fb)[y0 * m_width + x];
		for(; x <= x1; x++)
		{
			*fb++ = rgb;
			if(m_is_active)
			{
				*phy_fb++ = rgb;
			}
		}
	}
}

void c_surface::set_pixel(int x, int y, unsigned int rgb, unsigned int z_order)
{
	if ( x >= m_width || y >= m_height || x < 0 || y < 0)
	{
		return;
	}

	if (z_order > m_max_zorder)
	{
		ASSERT(FALSE);
		return;
	}

	if(z_order > m_top_zorder)
	{
		m_top_zorder = (Z_ORDER_LEVEL)z_order;
	}

	if(0 == m_frame_layers[z_order].rect.PtInRect(x, y))
	{
		ASSERT(FALSE);
		return;
	}

	if (z_order == m_max_zorder)
	{
		return do_quick_set_pixel(x, y, rgb);
	}

	((short*)(m_frame_layers[z_order].fb))[x + y * m_width] = rgb;

	if(z_order == m_top_zorder)
	{
		return do_quick_set_pixel(x, y, rgb);
	}

	bool is_covered = false;
	for (int tmp_z_order = Z_ORDER_LEVEL_MAX - 1; tmp_z_order > z_order; tmp_z_order--)
	{
		if(1 == m_frame_layers[tmp_z_order].rect.PtInRect(x, y))
		{
			is_covered = true;
			break;
		}
	}

	if (!is_covered)
	{
		((short*)m_fb)[y * m_width + x] = rgb;
		if(m_is_active)
		{
			((short*)m_phy_fb)[y * m_width + x] = rgb;
		}
	}
}

unsigned int c_surface::get_pixel(int x, int y, unsigned int z_order)
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0 ||
			z_order >= Z_ORDER_LEVEL_MAX)
	{
		ASSERT(FALSE);
		return 0;
	}

	if (z_order == m_max_zorder)
	{
		return ((unsigned short*)m_fb)[y * m_width + x];
	}
	
	return ((unsigned short*)(m_frame_layers[z_order].fb))[y * m_width + x];
}

void c_surface::draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order)
{
	if (x0 > x1)
	{
		return;
	}

	for (;x0 <= x1; x0++) 
	{
		set_pixel(x0, y, rgb, z_order);
	}
}

void c_surface::draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order)
{
	if (y0 > y1)
	{
		return;
	}

	for (;y0 <= y1; y0++) 
	{
		set_pixel(x, y0, rgb, z_order);
	}
}

void c_surface::draw_line(int x1, int y1, int x2, int y2, unsigned int rgb, unsigned int z_order)
{
	int dx, dy, e;
	dx = x2 - x1;
	dy = y2 - y1;

	if ((dx >= 0) && (dy >= 0))
	{
		if (dx >= dy)
		{
			e = dy - dx / 2;
			for(; x1 <= x2; x1++, e += dy)
			{
				set_pixel(x1, y1, rgb, z_order);
				if (e>0) { y1++; e -= dx; }
			}
		}
		else
		{
			e = dx - dy / 2;
			for(; y1 <= y2; y1++, e += dx)
			{
				set_pixel(x1, y1, rgb, z_order);
				if (e>0) { x1++; e -= dy; }
			}
		}
	}

	else if ((dx >= 0) && (dy < 0))
	{
		dy = -dy;
		if (dx >= dy)
		{
			e = dy - dx / 2;
			for(; x1 <= x2; x1++, e += dy)
			{
				set_pixel(x1, y1, rgb, z_order);
				if (e>0) { y1--; e -= dx; }
			}
		}
		else
		{
			e = dx - dy / 2;
			for(; y1 >= y2; y1--, e += dx)
			{
				set_pixel(x1, y1, rgb, z_order);
				if (e>0) { x1++; e -= dy; }
			}
		}
	}

	else if ((dx < 0) && (dy >= 0))
	{
		dx = -dx;
		if (dx >= dy)
		{
			e = dy - dx / 2;
			for(; x1 >= x2; x1--, e += dy)
			{
				set_pixel(x1, y1, rgb, z_order);
				if (e>0) { y1++; e -= dx; }
			}
		}
		else
		{
			e = dx - dy / 2;
			for(; y1 <= y2; y1++, e += dx)
			{
				set_pixel(x1, y1, rgb, z_order);
				if (e>0) { x1--; e -= dy; }
			}
		}
	}

	else if ((dx < 0) && (dy < 0))
	{
		dx = -dx;
		dy = -dy;
		if (dx >= dy)
		{
			e = dy - dx / 2;
			for(; x1 >= x2; x1--, e += dy)
			{
				set_pixel(x1, y1, rgb, z_order);
				if (e>0) { y1--; e -= dx; }
			}
		}
		else
		{
			e = dx - dy / 2;
			while (y1-- >= y2)
			for(; y1 >= y2; y1--, e += dx)
			{
				set_pixel(x1, y1, rgb, z_order);
				if (e>0) { x1--; e -= dy; }
			}
		}
	}
}

void c_surface::draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order)
{
	draw_hline(x0, x1, y0, rgb, z_order);
	draw_hline(x0, x1, y1, rgb, z_order);
	draw_vline(x0, y0, y1, rgb, z_order);
	draw_vline(x1, y0, y1, rgb, z_order);
}

void c_surface::fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order)
{
	if (z_order == m_max_zorder)
	{
		return do_quick_fill_rect(x0, y0, x1, y1, rgb);
	}

	if(z_order == m_top_zorder)
	{
		int x,y;
		short *mem_fb;
		for(y = y0; y <= y1; y++)
		{
			x = x0;
			mem_fb = &((short*)m_frame_layers[z_order].fb)[y * m_width + x];
			for(; x <= x1; x++)
			{
				*mem_fb++ = rgb;
			}
		}
		return do_quick_fill_rect(x0, y0, x1, y1, rgb);
	}

	for (; y0 <= y1; y0++) 
	{
		draw_hline(x0, x1, y0, rgb, z_order);
	}
}

int c_surface::copy_layer_pixel_2_fb(int x, int y, unsigned int z_order)
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0 ||
				z_order >= Z_ORDER_LEVEL_MAX)
	{
		ASSERT(FALSE);
		return 0;
	}

	char* des_addr,* src_addr;

	short rgb = ((short*)(m_frame_layers[z_order].fb))[x + y * m_width];
	((short*)m_fb)[y * m_width + x] = rgb;
	if(m_is_active)
	{
		((short*)m_phy_fb)[y * m_width + x] = rgb;
	}
	return 0;
}

int c_surface::set_frame_layer(c_rect& rect, unsigned int z_order)
{
	if (z_order <= Z_ORDER_LEVEL_0 || z_order >= Z_ORDER_LEVEL_MAX)
	{
		ASSERT(FALSE);
		return -1;
	}
	if (!(rect == m_frame_layers[z_order].rect))
	{//release current zone, and recover the lower layer
		c_rect rc = m_frame_layers[z_order].rect;
		int src_order = m_top_zorder = (Z_ORDER_LEVEL)(z_order - 1);
		
		int x,y;
		for(y = rc.m_top; y <= rc.m_bottom; y++)
		{
			for(x = rc.m_left; x <= rc.m_right; x++)
			{
				if(m_frame_layers[src_order].rect.PtInRect(x,y))
				{
					copy_layer_pixel_2_fb(x, y, src_order);
				}
				else
				{
					if(src_order - 1 < Z_ORDER_LEVEL_0)continue;

					if(m_frame_layers[src_order - 1].rect.PtInRect(x,y))
					{
						copy_layer_pixel_2_fb(x, y, src_order - 1);
					}
				}
			}
		}
	}
	m_frame_layers[z_order].rect = rect;
	return 1;
}

void c_surface::draw_custom_shape(int l, int t, int r, int b, unsigned int color, const CUSTOM_SHAPE pRgn[], int z_order)
{
	int i = 0;
	int templ, tempt, tempr, tempb;
	unsigned int tempcolor;
	while (INVALID_RGN != pRgn[i].l)
	{
		templ = (pRgn[i].l < 0) ? (r + 1 + pRgn[i].l) : pRgn[i].l + l;
		tempt = (pRgn[i].t < 0) ? (b + 1 + pRgn[i].t) : pRgn[i].t + t;
		tempr = (pRgn[i].r < 0) ? (r + 1 + pRgn[i].r) : pRgn[i].r + l;
		tempb = (pRgn[i].b < 0) ? (b + 1 + pRgn[i].b) : pRgn[i].b + t;

		if (templ >= tempr)
			tempr = templ;
		if (tempt >= tempb)
			tempb = tempt;

		tempcolor = (COLOR_USERDEF == pRgn[i].color) ? (color) : pRgn[i].color;
	
		for (int y = tempt ; y <= tempb; y++)
		{ 
			for(int x = templ; x <= tempr; x++)
			{
				set_pixel(x , y, tempcolor, z_order);
			}
		}
		i++;
	}
}

int c_surface::flush_scrren(int left, int top, int right, int bottom)
{
	if(left < 0 || left >= m_width)
	{
		ASSERT(FALSE);
	}
	if(right < 0 || right >= m_width)
	{
		ASSERT(FALSE);
	}

	if(top < 0 || top >= m_height)
	{
		ASSERT(FALSE);
	}

	if(bottom < 0 || bottom >= m_height)
	{
		ASSERT(FALSE);
	}

	if(!m_is_active)
	{
		return -1;
	}
	if((0 == m_phy_fb) || (0 == m_fb))
	{
		return -2;
	}

	for (int y = top; y < bottom; y++)
	{
		void* s_addr = (char*)m_fb + ((y * m_width + left) * 2);
		void* d_addr = (char*)m_phy_fb + ((y * m_width + left) * 2);
		memcpy(d_addr, s_addr, (right - left) * 2);
	}
	return 0;
}

bool c_surface::is_valid(c_rect rect)
{
	if (rect.m_left < 0 || rect.m_top < 0)
	{
		return false;
	}

	if (rect.m_right >= m_width || rect.m_bottom >= m_height)
	{
		return false;
	}
	return true;
}