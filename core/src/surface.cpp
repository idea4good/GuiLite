#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/surface.h"
#include "../core_include/display.h"
#include "../core_include/resource.h"
#include <string.h>
#include <stdlib.h>

c_surface::c_surface(c_display* display, void* phy_fb, unsigned int width, unsigned int height, unsigned int color_bytes)
{
	m_width = width;
	m_height = height;
	m_color_bytes = color_bytes;
	m_display = display;
	m_phy_fb = phy_fb;
	m_fb = m_usr = NULL;
	m_top_zorder = m_max_zorder = Z_ORDER_LEVEL_0;
	m_is_active = false;

	m_fb = calloc(m_width * m_height, color_bytes);
	m_frame_layers[Z_ORDER_LEVEL_0].rect = c_rect(0, 0, m_width, m_height);
}

void c_surface::set_surface(void* wnd_root, Z_ORDER_LEVEL max_z_order)
{
	m_usr = wnd_root;
	m_max_zorder = max_z_order;
	for(int i = 0; i <= m_max_zorder; i++)
	{
		m_frame_layers[i].fb = calloc(m_width * m_height, m_color_bytes);
		ASSERT(NULL != m_frame_layers[i].fb);
	}
}

void c_surface::set_pixel(int x, int y, unsigned int rgb, unsigned int z_order)
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0)
	{
		return;
	}
	if (z_order > m_max_zorder)
	{
		ASSERT(FALSE);
		return;
	}

	if (z_order > m_top_zorder)
	{
		m_top_zorder = (Z_ORDER_LEVEL)z_order;
	}

	if (0 == m_frame_layers[z_order].rect.PtInRect(x, y))
	{
		ASSERT(FALSE);
		return;
	}

	if (z_order == m_max_zorder)
	{
		return set_pixel_on_fb(x, y, rgb);
	}

	((unsigned int*)(m_frame_layers[z_order].fb))[x + y * m_width] = rgb;

	if (z_order == m_top_zorder)
	{
		return set_pixel_on_fb(x, y, rgb);
	}

	bool is_covered = false;
	for (int tmp_z_order = Z_ORDER_LEVEL_MAX - 1; tmp_z_order > z_order; tmp_z_order--)
	{
		if (1 == m_frame_layers[tmp_z_order].rect.PtInRect(x, y))
		{
			is_covered = true;
			break;
		}
	}

	if (!is_covered)
	{
		set_pixel_on_fb(x, y, rgb);
	}
}

void c_surface::set_pixel_on_fb(int x, int y, unsigned int rgb)
{
	((unsigned int*)m_fb)[y * m_width + x] = rgb;

	int display_width = m_display->get_width();
	int display_height = m_display->get_height();
	if (m_is_active && (x < display_width) && (y < display_height))
	{
		((unsigned int*)m_phy_fb)[y * (m_display->get_width()) + x] = rgb;
	}
}

void c_surface::fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order)
{
	if (z_order == m_max_zorder)
	{
		return fill_rect_on_fb(x0, y0, x1, y1, rgb);
	}

	if (z_order == m_top_zorder)
	{
		int x, y;
		unsigned int *mem_fb;
		for (y = y0; y <= y1; y++)
		{
			x = x0;
			mem_fb = &((unsigned int*)m_frame_layers[z_order].fb)[y * m_width + x];
			for (; x <= x1; x++)
			{
				*mem_fb++ = rgb;
			}
		}
		return fill_rect_on_fb(x0, y0, x1, y1, rgb);
	}

	for (; y0 <= y1; y0++)
	{
		draw_hline(x0, x1, y0, rgb, z_order);
	}
}

void c_surface::fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb)
{
	if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0 ||
		x0 >= m_width || x1 >= m_width || y0 >= m_height || y1 >= m_height)
	{
		ASSERT(FALSE);
	}

	int x;
	unsigned int *fb, *phy_fb;
	int display_width = m_display->get_width();
	int display_height = m_display->get_height();
	for (; y0 <= y1; y0++)
	{
		x = x0;
		fb = &((unsigned int*)m_fb)[y0 * m_width + x];
		phy_fb = &((unsigned int*)m_phy_fb)[y0 * display_width + x];
		for (; x <= x1; x++)
		{
			*fb++ = rgb;
			if (m_is_active && (x < display_width) && (y0 < display_height))
			{
				*phy_fb++ = rgb;
			}
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
		return ((unsigned int*)m_fb)[y * m_width + x];
	}
	
	return ((unsigned int*)(m_frame_layers[z_order].fb))[y * m_width + x];
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

int c_surface::copy_layer_pixel_2_fb(int x, int y, unsigned int z_order)
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0 ||
		z_order >= Z_ORDER_LEVEL_MAX)
	{
		ASSERT(FALSE);
		return 0;
	}

	int display_width = m_display->get_width();
	int display_height = m_display->get_height();

	if (m_color_bytes == 4)
	{
		unsigned int rgb = ((unsigned int*)(m_frame_layers[z_order].fb))[x + y * m_width];
		((unsigned int*)m_fb)[y * m_width + x] = rgb;
		if (m_is_active && (x < display_width) && (y < display_height))
		{
			((unsigned int*)m_phy_fb)[y * display_width + x] = rgb;
		}
	}
	else//16 bits
	{
		short rgb = ((short*)(m_frame_layers[z_order].fb))[x + y * m_width];
		((short*)m_fb)[y * m_width + x] = rgb;
		if (m_is_active && (x < display_width) && (y < display_height))
		{
			((short*)m_phy_fb)[y * display_width + x] = rgb;
		}
	}
	return 0;
}

void c_surface::fill_rect_ex(int l, int t, int r, int b, unsigned int color, const COLOR_RECT* extend_rects, int z_order)
{
	if (NULL == extend_rects)
	{
		return fill_rect(l, t, r, b, color, z_order);
	}

	COLOR_RECT* p_item = (COLOR_RECT*)extend_rects;
	int templ, tempt, tempr, tempb;
	for(int i = 0; INVALID_RGN != p_item[i].l; i++)
	{
		templ = (p_item[i].l < 0) ? (r + 1 + p_item[i].l) : p_item[i].l + l;
		tempt = (p_item[i].t < 0) ? (b + 1 + p_item[i].t) : p_item[i].t + t;
		tempr = (p_item[i].r < 0) ? (r + 1 + p_item[i].r) : p_item[i].r + l;
		tempb = (p_item[i].b < 0) ? (b + 1 + p_item[i].b) : p_item[i].b + t;

		if (templ >= tempr)
			tempr = templ;
		if (tempt >= tempb)
			tempb = tempt;

		unsigned int tempcolor = (COLOR_USERDEF == p_item[i].color) ? (color) : p_item[i].color;
	
		for (int y = tempt ; y <= tempb; y++)
		{ 
			for(int x = templ; x <= tempr; x++)
			{
				set_pixel(x , y, tempcolor, z_order);
			}
		}
	}
}

int c_surface::flush_scrren(int left, int top, int right, int bottom)
{
	if(left < 0 || left >= m_width || right < 0 || right >= m_width ||
		top < 0 || top >= m_height || bottom < 0 || bottom >= m_height)
	{
		ASSERT(FALSE);
	}

	if(!m_is_active || (0 == m_phy_fb) || (0 == m_fb))
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

//////////////////////////////////////////////////////////////////////////////////////
void c_surface_16bits::set_pixel(int x, int y, unsigned int rgb, unsigned int z_order)
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0)
	{
		return;
	}
	if (z_order > m_max_zorder)
	{
		ASSERT(FALSE);
		return;
	}

	if (z_order > m_top_zorder)
	{
		m_top_zorder = (Z_ORDER_LEVEL)z_order;
	}

	if (0 == m_frame_layers[z_order].rect.PtInRect(x, y))
	{
		ASSERT(FALSE);
		return;
	}

	rgb = GLT_RGB_32_to_16(rgb);
	if (z_order == m_max_zorder)
	{
		return set_pixel_on_fb(x, y, rgb);
	}

	((unsigned short*)(m_frame_layers[z_order].fb))[x + y * m_width] = rgb;

	if (z_order == m_top_zorder)
	{
		return set_pixel_on_fb(x, y, rgb);
	}

	bool is_covered = false;
	for (int tmp_z_order = Z_ORDER_LEVEL_MAX - 1; tmp_z_order > z_order; tmp_z_order--)
	{
		if (1 == m_frame_layers[tmp_z_order].rect.PtInRect(x, y))
		{
			is_covered = true;
			break;
		}
	}

	if (!is_covered)
	{
		set_pixel_on_fb(x, y, rgb);
	}
}

void c_surface_16bits::set_pixel_on_fb(int x, int y, unsigned int rgb)
{
	((unsigned short*)m_fb)[y * m_width + x] = rgb;

	int display_width = m_display->get_width();
	int display_height = m_display->get_height();
	if (m_is_active && (x < display_width) && (y < display_height))
	{
		((unsigned short*)m_phy_fb)[y * (m_display->get_width()) + x] = rgb;
	}
}

void c_surface_16bits::fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order)
{
	rgb = GLT_RGB_32_to_16(rgb);
	if (z_order == m_max_zorder)
	{
		return fill_rect_on_fb(x0, y0, x1, y1, rgb);
	}
	if (z_order == m_top_zorder)
	{
		int x, y;
		unsigned short *mem_fb;
		for (y = y0; y <= y1; y++)
		{
			x = x0;
			mem_fb = &((unsigned short*)m_frame_layers[z_order].fb)[y * m_width + x];
			for (; x <= x1; x++)
			{
				*mem_fb++ = rgb;
			}
		}
		return fill_rect_on_fb(x0, y0, x1, y1, rgb);
	}

	for (; y0 <= y1; y0++)
	{
		draw_hline(x0, x1, y0, rgb, z_order);
	}
}

void c_surface_16bits::fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb)
{
	if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0 ||
		x0 >= m_width || x1 >= m_width || y0 >= m_height || y1 >= m_height)
	{
		ASSERT(FALSE);
	}

	int x;
	unsigned short *fb, *phy_fb;
	int display_width = m_display->get_width();
	int display_height = m_display->get_height();
	for (; y0 <= y1; y0++)
	{
		x = x0;
		fb = &((unsigned short*)m_fb)[y0 * m_width + x];
		phy_fb = &((unsigned short*)m_phy_fb)[y0 * display_width + x];
		for (; x <= x1; x++)
		{
			*fb++ = rgb;
			if (m_is_active && (x < display_width) && (y0 < display_height))
			{
				*phy_fb++ = rgb;
			}
		}
	}
}

unsigned int c_surface_16bits::get_pixel(int x, int y, unsigned int z_order)
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0 ||
		z_order >= Z_ORDER_LEVEL_MAX)
	{
		ASSERT(FALSE);
		return 0;
	}

	if (z_order == m_max_zorder)
	{
		return GLT_RGB_16_to_32(((unsigned short*)m_fb)[y * m_width + x]);
	}

	return GLT_RGB_16_to_32(((unsigned short*)(m_frame_layers[z_order].fb))[y * m_width + x]);
}