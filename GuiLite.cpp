#include "GuiLite.h" 
void c_bitmap::draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, int x, int y, unsigned int mask_rgb)
{
	if (0 == pBitmap)
	{
		return;
	}
	unsigned short* lower_fb = 0;
	int lower_fb_width = surface->m_width;
	if (z_order >= Z_ORDER_LEVEL_1)
	{
		lower_fb = surface->m_frame_layers[z_order - 1].fb;
	}
	unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
	int xsize = pBitmap->width;
	int ysize = pBitmap->height;
	const unsigned short* pData = (const unsigned short*)pBitmap->pixel_color_array;
	for (int j = 0; j < ysize; j++)
	{
		for (int i = 0; i < xsize; i++)
		{
			unsigned int rgb = *pData++;
			if (mask_rgb_16 == rgb)
			{
				if (lower_fb)
				{//restore lower layer
					surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(lower_fb[(y + j) * lower_fb_width + x + i]), z_order);
				}
			}
			else
			{
				surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(rgb), z_order);
			}
		}
	}
}
void c_bitmap::draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO* pBitmap, int x, int y, int src_x, int src_y, int width, int height, unsigned int mask_rgb)
{
	if (0 == pBitmap || (src_x + width > pBitmap->width) || (src_y + height > pBitmap->height))
	{
		return;
	}
	unsigned short* lower_fb = 0;
	int lower_fb_width = surface->m_width;
	if (z_order >= Z_ORDER_LEVEL_1)
	{
		lower_fb = surface->m_frame_layers[z_order - 1].fb;
	}
	unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
	const unsigned short* pData = (const unsigned short*)pBitmap->pixel_color_array;
	for (int j = 0; j < height; j++)
	{
		const unsigned short* p = &pData[src_x + (src_y + j) * pBitmap->width];
		for (int i = 0; i < width; i++)
		{
			unsigned int rgb = *p++;
			if (mask_rgb_16 == rgb)
			{
				if (lower_fb)
				{//restore lower layer
					surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(lower_fb[(y + j) * lower_fb_width + x + i]), z_order);
				}
			}
			else
			{
				surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(rgb), z_order);
			}
		}
	}
}
GL_MSG_ENTRY c_cmd_target::ms_usr_map_entries[USR_MSG_MAX];
unsigned short c_cmd_target::ms_user_map_size;
GL_BEGIN_MESSAGE_MAP(c_cmd_target)
GL_END_MESSAGE_MAP()
c_cmd_target::c_cmd_target()
{
}
c_cmd_target::~c_cmd_target()
{
}
int c_cmd_target::handle_usr_msg(unsigned int msgId, unsigned int wParam, unsigned int lParam)
{
	int i;
	c_cmd_target* p_wnd = 0;
	MSGFUNCS msg_funcs;
	for (i = 0; i < ms_user_map_size; i++)
	{
		if (msgId == ms_usr_map_entries[i].msgId)
		{
			p_wnd = (c_cmd_target*)ms_usr_map_entries[i].pObject;
			msg_funcs.func = ms_usr_map_entries[i].func;
			(p_wnd->*msg_funcs.func_vwl)(wParam , lParam);
		}
	}
	return 1;
}
void c_cmd_target::load_cmd_msg()
{
	const GL_MSG_ENTRY* p_entry = GetMSgEntries();
	if (0 == p_entry)
	{
		return;
	}
	bool bExist = false;
	while(MSG_TYPE_INVALID != p_entry->msgType)
	{
		if (MSG_TYPE_WND == p_entry->msgType)
		{
			p_entry++;
			continue;
		}
		bExist = false;
		for (int i = 0; i < ms_user_map_size; i++)
		{
			//repeat register, return.
			if (p_entry->msgId == ms_usr_map_entries[i].msgId
				&& ms_usr_map_entries[i].pObject == this)
			{
				bExist = true;
				break;
			}
		}
		if (true == bExist)
		{
			p_entry++;
			continue;
		}
		if (MSG_TYPE_USR == p_entry->msgType)
		{
			ms_usr_map_entries[ms_user_map_size] = *p_entry;
			ms_usr_map_entries[ms_user_map_size].pObject = this;
			ms_user_map_size++;
			if (USR_MSG_MAX == ms_user_map_size)
			{
				ASSERT(false);
			}
		}
		else
		{
			ASSERT(false);
			break;
		}
		p_entry++;
	}
}
const GL_MSG_ENTRY* c_cmd_target::FindMsgEntry(const GL_MSG_ENTRY *pEntry, 
	unsigned int msgType, unsigned short msgId, unsigned short ctrlId)
{
	if ( MSG_TYPE_INVALID == msgType)
	{
		return 0;
	}
	while (MSG_CALLBACK_NULL != pEntry->callbackType)
	{
		if ( (msgType == pEntry->msgType) && (msgId == pEntry->msgId) && (void*)(unsigned long)ctrlId == pEntry->pObject)
		{
			return pEntry;
		}
		pEntry++;
	}
	return 0;
}
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
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
c_rect::c_rect(const c_rect& rect)
{
	SetRect(rect.m_left,rect.m_top,rect.m_right,rect.m_bottom);
}
c_rect& c_rect::operator=(const c_rect& rect)
{
	SetRect(rect.m_left,rect.m_top,rect.m_right,rect.m_bottom);
	return *this;
}
void c_rect::SetRect( int Left, int Top, int Right, int Bottom)
{ 
	m_left = MIN(Left, Right);
	m_top = MIN(Top, Bottom);
	m_right = MAX(Left, Right);
	m_bottom = MAX(Top, Bottom);
}
c_rect  c_rect::operator&(const c_rect& rect) const
{
	c_rect  dst;
	dst.m_left = MAX(m_left, rect.m_left);
	dst.m_top = MAX(m_top, rect.m_top);
	dst.m_right = MIN(m_right, rect.m_right);
	dst.m_bottom = MIN(m_bottom, rect.m_bottom);
	if(dst.m_left >= dst.m_right || dst.m_top >= dst.m_bottom)
		dst.Empty();
	return dst;	
}
void c_rect::Empty()
{
	m_left = m_top = m_right = m_bottom = 0;
}
void c_rect::Offset(int x, int y)
{
	m_left +=x;
	m_right +=x;
	m_top += y;
	m_bottom += y;
}
int c_rect::IsEmpty() const
{
	return m_top == m_bottom || m_left == m_right;
}
int c_rect::PtInRect(int x, int y) const 
{
	return x >= m_left && x <= m_right && y >= m_top && y <= m_bottom;
}
int c_rect::operator==(const c_rect& rect) const
{
	return (m_left == rect.m_left) && (m_top==rect.m_top) &&
		(m_right == rect.m_right) && (m_bottom==rect.m_bottom);
}
#include <string.h>
#include <stdlib.h>
#define GL_ROUND_RGB_32(rgb) (rgb & 0xFFF8FCF8) //make RGB32 = RGB16
c_surface::c_surface(c_display* display,  unsigned int width, unsigned int height, unsigned int color_bytes)
{
	m_width = width;
	m_height = height;
	m_color_bytes = color_bytes;
	m_display = display;
	m_phy_fb = display->m_phy_fb;
	m_phy_write_index = &display->m_phy_write_index;
	m_fb = 0;
	m_top_zorder = m_max_zorder = Z_ORDER_LEVEL_0;
	m_is_active = false;
	m_frame_layers[Z_ORDER_LEVEL_0].visible_rect = c_rect(0, 0, m_width, m_height);
}
void c_surface::set_surface(Z_ORDER_LEVEL max_z_order)
{
	m_max_zorder = max_z_order;
	if (m_display->m_surface_cnt > 1)
	{
		m_fb = calloc(m_width * m_height, m_color_bytes);
	}
	
	for(int i = Z_ORDER_LEVEL_0; i < m_max_zorder; i++)
	{//Top layber fb always be 0
		m_frame_layers[i].fb = (unsigned short*)calloc(m_width * m_height, sizeof(unsigned short));
		ASSERT(0 != m_frame_layers[i].fb);
	}
}
void c_surface::draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order)
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0)
	{
		return;
	}
	if (z_order > m_max_zorder)
	{
		ASSERT(false);
		return;
	}
	rgb = GL_ROUND_RGB_32(rgb);
	if (z_order == m_max_zorder)
	{
		return draw_pixel_on_fb(x, y, rgb);
	}
	if (z_order > m_top_zorder)
	{
		m_top_zorder = (Z_ORDER_LEVEL)z_order;
	}
	if (0 == m_frame_layers[z_order].visible_rect.PtInRect(x, y))
	{
		ASSERT(false);
		return;
	}
	((unsigned short*)(m_frame_layers[z_order].fb))[x + y * m_width] = GL_RGB_32_to_16(rgb);
	if (z_order == m_top_zorder)
	{
		return draw_pixel_on_fb(x, y, rgb);
	}
	bool is_covered = false;
	for (int tmp_z_order = Z_ORDER_LEVEL_MAX - 1; tmp_z_order > z_order; tmp_z_order--)
	{
		if (true == m_frame_layers[tmp_z_order].visible_rect.PtInRect(x, y))
		{
			is_covered = true;
			break;
		}
	}
	if (!is_covered)
	{
		draw_pixel_on_fb(x, y, rgb);
	}
}
void c_surface::draw_pixel_on_fb(int x, int y, unsigned int rgb)
{
	if (m_fb)
	{
		(m_color_bytes == 4) ? ((unsigned int*)m_fb)[y * m_width + x] = rgb : ((unsigned short*)m_fb)[y * m_width + x] = GL_RGB_32_to_16(rgb);
	}
	int display_width = m_display->get_width();
	int display_height = m_display->get_height();
	if (m_is_active && (x < display_width) && (y < display_height))
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
void c_surface::fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order)
{
	x0 = (x0 < 0) ? 0 : x0;
	y0 = (y0 < 0) ? 0 : y0;
	x1 = (x1 > (m_width - 1)) ? (m_width - 1) : x1;
	y1 = (y1 > (m_height - 1)) ? (m_height - 1) : y1;
	rgb = GL_ROUND_RGB_32(rgb);
	if (z_order == m_max_zorder)
	{
		return fill_rect_on_fb(x0, y0, x1, y1, rgb);
	}
	if (z_order == m_top_zorder)
	{
		int x, y;
		unsigned short *mem_fb;
		unsigned int rgb_16 = GL_RGB_32_to_16(rgb);
		for (y = y0; y <= y1; y++)
		{
			x = x0;
			mem_fb = &((unsigned short*)m_frame_layers[z_order].fb)[y * m_width + x];
			for (; x <= x1; x++)
			{
				*mem_fb++ = rgb_16;
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
	int display_width = m_display->get_width();
	int display_height = m_display->get_height();
	if (m_color_bytes == 4)
	{
		int x;
		unsigned int *fb, *phy_fb;
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
	else if(m_color_bytes == 2)
	{
		int x;
		unsigned short *fb, *phy_fb;
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
unsigned int c_surface::get_pixel(int x, int y, unsigned int z_order)
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0 ||
			z_order >= Z_ORDER_LEVEL_MAX)
	{
		ASSERT(false);
		return 0;
	}
	if (z_order == m_max_zorder)
	{
		if (m_fb)
		{
			return (m_color_bytes == 4) ? ((unsigned int*)m_fb)[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)m_fb)[y * m_width + x]);
		}
		else if(m_phy_fb)
		{
			return (m_color_bytes == 4) ? ((unsigned int*)m_phy_fb)[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)m_phy_fb)[y * m_width + x]);
		}
		return 0;
	}
	
	unsigned short rgb_16 = ((unsigned short*)(m_frame_layers[z_order].fb))[y * m_width + x];
	return GL_RGB_16_to_32(rgb_16);
}
void c_surface::draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order)
{
	for (;x0 <= x1; x0++)
	{ draw_pixel(x0, y, rgb, z_order); }
}
void c_surface::draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order)
{
	for (;y0 <= y1; y0++)
	{ draw_pixel(x, y0, rgb, z_order); }
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
				draw_pixel(x1, y1, rgb, z_order);
				if (e>0) { y1++; e -= dx; }
			}
		}
		else
		{
			e = dx - dy / 2;
			for(; y1 <= y2; y1++, e += dx)
			{
				draw_pixel(x1, y1, rgb, z_order);
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
				draw_pixel(x1, y1, rgb, z_order);
				if (e>0) { y1--; e -= dx; }
			}
		}
		else
		{
			e = dx - dy / 2;
			for(; y1 >= y2; y1--, e += dx)
			{
				draw_pixel(x1, y1, rgb, z_order);
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
				draw_pixel(x1, y1, rgb, z_order);
				if (e>0) { y1++; e -= dx; }
			}
		}
		else
		{
			e = dx - dy / 2;
			for(; y1 <= y2; y1++, e += dx)
			{
				draw_pixel(x1, y1, rgb, z_order);
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
				draw_pixel(x1, y1, rgb, z_order);
				if (e>0) { y1--; e -= dx; }
			}
		}
		else
		{
			e = dx - dy / 2;
			while (y1-- >= y2)
			for(; y1 >= y2; y1--, e += dx)
			{
				draw_pixel(x1, y1, rgb, z_order);
				if (e>0) { x1--; e -= dy; }
			}
		}
	}
}
void c_surface::draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order, unsigned int size)
{
	for (unsigned int offset = 0; offset < size; offset++)
	{
		draw_hline(x0 + offset, x1 - offset, y0 + offset, rgb, z_order);
		draw_hline(x0 + offset, x1 - offset, y1 - offset, rgb, z_order);
		draw_vline(x0 + offset, y0 + offset, y1 - offset, rgb, z_order);
		draw_vline(x1 - offset, y0 + offset, y1 - offset, rgb, z_order);
	}
}
int c_surface::set_frame_layer_visible_rect(c_rect& rect, unsigned int z_order)
{
	if (rect == m_frame_layers[z_order].visible_rect)
	{
		return 0;
	}
	if (rect.m_left < 0 || rect.m_left >= m_width ||
		rect.m_right < 0 || rect.m_right >= m_width ||
		rect.m_top < 0 || rect.m_top >= m_height ||
		rect.m_bottom < 0 || rect.m_bottom >=m_height)
	{
		ASSERT(false);
		return -1;
	}
	if (!(z_order > Z_ORDER_LEVEL_0 && z_order < Z_ORDER_LEVEL_MAX))
	{
		ASSERT(false);
		return -2;
	}
	if (z_order < m_top_zorder)
	{
		ASSERT(false);
		return -3;
	}
	m_top_zorder = (Z_ORDER_LEVEL)z_order;
	
	c_rect old_rect = m_frame_layers[z_order].visible_rect;
	//Recover the lower layer
	int src_zorder = (Z_ORDER_LEVEL)(z_order - 1);
	int display_width = m_display->get_width();
	int display_height = m_display->get_height();
	for (int y = old_rect.m_top; y <= old_rect.m_bottom; y++)
	{
		for (int x = old_rect.m_left; x <= old_rect.m_right; x++)
		{
			if (!rect.PtInRect(x, y))
			{
				unsigned int rgb = ((unsigned short*)(m_frame_layers[src_zorder].fb))[x + y * m_width];
				draw_pixel_on_fb(x, y, GL_RGB_16_to_32(rgb));
			}
		}
	}
	m_frame_layers[z_order].visible_rect = rect;
	if (rect.IsEmpty())
	{
		m_top_zorder = (Z_ORDER_LEVEL)(z_order - 1);
	}
	return 0;
}
int c_surface::flush_screen(int left, int top, int right, int bottom)
{
	if(left < 0 || left >= m_width || right < 0 || right >= m_width ||
		top < 0 || top >= m_height || bottom < 0 || bottom >= m_height)
	{
		ASSERT(false);
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
	*m_phy_write_index = *m_phy_write_index + 1;
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
void c_surface_no_fb::fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb)
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
	if(m_color_bytes == 4)
	{
		unsigned int *fb;
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
		unsigned short *fb;
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
void c_surface_no_fb::draw_pixel_on_fb(int x, int y, unsigned int rgb)
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

static const FONT_INFO* s_font_map[FONT_MAX];
static const BITMAP_INFO* s_bmp_map[BITMAP_MAX];
static unsigned int s_color_map[COLOR_MAX];

int c_theme::add_font(FONT_TYPE index, const FONT_INFO* font)
{
	if (index >= FONT_MAX)
	{
		ASSERT(false);
		return -1;
	}
	s_font_map[index] = font;
	return 0;
}

const FONT_INFO* c_theme::get_font(FONT_TYPE index)
{
	if (index >= FONT_MAX)
	{
		ASSERT(false);
		return 0;
	}
	return s_font_map[index];
}

int c_theme::add_bitmap(BITMAP_TYPE index, const BITMAP_INFO* bmp)
{
	if (index >= BITMAP_MAX)
	{
		ASSERT(false);
		return -1;
	}
	s_bmp_map[index] = bmp;
	return 0;
}

const BITMAP_INFO* c_theme::get_bmp(BITMAP_TYPE index)
{
	if (index >= BITMAP_MAX)
	{
		ASSERT(false);
		return 0;
	}
	return s_bmp_map[index];
}

int c_theme::add_color(COLOR_TYPE index, const unsigned int color)
{
	if (index >= COLOR_MAX)
	{
		ASSERT(false);
		return -1;
	}
	s_color_map[index] = color;
	return 0;
}

const unsigned int c_theme::get_color(COLOR_TYPE index)
{
	if (index >= COLOR_MAX)
	{
		ASSERT(false);
		return 0;
	}
	return s_color_map[index];
}
c_wnd::c_wnd(): m_status(STATUS_NORMAL), m_attr(ATTR_VISIBLE), m_parent(0), m_top_child(0), m_prev_sibling(0), m_next_sibling(0),
m_str(0), m_font_color(0), m_bg_color(0), m_resource_id(0),	m_z_order(Z_ORDER_LEVEL_0),	m_focus_child(0), m_surface(0)
{
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
}
int c_wnd::connect(c_wnd *parent, unsigned short resource_id, const char* str,
		   short x, short y, short width, short height, WND_TREE* p_child_tree )
{
	if(0 == resource_id)
	{
		ASSERT(false);
		return -1;
	}
	m_resource_id = resource_id;
	set_str(str);
	m_parent  = parent;
	m_status = STATUS_NORMAL;
	if (parent)
	{
		m_z_order = parent->m_z_order;
		m_surface = parent->m_surface;
	}
	if(0 == m_surface)
	{
		ASSERT(false);
		return -2;
	}
	/* (cs.x = x * 1024 / 768) for 1027*768=>800*600 quickly*/
	m_wnd_rect.m_left	= x;
	m_wnd_rect.m_top    = y;
	m_wnd_rect.m_right	= (x + width - 1);
	m_wnd_rect.m_bottom = (y + height - 1);
	c_rect rect;
	get_screen_rect(rect);
	ASSERT(m_surface->is_valid(rect));
	pre_create_wnd();
	if ( 0 != parent )
	{
		parent->add_child_2_tail(this);
	}
	if (load_child_wnd(p_child_tree) >= 0)
	{
		load_cmd_msg();
		on_init_children();
	}
	return 0;
}
int c_wnd::load_child_wnd(WND_TREE *p_child_tree)
{
	if (0 == p_child_tree)
	{
		return 0;
	}
	int sum = 0;
	WND_TREE* p_cur = p_child_tree;
	while(p_cur->p_wnd)
	{
		if (0 != p_cur->p_wnd->m_resource_id)
		{//This wnd has been used! Do not share!
			ASSERT(false);
			return -1;
		}
		else
		{
			p_cur->p_wnd->connect(this, p_cur->resource_id, p_cur->str,
				p_cur->x, p_cur->y, p_cur->width, p_cur->height,p_cur->p_child_tree);
		}
		p_cur++;
		sum++;
	}
	return sum;
}
c_wnd* c_wnd::connect_clone(c_wnd *parent, unsigned short resource_id, const char* str,
		   short x, short y, short width, short height, WND_TREE* p_child_tree )
{
	if(0 == resource_id)
	{
		ASSERT(false);
		return 0;
	}
	c_wnd* wnd = clone();
	wnd->m_resource_id = resource_id;
	wnd->set_str(str);
	wnd->m_parent  = parent;
	wnd->m_status = STATUS_NORMAL;
	if (parent)
	{
		wnd->m_z_order =  parent->m_z_order;
		wnd->m_surface = parent->m_surface;
	}
	else
	{
		wnd->m_surface = m_surface;
	}
	if(0 == wnd->m_surface)
	{
		ASSERT(false);
		return 0;
	}
	/* (cs.x = x * 1024 / 768) for 1027*768=>800*600 quickly*/
	wnd->m_wnd_rect.m_left   = x;
	wnd->m_wnd_rect.m_top    = y;
	wnd->m_wnd_rect.m_right  = (x + width - 1);
	wnd->m_wnd_rect.m_bottom = (y + height - 1);
	c_rect rect;
	wnd->get_screen_rect(rect);
	ASSERT(wnd->m_surface->is_valid(rect));
	wnd->pre_create_wnd();
	
	if ( 0 != parent )
	{
		parent->add_child_2_tail(wnd);
	}
	if (wnd->load_clone_child_wnd(p_child_tree) >= 0)
	{
		wnd->load_cmd_msg();
		wnd->on_init_children();
	}
	return wnd;
}
int c_wnd::load_clone_child_wnd(WND_TREE *p_child_tree)
{
	if (0 == p_child_tree)
	{
		return 0;
	}
	int sum = 0;
	WND_TREE* p_cur = p_child_tree;
	while(p_cur->p_wnd)
	{
		p_cur->p_wnd->connect_clone(this, p_cur->resource_id, p_cur->str,
									p_cur->x, p_cur->y, p_cur->width, p_cur->height,p_cur->p_child_tree);
		p_cur++;
		sum++;
	}
	return sum;
}
void c_wnd::disconnect()
{
	if (0 == m_resource_id)
	{
		return;
	}
	if (0 != m_top_child)
	{
		c_wnd *child = m_top_child;
		c_wnd *next_child = 0;
		while (child)
		{
			next_child = child->m_next_sibling;
			child->disconnect();
			child = next_child;
		}
	}
	if (0 != m_parent)
	{
		m_parent->unlink_child(this);
	}
	m_focus_child = 0;
	m_resource_id = 0;
}
c_wnd* c_wnd::get_wnd_ptr(unsigned short id) const
{
	c_wnd *child = m_top_child;
	while ( child )
	{
		if ( child->get_id() == id )
		{
			break;
		}
		child = child->m_next_sibling;
	}
	return child;
}
void c_wnd::set_attr(WND_ATTRIBUTION attr)
{
	m_attr = attr;
	if ( ATTR_DISABLED == (attr & ATTR_DISABLED) )
	{
		m_status = STATUS_DISABLED;
	}
	else
	{
		if (m_status == STATUS_DISABLED)
		{
			m_status = STATUS_NORMAL;
		}
	}
}
int c_wnd::is_focus_wnd() const
{
	if ( (m_attr & ATTR_VISIBLE)
		&& !(m_attr & ATTR_DISABLED)
		&& (m_attr & ATTR_FOCUS))
	{
		return true;
	}
	else
	{
		return false;
	}
}
void c_wnd::set_wnd_pos(short x, short y, short width, short height)
{
	m_wnd_rect.m_left   = x;
	m_wnd_rect.m_top    = y;
	m_wnd_rect.m_right  = x + width - 1;
	m_wnd_rect.m_bottom = y + height - 1;
}
void c_wnd::get_wnd_rect(c_rect &rect) const
{
	rect = m_wnd_rect;
}
void c_wnd::get_screen_rect(c_rect &rect) const
{
	rect.SetRect(0, 0, (m_wnd_rect.Width() - 1), (m_wnd_rect.Height() - 1));
	wnd2screen(rect);
}
void c_wnd::wnd2screen(int &x, int &y) const
{
	c_wnd *parent = m_parent;
	c_rect rect;
	x += m_wnd_rect.m_left;
	y += m_wnd_rect.m_top;
	while ( 0 != parent )
	{
		parent->get_wnd_rect(rect);
		x += rect.m_left;
		y += rect.m_top;
		parent = parent->m_parent;
	}
}
void c_wnd::wnd2screen(c_rect &rect) const
{
	int l = rect.m_left;
	int t = rect.m_top;
	wnd2screen(l, t);
	int r = (l + rect.Width() - 1);
	int b = (t + rect.Height() - 1);
	rect.SetRect(l, t, r, b);
}
c_wnd* c_wnd::set_child_focus(c_wnd * focus_child)
{
	ASSERT(0 != focus_child);
	ASSERT(focus_child->m_parent == this);
	c_wnd *old_focus_child = m_focus_child;
	if (focus_child->is_focus_wnd())
	{
		if (focus_child != old_focus_child )
		{
			if (old_focus_child)
			{
				old_focus_child->on_kill_focus();
			}
			m_focus_child = focus_child;
			if (m_parent)
			{
				m_parent->set_child_focus(this);
			}
			m_focus_child->on_focus();
		}
	}
	return m_focus_child;
}
void c_wnd::add_child_2_tail(c_wnd *child)
{
	if( 0 == child )return;
	if(child == get_wnd_ptr(child->m_resource_id))return;
	if ( 0 == m_top_child )
	{
		m_top_child = child;
		child->m_prev_sibling = 0;
		child->m_next_sibling = 0;
	}
	else
	{
		c_wnd *last_child = get_last_child();
		if (0 == last_child)
		{
			ASSERT(false);
		}
		last_child->m_next_sibling = child;
		child->m_prev_sibling = last_child;
		child->m_next_sibling = 0;
	}
}
c_wnd* c_wnd::get_last_child() const
{
	if ( 0 == m_top_child )
	{
		return 0;
	}
	c_wnd *child = m_top_child;
	while ( child->m_next_sibling)
	{
		child = child->m_next_sibling;
	}
	return child;
}
int	c_wnd::unlink_child(c_wnd *child)
{
	if ((0 == child)
		|| (this != child->m_parent))
	{
		return -1;
	}
	if (0 == m_top_child)
	{
		return -2;
	}
	int find = false;
	c_wnd *tmp_child = m_top_child;
	if (tmp_child == child)
	{
		m_top_child = child->m_next_sibling;
		if (0 != child->m_next_sibling)
		{
			child->m_next_sibling->m_prev_sibling = 0;
		}
		find = true;
	}
	else
	{
		while (tmp_child->m_next_sibling)
		{
			if (child == tmp_child->m_next_sibling)
			{
				tmp_child->m_next_sibling = child->m_next_sibling;
				if (0 != child->m_next_sibling)
				{
					child->m_next_sibling->m_prev_sibling = tmp_child;
				}
				find = true;
				break;
			}
			tmp_child = tmp_child->m_next_sibling;
		}
	}
	if (true == find)
	{
		if (m_focus_child == child)
		{
			m_focus_child = 0;
		}
		child->m_next_sibling = 0;
		child->m_prev_sibling = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
void c_wnd::show_window()
{
	if (ATTR_VISIBLE == (m_attr & ATTR_VISIBLE))
	{
		on_paint();
		c_wnd *child = m_top_child;
		if ( 0 != child )
		{
			while ( child )
			{
				child->show_window();
				child = child->m_next_sibling;
			}
		}
	}
}
bool c_wnd::on_touch(int x, int y, TOUCH_ACTION action)
{
	c_rect rect;
	x -= m_wnd_rect.m_left;
	y -= m_wnd_rect.m_top;
	c_wnd* child = m_top_child;
	c_wnd* target_wnd = 0;
	int target_z_order = Z_ORDER_LEVEL_0;
	while (child)
	{
		if (ATTR_VISIBLE == (child->m_attr & ATTR_VISIBLE))
		{
			child->get_wnd_rect(rect);
			if (true == rect.PtInRect(x, y) || child->m_attr & ATTR_MODAL)
			{
				if (true == child->is_focus_wnd())
				{
					if (child->m_z_order >= target_z_order)
					{
						target_wnd = child;
						target_z_order = child->m_z_order;
					}
				}
			}
		}
		child = child->m_next_sibling;
	}
	if (target_wnd == 0)
	{
		return false;
	}
	return target_wnd->on_touch(x, y, action);
}
bool c_wnd::on_key(KEY_TYPE key)
{
	ASSERT(key == KEY_FORWARD || key == KEY_BACKWARD || key == KEY_ENTER);
	// Find current focus wnd.
	c_wnd* old_focus_wnd = m_focus_child;
	while (m_focus_child && m_focus_child->m_focus_child)
	{
		old_focus_wnd = m_focus_child->m_focus_child;
	}
	if (old_focus_wnd && !old_focus_wnd->on_key(key))
	{
		return true;
	}
	// Default moving focus(Default handle KEY_FOWARD/KEY_BACKWARD)
	if (key == KEY_ENTER)
	{
		return true;
	}
	if (!old_focus_wnd)
	{// No current focus wnd, new one.
		c_wnd *child = m_top_child;
		c_wnd *new_focus_wnd = 0;
		while (child)
		{
			if (ATTR_VISIBLE == (child->m_attr & ATTR_VISIBLE))
			{
				if (true == child->is_focus_wnd())
				{
					new_focus_wnd = child;
					new_focus_wnd->m_parent->set_child_focus(new_focus_wnd);
					child = child->m_top_child;
					continue;
				}
			}
			child = child->m_next_sibling;
		}
		return true;
	}
	// Move focus from old wnd to next wnd
	c_wnd* next_focus_wnd = (key == KEY_FORWARD) ? old_focus_wnd->m_next_sibling : old_focus_wnd->m_prev_sibling;
	while (next_focus_wnd && (!next_focus_wnd->is_focus_wnd()))
	{// Search neighbor of old focus wnd
		next_focus_wnd = (key == KEY_FORWARD) ? next_focus_wnd->m_next_sibling : next_focus_wnd->m_prev_sibling;
	}
	if (!next_focus_wnd)
	{// Search whole brother wnd
		next_focus_wnd = (key == KEY_FORWARD) ? old_focus_wnd->m_parent->m_top_child : old_focus_wnd->m_parent->get_last_child();
		while (next_focus_wnd && (!next_focus_wnd->is_focus_wnd()))
		{
			next_focus_wnd = (key == KEY_FORWARD) ? next_focus_wnd->m_next_sibling : next_focus_wnd->m_prev_sibling;
		}
	}
	if (next_focus_wnd)
	{
		next_focus_wnd->m_parent->set_child_focus(next_focus_wnd);
	}
	return true;
}
void c_wnd::notify_parent(unsigned int msg_id, int param)
{
	if (!m_parent)
	{
		return;
	}
	const GL_MSG_ENTRY* entry = m_parent->FindMsgEntry(m_parent->GetMSgEntries(), MSG_TYPE_WND, msg_id, m_resource_id);
	if (0 == entry)
	{
		return;
	}
	MSGFUNCS msg_funcs;
	msg_funcs.func = entry->func;
	switch (entry->callbackType)
	{
	case MSG_CALLBACK_VV:
		(m_parent->*msg_funcs.func)();
		break;
	case MSG_CALLBACK_VVL:
		(m_parent->*msg_funcs.func_vvl)(param);
		break;
	case MSG_CALLBACK_VWV:
		(m_parent->*msg_funcs.func_vwv)(m_resource_id);
		break;
	case MSG_CALLBACK_VWL:
		(m_parent->*msg_funcs.func_vwl)(m_resource_id, param);
		break;
	default:
		ASSERT(false);
		break;
	}
}
#include <string.h>
#include <stdio.h>
#define BUFFER_LEN	16
unsigned char s_utf8_length_table[256] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};
inline static int get_utf8_code(const char* s, unsigned int& output_utf8_code)
{
	unsigned char* us = (unsigned char*)s;
	int utf8_bytes = s_utf8_length_table[*us];
	switch (utf8_bytes)
	{
	case 1:
		output_utf8_code = *us;
		break;
	case 2:
		output_utf8_code = (*us << 8) | (*(us + 1));
		break;
	case 3:
		output_utf8_code = (*us << 16) | ((*(us + 1)) << 8) | *(us + 2);
		break;
	case 4:
		output_utf8_code = (*us << 24) | ((*(us + 1)) << 16) | (*(us + 2) << 8) | *(us + 3);
		break;
	default:
		ASSERT(false);
		break;
	}
	return utf8_bytes;
}
void c_word::draw_value_in_rect(c_surface* surface, int z_order, int value, int dot_position, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
	char buf[BUFFER_LEN];
	value_2_string(value, dot_position, buf, BUFFER_LEN);
	draw_string_in_rect(surface, z_order, buf, rect, font, font_color, bg_color, align_type);
}
void c_word::draw_value(c_surface* surface, int z_order, int value, int dot_position, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
	char buf[BUFFER_LEN];
	value_2_string(value, dot_position, buf, BUFFER_LEN);
	draw_string(surface, z_order, buf, x, y, font, font_color, bg_color, align_type);
}
void c_word::draw_string_in_rect(c_surface* surface, int z_order, const char *s, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
	if(0 == s)
	{
		return;
	}
	int x, y;
	get_string_pos(s, font, rect, align_type, x, y);
	draw_string(surface, z_order, s, rect.m_left + x, rect.m_top + y, font, font_color, bg_color, ALIGN_LEFT);
}
void c_word::draw_string(c_surface* surface, int z_order, const char *s, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
	if (0 == s)
	{
		return;
	}
	int offset = 0;
	unsigned int utf8_code;
	while (*s)
	{
		s += get_utf8_code(s, utf8_code);
		offset += draw_single_char(surface, z_order, utf8_code, (x + offset), y, font, font_color, bg_color);
	}
}
void c_word::value_2_string(int value, int dot_position, char* buf, int len)
{
	memset(buf, 0, len);
	switch (dot_position)
	{
	case 0:
		sprintf(buf, "%d", value);
		break;
	case 1:
		sprintf(buf, "%.1f", value*1.0 / 10);
		break;
	case 2:
		sprintf(buf, "%.2f", value*1.0 / 100);
		break;
	case 3:
		sprintf(buf, "%.3f", value*1.0 / 1000);
		break;
	default:
		ASSERT(false);
		break;
	}
}
const LATTICE* c_word::get_lattice(const FONT_INFO* font, unsigned int utf8_code)
{
	int first = 0;
	int last = font->count - 1;
	int middle = (first + last) / 2;
	while (first <= last) 
	{
		if (font->lattice_array[middle].utf8_code < utf8_code)
			first = middle + 1;
		else if (font->lattice_array[middle].utf8_code == utf8_code)
		{
			return &font->lattice_array[middle];
		}
		else
		{
			last = middle - 1;
		}
		middle = (first + last) / 2;
	}
	return 0;
}
int c_word::draw_single_char(c_surface* surface, int z_order, unsigned int utf8_code, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color)
{
	unsigned int error_color = 0xFFFFFFFF;
	if (font)
	{
		const LATTICE* p_lattice = get_lattice(font, utf8_code);
		if (p_lattice)
		{
			draw_lattice(surface, z_order, x, y, p_lattice->width, font->height, p_lattice->pixel_gray_array, font_color, bg_color);
			return p_lattice->width;
		}
	}
	else
	{
		error_color = GL_RGB(255, 0, 0);
	}
	
	//lattice/font not found, draw "X"
	int len = 16;
	for (int y_ = 0; y_ < len; y_++)
	{
		for (int x_ = 0; x_ < len; x_++)
		{
			int diff = (x_ - y_);
			int sum = (x_ + y_);
			(diff == 0 || diff == -1 || diff == 1 || sum == len || sum == (len - 1) || sum == (len + 1)) ?
			surface->draw_pixel((x + x_), (y + y_), error_color, z_order) : surface->draw_pixel((x + x_), (y + y_), 0, z_order);
		}
	}
	return len;
}
void c_word::draw_lattice(c_surface* surface, int z_order, int x, int y, int width, int height,
						const unsigned char* p_data, unsigned int font_color, unsigned int bg_color)
{
	unsigned int r, g, b, rgb;
	unsigned char blk_value = *p_data++;
	unsigned char blk_cnt = *p_data++;
	b = (GL_RGB_B(font_color) * blk_value + GL_RGB_B(bg_color) * (255 - blk_value)) >> 8;
	g = (GL_RGB_G(font_color) * blk_value + GL_RGB_G(bg_color) * (255 - blk_value)) >> 8;
	r = (GL_RGB_R(font_color) * blk_value + GL_RGB_R(bg_color) * (255 - blk_value)) >> 8;
	rgb = GL_RGB(r, g, b);
	for (int y_ = 0; y_ < height; y_++)
	{
		for (int x_ = 0; x_ < width; x_++)
		{
			ASSERT(blk_cnt);
			if (0x00 == blk_value)
			{
				if (GL_ARGB_A(bg_color))
				{
					surface->draw_pixel(x + x_, y + y_, bg_color, z_order);
				}
			}
			else
			{
				surface->draw_pixel((x + x_), (y + y_), rgb, z_order);
			}
			if (--blk_cnt == 0)
			{//reload new block
				blk_value = *p_data++;
				blk_cnt = *p_data++;
				b = (GL_RGB_B(font_color) * blk_value + GL_RGB_B(bg_color) * (255 - blk_value)) >> 8;
				g = (GL_RGB_G(font_color) * blk_value + GL_RGB_G(bg_color) * (255 - blk_value)) >> 8;
				r = (GL_RGB_R(font_color) * blk_value + GL_RGB_R(bg_color) * (255 - blk_value)) >> 8;
				rgb = GL_RGB(r, g, b);
			}
		}
	}
}
int c_word::get_str_size(const char *s, const FONT_INFO* font, int& width, int& height)
{
	if(0 == s || 0 == font)
	{
		width = height = 0;
		return -1;
	}
	int lattice_width = 0;
	unsigned int utf8_code;
	int utf8_bytes;
	while (*s)
	{
		utf8_bytes = get_utf8_code(s, utf8_code);
		const LATTICE* p_lattice = get_lattice(font, utf8_code);
		lattice_width += p_lattice?p_lattice->width:font->height;
		s += utf8_bytes;
	}
	width = lattice_width;
	height = font->height;
	return 0;
}
void c_word::get_string_pos(const char *s, const FONT_INFO* font, c_rect rect, unsigned int align_type, int &x, int &y)
{
	int x_size, y_size;
	get_str_size(s, font, x_size, y_size);
	int height = rect.m_bottom - rect.m_top + 1;
	int width  = rect.m_right - rect.m_left + 1;
	x = y = 0;
	switch (align_type & ALIGN_HMASK)
	{
	case ALIGN_HCENTER:
		//m_text_org_x=0
		if (width > x_size)
		{
			x = (width - x_size)/2;
		}
		break;
	case ALIGN_LEFT:
		x = 0;
		break;
	case ALIGN_RIGHT:
		//m_text_org_x=0
		if (width > x_size)
		{
			x = width - x_size;
		}
		break;
	default:
		ASSERT(0);
		break;
	}
	switch (align_type & ALIGN_VMASK)
	{
	case ALIGN_VCENTER:
		//m_text_org_y=0
		if (height > y_size)
		{
			y = (height - y_size)/2;
		}
		break;
	case ALIGN_TOP:
		y = 0;
		break;
	case ALIGN_BOTTOM:
		//m_text_org_y=0
		if (height > y_size)
		{
			y = height - y_size;
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}
#if (defined __linux__) || (defined __APPLE__)
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/times.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_TIMER_CNT 10
#define TIMER_UNIT 50//ms
static void(*do_assert)(const char* file, int line);
static void(*do_log_out)(const char* log);
void register_debug_function(void(*my_assert)(const char* file, int line), void(*my_log_out)(const char* log))
{
	do_assert = my_assert;
	do_log_out = my_log_out;
}
void _assert(const char* file, int line)
{
	if(do_assert)
	{
		do_assert(file, line);
	}
	else
	{
		printf("assert@ file:%s, line:%d, error no: %d\n", file, line, errno);
	} 
}
void log_out(const char* log)
{
	if (do_log_out)
	{
		do_log_out(log);
	}
	else
	{
		printf("%s", log);
		fflush(stdout);
	}
}
typedef struct _timer_manage
{
    struct  _timer_info
    {
        int state; /* on or off */
        int interval;
        int elapse; /* 0~interval */
        void (* timer_proc) (void* ptmr, void* parg);
    }timer_info[MAX_TIMER_CNT];
    void (* old_sigfunc)(int);
    void (* new_sigfunc)(int);
}_timer_manage_t;
static struct _timer_manage timer_manage;
static void* timer_routine(void*)
{
    int i;
    while(true)
    {
    	for(i = 0; i < MAX_TIMER_CNT; i++)
		{
			if(timer_manage.timer_info[i].state == 0)
			{
				continue;
			}
			timer_manage.timer_info[i].elapse++;
			if(timer_manage.timer_info[i].elapse == timer_manage.timer_info[i].interval)
			{
				timer_manage.timer_info[i].elapse = 0;
				timer_manage.timer_info[i].timer_proc(0, 0);
			}
		}
    	usleep(1000 * TIMER_UNIT);
    }
    return NULL;
}
static int init_mul_timer()
{
	static bool s_is_init = false;
	if(s_is_init == true)
	{
		return 0;
	}
    memset(&timer_manage, 0, sizeof(struct _timer_manage));
    pthread_t pid;
    pthread_create(&pid, NULL, timer_routine, NULL);
    s_is_init = true;
    return 1;
}
static int set_a_timer(int interval, void (* timer_proc) (void* ptmr, void* parg))
{
	init_mul_timer();
	int i;
    if(timer_proc == NULL || interval <= 0)
    {
        return (-1);
    }
    for(i = 0; i < MAX_TIMER_CNT; i++)
    {
        if(timer_manage.timer_info[i].state == 1)
        {
            continue;
        }
        memset(&timer_manage.timer_info[i], 0, sizeof(timer_manage.timer_info[i]));
        timer_manage.timer_info[i].timer_proc = timer_proc;
        timer_manage.timer_info[i].interval = interval;
        timer_manage.timer_info[i].elapse = 0;
        timer_manage.timer_info[i].state = 1;
        break;
    }
    if(i >= MAX_TIMER_CNT)
    {
    	ASSERT(false);
        return (-1);
    }
    return (i);
}
typedef void (*EXPIRE_ROUTINE)(void* arg);
EXPIRE_ROUTINE s_expire_function;
static c_fifo s_real_timer_fifo;
static void* real_timer_routine(void*)
{
	char dummy;
	while(1)
	{
		if(s_real_timer_fifo.read(&dummy, 1) > 0)
		{
			if(s_expire_function)s_expire_function(0);
		}
		else
		{
			ASSERT(false);
		}
	}
	return 0;
}
static void expire_real_timer(int sigo)
{
	char dummy = 0x33;
	if(s_real_timer_fifo.write(&dummy, 1) <= 0)
	{
		ASSERT(false);
	}
}
void start_real_timer(void (*func)(void* arg))
{
	if(NULL == func)
	{
		return;
	}
	s_expire_function = func;
	signal(SIGALRM, expire_real_timer);
	struct itimerval value, ovalue;
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = REAL_TIME_TASK_CYCLE_MS * 1000;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = REAL_TIME_TASK_CYCLE_MS * 1000;
	setitimer(ITIMER_REAL, &value, &ovalue);
	static pthread_t s_pid;
	if(s_pid == 0)
	{
		pthread_create(&s_pid, NULL, real_timer_routine, NULL);
	}
}
unsigned int get_cur_thread_id()
{
	return (unsigned long)pthread_self();
}
void register_timer(int milli_second,void func(void* ptmr, void* parg))
{
	set_a_timer(milli_second/TIMER_UNIT,func);
}
long get_time_in_second()
{
	return time(NULL);         /* + 8*60*60*/
}
T_TIME get_time()
{
	T_TIME ret = {0};
	struct tm *fmt;
	time_t timer;
	timer = get_time_in_second();
	fmt = localtime(&timer);
	ret.year   = fmt->tm_year + 1900;
	ret.month  = fmt->tm_mon + 1;
	ret.day    = fmt->tm_mday;
	ret.hour   = fmt->tm_hour;
	ret.minute = fmt->tm_min;
	ret.second = fmt->tm_sec;
	return ret;
}
T_TIME second_to_day(long second)
{
	T_TIME ret = {0};
	struct tm *fmt;
	fmt = localtime(&second);
	ret.year   = fmt->tm_year + 1900;
	ret.month  = fmt->tm_mon + 1;
	ret.day    = fmt->tm_mday;
	ret.hour   = fmt->tm_hour;
	ret.minute = fmt->tm_min;
	ret.second = fmt->tm_sec;
	return ret;
}
void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg)
{
    pthread_create((pthread_t*)thread_id, (pthread_attr_t const*)attr, start_routine, arg);
}
void thread_sleep(unsigned int milli_seconds)
{
	usleep(milli_seconds * 1000);
}
typedef struct {
	unsigned short	bfType;
	unsigned int   	bfSize;
	unsigned short  bfReserved1;
	unsigned short  bfReserved2;
	unsigned int   	bfOffBits;
}__attribute__((packed))FileHead;
typedef struct{
	unsigned int  	biSize;
	int 			biWidth;
	int       		biHeight;
	unsigned short	biPlanes;
	unsigned short  biBitCount;
	unsigned int    biCompress;
	unsigned int    biSizeImage;
	int       		biXPelsPerMeter;
	int       		biYPelsPerMeter;
	unsigned int 	biClrUsed;
	unsigned int    biClrImportant;
	unsigned int 	biRedMask;
	unsigned int 	biGreenMask;
	unsigned int 	biBlueMask;
}__attribute__((packed))Infohead;
int build_bmp(const char *filename, unsigned int width, unsigned int height, unsigned char *data)
{
	FileHead bmp_head;
	Infohead bmp_info;
	int size = width * height * 2;
	//initialize bmp head.
	bmp_head.bfType = 0x4d42;
	bmp_head.bfSize = size + sizeof(FileHead) + sizeof(Infohead);
	bmp_head.bfReserved1 = bmp_head.bfReserved2 = 0;
	bmp_head.bfOffBits = bmp_head.bfSize - size;
	//initialize bmp info.
	bmp_info.biSize = 40;
	bmp_info.biWidth = width;
	bmp_info.biHeight = height;
	bmp_info.biPlanes = 1;
	bmp_info.biBitCount = 16;
	bmp_info.biCompress = 3;
	bmp_info.biSizeImage = size;
	bmp_info.biXPelsPerMeter = 0;
	bmp_info.biYPelsPerMeter = 0;
	bmp_info.biClrUsed = 0;
	bmp_info.biClrImportant = 0;
	//RGB565
	bmp_info.biRedMask = 0xF800;
	bmp_info.biGreenMask = 0x07E0;
	bmp_info.biBlueMask = 0x001F;
	//copy the data
	FILE *fp;
	if(!(fp=fopen(filename,"wb")))
	{
		return -1;
	}
	fwrite(&bmp_head, 1, sizeof(FileHead),fp);
	fwrite(&bmp_info, 1, sizeof(Infohead),fp);
	//fwrite(data, 1, size, fp);//top <-> bottom
	for (int i = (height - 1); i >= 0; --i)
	{
		fwrite(&data[i * width * 2], 1, width * 2, fp);
	}
	
	fclose(fp);
	return 0;
}
c_fifo::c_fifo()
{
	m_head = m_tail = 0;
	m_read_sem = malloc(sizeof(sem_t));
	m_write_mutex = malloc(sizeof(pthread_mutex_t));
	
	sem_init((sem_t*)m_read_sem, 0, 0);
	pthread_mutex_init((pthread_mutex_t*)m_write_mutex, 0);
}
int c_fifo::read(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	while(i < len)
	{
		if (m_tail == m_head)
		{//empty
			sem_wait((sem_t*)m_read_sem);
			continue;
		}
		*pbuf++ = m_buf[m_head];
		m_head = (m_head + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	if(i != len)
	{
		ASSERT(false);
	}
	return i;
}
int c_fifo::write(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	int tail = m_tail;
	pthread_mutex_lock((pthread_mutex_t*)m_write_mutex);
	while(i < len)
	{
		if ((m_tail + 1) % FIFO_BUFFER_LEN == m_head)
		{//full, clear data has been written;
			m_tail = tail;
			log_out("Warning: fifo full\n");
			pthread_mutex_unlock((pthread_mutex_t*)m_write_mutex);
			return 0;
		}
		m_buf[m_tail] = *pbuf++;
		m_tail = (m_tail + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	pthread_mutex_unlock((pthread_mutex_t*)m_write_mutex);
	if(i != len)
	{
		ASSERT(false);
	}
	else
	{
		sem_post((sem_t*)m_read_sem);
	}
	return i;
}
#endif
#if (!defined _WIN32) && (!defined WIN32) && (!defined _WIN64) && (!defined WIN64) && (!defined __linux__) && (!defined __APPLE__)

#include <stdio.h>

static void(*do_assert)(const char* file, int line);
static void(*do_log_out)(const char* log);
void register_debug_function(void(*my_assert)(const char* file, int line), void(*my_log_out)(const char* log))
{
	do_assert = my_assert;
	do_log_out = my_log_out;
}

void _assert(const char* file, int line)
{
	if(do_assert)
	{
		do_assert(file, line);
	}
	while(1);
}

void log_out(const char* log)
{
	if (do_log_out)
	{
		do_log_out(log);
	}
}

long get_time_in_second()
{
    return 0;
}

T_TIME second_to_day(long second)
{
    T_TIME ret = {0};
    return ret;
}

T_TIME get_time()
{
    T_TIME ret = {0};
    return ret;
}

void start_real_timer(void (*func)(void* arg))
{
    log_out("Not support now");
}

void register_timer(int milli_second, void func(void* ptmr, void* parg))
{
    log_out("Not support now");
}

unsigned int get_cur_thread_id()
{
    log_out("Not support now");
    return 0;
}

void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg)
{
    log_out("Not support now");
}

extern "C" void delay_ms(unsigned short nms);
void thread_sleep(unsigned int milli_seconds)
{//MCU alway implemnet driver code in APP.
		delay_ms(milli_seconds);
}

int build_bmp(const char *filename, unsigned int width, unsigned int height, unsigned char *data)
{
    log_out("Not support now");
    return 0;
}

c_fifo::c_fifo()
{
	m_head = m_tail = 0;
	m_read_sem = m_write_mutex = 0;
}

int c_fifo::read(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	while(i < len)
	{
		if (m_tail == m_head)
		{//empty
			continue;
		}
		*pbuf++ = m_buf[m_head];
		m_head = (m_head + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	if(i != len)
	{
		ASSERT(false);
	}
	return i;
}

int c_fifo::write(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	int tail = m_tail;

	while(i < len)
	{
		if ((m_tail + 1) % FIFO_BUFFER_LEN == m_head)
		{//full, clear data has been written;
			m_tail = tail;
			log_out("Warning: fifo full\n");
			return 0;
		}
		m_buf[m_tail] = *pbuf++;
		m_tail = (m_tail + 1) % FIFO_BUFFER_LEN;
		i++;
	}

	if(i != len)
	{
		ASSERT(false);
	}
	return i;
}

#endif
#if (defined _WIN32) || (defined WIN32) || (defined _WIN64) || (defined WIN64)
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <assert.h>
#define MAX_TIMER_CNT 10
#define TIMER_UNIT 50//ms
static void(*do_assert)(const char* file, int line);
static void(*do_log_out)(const char* log);
void register_debug_function(void(*my_assert)(const char* file, int line), void(*my_log_out)(const char* log))
{
	do_assert = my_assert;
	do_log_out = my_log_out;
}
void _assert(const char* file, int line)
{
	static char s_buf[192];
	if (do_assert) 
	{
		do_assert(file, line);
	}
	else
	{
		memset(s_buf, 0, sizeof(s_buf));
		sprintf_s(s_buf, sizeof(s_buf), "vvvvvvvvvvvvvvvvvvvvvvvvvvvv\n\nAssert@ file = %s, line = %d\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n", file, line);
		OutputDebugStringA(s_buf);
		printf("%s", s_buf);
		fflush(stdout);
		assert(false);
	}
}
void log_out(const char* log)
{
	if (do_log_out)
	{
		do_log_out(log);
	}
	else
	{
		printf("%s", log);
		fflush(stdout);
		OutputDebugStringA(log);
	}
}
typedef struct _timer_manage
{
    struct  _timer_info
    {
        int state; /* on or off */
        int interval;
        int elapse; /* 0~interval */
        void (* timer_proc) (void* ptmr, void* parg);
    }timer_info[MAX_TIMER_CNT];
    void (* old_sigfunc)(int);
    void (* new_sigfunc)(int);
}_timer_manage_t;
static struct _timer_manage timer_manage;
DWORD WINAPI timer_routine(LPVOID lpParam)
{
    int i;
    while(true)
    {
    	for(i = 0; i < MAX_TIMER_CNT; i++)
		{
			if(timer_manage.timer_info[i].state == 0)
			{
				continue;
			}
			timer_manage.timer_info[i].elapse++;
			if(timer_manage.timer_info[i].elapse == timer_manage.timer_info[i].interval)
			{
				timer_manage.timer_info[i].elapse = 0;
				timer_manage.timer_info[i].timer_proc(0, 0);
			}
		}
		Sleep(TIMER_UNIT);
    }
    return 0;
}
static int init_mul_timer()
{
	static bool s_is_init = false;
	if(s_is_init == true)
	{
		return 0;
	}
    memset(&timer_manage, 0, sizeof(struct _timer_manage));
    DWORD pid;
	CreateThread(0, 0, timer_routine, 0, 0, &pid);
    s_is_init = true;
    return 1;
}
static int set_a_timer(int interval, void (* timer_proc) (void* ptmr, void* parg))
{
	init_mul_timer();
	int i;
    if(timer_proc == 0 || interval <= 0)
    {
        return (-1);
    }
    for(i = 0; i < MAX_TIMER_CNT; i++)
    {
        if(timer_manage.timer_info[i].state == 1)
        {
            continue;
        }
        memset(&timer_manage.timer_info[i], 0, sizeof(timer_manage.timer_info[i]));
        timer_manage.timer_info[i].timer_proc = timer_proc;
        timer_manage.timer_info[i].interval = interval;
        timer_manage.timer_info[i].elapse = 0;
        timer_manage.timer_info[i].state = 1;
        break;
    }
    if(i >= MAX_TIMER_CNT)
    {
		ASSERT(false);
        return (-1);
    }
    return (i);
}
typedef void (*EXPIRE_ROUTINE)(void* arg);
EXPIRE_ROUTINE s_expire_function;
static c_fifo s_real_timer_fifo;
static DWORD WINAPI fire_real_timer(LPVOID lpParam)
{
	char dummy;
	while(1)
	{
		if(s_real_timer_fifo.read(&dummy, 1) > 0)
		{
			if(s_expire_function)s_expire_function(0);
		}
		else
		{
			ASSERT(false);
		}
	}
	return 0;
}
/*Win32 desktop only
static void CALLBACK trigger_real_timer(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)
{
	char dummy = 0x33;
	s_real_timer_fifo.write(&dummy, 1);
}
*/
static DWORD WINAPI trigger_real_timer(LPVOID lpParam)
{
	char dummy = 0x33;
	while (1)
	{
		s_real_timer_fifo.write(&dummy, 1);
		Sleep(REAL_TIME_TASK_CYCLE_MS);
	}
	return 0;
}
void start_real_timer(void (*func)(void* arg))
{
	if(0 == func)
	{
		return;
	}
	s_expire_function = func;
	//timeSetEvent(REAL_TIME_TASK_CYCLE_MS, 0, trigger_real_timer, 0, TIME_PERIODIC);//Win32 desktop only
	static DWORD s_pid;
	if(s_pid == 0)
	{
		CreateThread(0, 0, trigger_real_timer, 0, 0, &s_pid);
		CreateThread(0, 0, fire_real_timer, 0, 0, &s_pid);
	}
}
unsigned int get_cur_thread_id()
{
	return GetCurrentThreadId();
}
void register_timer(int milli_second,void func(void* ptmr, void* parg))
{
	set_a_timer(milli_second/TIMER_UNIT,func);
}
long get_time_in_second()
{
	return time(0);
}
T_TIME get_time()
{
	T_TIME ret = {0};
	
	SYSTEMTIME time;
	GetLocalTime(&time);
	ret.year = time.wYear;
	ret.month = time.wMonth;
	ret.day = time.wDay;
	ret.hour = time.wHour;
	ret.minute = time.wMinute;
	ret.second = time.wSecond;
	return ret;
}
T_TIME second_to_day(long second)
{
	T_TIME ret;
	ret.year = 1999;
	ret.month = 10;
	ret.date = 1;
	ret.second = second % 60;
	second /= 60;
	ret.minute = second % 60;
	second /= 60;
	ret.hour = (second + 8) % 24;//China time zone.
	return ret;
}
void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg)
{
	DWORD pid = 0;
	CreateThread(0, 0, LPTHREAD_START_ROUTINE(start_routine), arg, 0, &pid);
	*thread_id = pid;
}
void thread_sleep(unsigned int milli_seconds)
{
	Sleep(milli_seconds);
}
#pragma pack(push,1)
typedef struct {
	unsigned short	bfType;
	unsigned int   	bfSize;
	unsigned short  bfReserved1;
	unsigned short  bfReserved2;
	unsigned int   	bfOffBits;
}FileHead;
typedef struct {
	unsigned int  	biSize;
	int 			biWidth;
	int       		biHeight;
	unsigned short	biPlanes;
	unsigned short  biBitCount;
	unsigned int    biCompress;
	unsigned int    biSizeImage;
	int       		biXPelsPerMeter;
	int       		biYPelsPerMeter;
	unsigned int 	biClrUsed;
	unsigned int    biClrImportant;
	unsigned int 	biRedMask;
	unsigned int 	biGreenMask;
	unsigned int 	biBlueMask;
}Infohead;
#pragma pack(pop)
int build_bmp(const char *filename, unsigned int width, unsigned int height, unsigned char *data)
{
	FileHead bmp_head;
	Infohead bmp_info;
	int size = width * height * 2;
	//initialize bmp head.
	bmp_head.bfType = 0x4d42;
	bmp_head.bfSize = size + sizeof(FileHead) + sizeof(Infohead);
	bmp_head.bfReserved1 = bmp_head.bfReserved2 = 0;
	bmp_head.bfOffBits = bmp_head.bfSize - size;
	//initialize bmp info.
	bmp_info.biSize = 40;
	bmp_info.biWidth = width;
	bmp_info.biHeight = height;
	bmp_info.biPlanes = 1;
	bmp_info.biBitCount = 16;
	bmp_info.biCompress = 3;
	bmp_info.biSizeImage = size;
	bmp_info.biXPelsPerMeter = 0;
	bmp_info.biYPelsPerMeter = 0;
	bmp_info.biClrUsed = 0;
	bmp_info.biClrImportant = 0;
	//RGB565
	bmp_info.biRedMask = 0xF800;
	bmp_info.biGreenMask = 0x07E0;
	bmp_info.biBlueMask = 0x001F;
	//copy the data
	FILE *fp;
	if (!(fp = fopen(filename, "wb")))
	{
		return -1;
	}
	fwrite(&bmp_head, 1, sizeof(FileHead), fp);
	fwrite(&bmp_info, 1, sizeof(Infohead), fp);
	//fwrite(data, 1, size, fp);//top <-> bottom
	for (int i = (height - 1); i >= 0; --i)
	{
		fwrite(&data[i * width * 2], 1, width * 2, fp);
	}
	fclose(fp);
	return 0;
}
c_fifo::c_fifo()
{
	m_head = m_tail = 0;
	m_read_sem = CreateSemaphore(0,	// default security attributes
		0,		// initial count
		1,		// maximum count
		0);	// unnamed semaphore
	m_write_mutex = CreateMutex(0, false, 0);
}
int c_fifo::read(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	while (i < len)
	{
		if (m_tail == m_head)
		{//empty
			WaitForSingleObject(m_read_sem, INFINITE);
			continue;
		}
		*pbuf++ = m_buf[m_head];
		m_head = (m_head + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	if (i != len)
	{
		ASSERT(false);
	}
	return i;
}
int c_fifo::write(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	int tail = m_tail;
	WaitForSingleObject(m_write_mutex, INFINITE);
	while (i < len)
	{
		if ((m_tail + 1) % FIFO_BUFFER_LEN == m_head)
		{//full, clear data has been written;
			m_tail = tail;
			log_out("Warning: fifo full\n");
			ReleaseMutex(m_write_mutex);
			return 0;
		}
		m_buf[m_tail] = *pbuf++;
		m_tail = (m_tail + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	ReleaseMutex(m_write_mutex);
	if (i != len)
	{
		ASSERT(false);
	}
	else
	{
		ReleaseSemaphore(m_read_sem, 1, 0);
	}
	return i;
}
#endif
#if (defined __linux__) || (defined __APPLE__)
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
typedef void(*ANDROID_PLAY_WAV)(const char* fileName);
ANDROID_PLAY_WAV gAndroidPlayWav;
typedef struct
{
	AUDIO_TYPE type;
}AUDIO_REQUEST;
static c_fifo s_request_fifo;
static void* render_thread(void* param)
{
	while (true)
	{
		AUDIO_REQUEST request;
		s_request_fifo.read(&request, sizeof(request));
		
		if (AUDIO_MAX <= request.type)
		{
			continue;
		}
		if(gAndroidPlayWav)
		{
			gAndroidPlayWav("heart_beat.wav");
		}
	}
}
void c_audio::init()
{
	static bool s_flag = false;
	if (s_flag)
	{
		return;
	}
	unsigned long pid;
	create_thread(&pid, 0, render_thread, 0);
	s_flag = true;
}
int c_audio::play(AUDIO_TYPE type)
{
	if (AUDIO_MAX <= type)
	{
		return -1;
	}
	init();
	AUDIO_REQUEST request;
	request.type = type;
	s_request_fifo.write(&request, sizeof(request));
	return 0;
}
#endif
#if (defined _WIN32) || (defined WIN32) || (defined _WIN64) || (defined WIN64)
#include <windows.h>
#include <Audioclient.h>
#include <mmdeviceapi.h>
#ifndef AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM
	#define AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM 0x80000000
#endif
#define AUDIO_CHANNELS_MONO     1
#define AUDIO_SAMPLE_RATE       44000
#define AUDIO_BITS              16
#define AUDIO_BLOCK_ALIGN       (AUDIO_CHANNELS_MONO * (AUDIO_BITS >> 3))
#define AUDIO_BYTE_RATE         (AUDIO_SAMPLE_RATE * AUDIO_BLOCK_ALIGN)
#define AUDIO_OUTPUT_BUF_LEN	(10000000 * 5)	//5 seconds long.
#define CHECK_ERROR(ret) if(ret != 0){ASSERT(false);}
typedef struct
{
	AUDIO_TYPE type;
}AUDIO_REQUEST;
typedef struct
{
	BYTE* p_data;
	int size;
}WAV_RESOURCE;
static WAV_RESOURCE s_wav_resource[AUDIO_MAX];
static c_fifo s_request_fifo;
static IAudioClient* s_audio_client;
static IAudioRenderClient* s_audio_render_client;
static HANDLE s_audio_event;
//Should be call by UWP, and UWP create audio client.
void set_audio_client(IAudioClient* audio_client)
{
	s_audio_client = audio_client;
}
static WAVEFORMATEX s_wav_format = {
	WAVE_FORMAT_PCM,
	AUDIO_CHANNELS_MONO,
	AUDIO_SAMPLE_RATE,
	AUDIO_BYTE_RATE,
	AUDIO_BLOCK_ALIGN,
	AUDIO_BITS,
	0
};
static int register_wav_resouce(AUDIO_TYPE type, const wchar_t* wav_path)
{
	if (s_wav_resource[type].p_data)
	{
		return 0;
	}
  
	void* hFile = CreateFile(wav_path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		log_out("Open wave file failed\n");
		return -1;
	}
	LARGE_INTEGER ret;
	GetFileSizeEx(hFile, &ret);
	int size = ret.LowPart;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0x2C, 0, FILE_BEGIN))
	{
		ASSERT(false);
		return -2;
	}
	size -= 0x2C;
	BYTE* p_data = (BYTE*)malloc(size);
	DWORD read_num;
	ReadFile(hFile, p_data, size, &read_num, 0);
	s_wav_resource[type].p_data = p_data;
	s_wav_resource[type].size = size;
	return 0;
}
static int load_wav_chunk(BYTE* p_des, int des_size, BYTE* p_src, int src_size)
{
	if (des_size <= 0 || src_size <= 0)
	{
		return -1;
	}
	int write_size = (src_size > des_size) ? des_size : src_size;
	memcpy(p_des, p_src, write_size);
	memset(p_des + write_size, 0, (des_size - write_size));
	return write_size;
}
static int play_wav(BYTE* p_data, int size)
{
	if (0 == p_data || 0 >= size)
	{
		return -1;
	}
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	BYTE* p_buffer = 0;
	int ret = s_audio_client->GetBufferSize(&bufferFrameCount);
	CHECK_ERROR(ret);
	
	int offset = 0;
	while (WaitForSingleObject(s_audio_event, INFINITE) == WAIT_OBJECT_0)
	{
		ret = s_audio_client->GetCurrentPadding(&numFramesPadding);
		CHECK_ERROR(ret);
		numFramesAvailable = bufferFrameCount - numFramesPadding;
		if (numFramesAvailable < 1600)
		{
			Sleep(10);
			continue;
		}
		ret = s_audio_render_client->GetBuffer(numFramesAvailable, &p_buffer);
		CHECK_ERROR(ret);
		ret = load_wav_chunk(p_buffer, numFramesAvailable * s_wav_format.nBlockAlign, p_data + offset, (size - offset));
		if (ret > 0)
		{
			s_audio_render_client->ReleaseBuffer((ret / s_wav_format.nBlockAlign), 0);
			offset += ret;
		}
		else
		{
			s_audio_render_client->ReleaseBuffer(0, AUDCLNT_BUFFERFLAGS_SILENT);
			break;
		}
	}	
	return 0;
}
static void* render_thread(void* param)
{
	s_audio_client->Start();
	while (true)
	{
		AUDIO_REQUEST request;
		s_request_fifo.read(&request, sizeof(request));
		
		if (AUDIO_MAX <= request.type)
		{
			ASSERT(false);
			continue;
		}
		play_wav(s_wav_resource[request.type].p_data, s_wav_resource[request.type].size);
	}
	s_audio_client->Stop();
}
static int init_audio_client()
{
	if (s_audio_client)
	{
		return 0;
	}
	//For desktop only, could not pass Windows Store certification.
	/*
	int ret = CoInitializeEx(0, COINIT_MULTITHREADED);
	CHECK_ERROR(ret);
	IMMDeviceEnumerator *pEnumerator = nullptr;
	ret = CoCreateInstance(__uuidof(MMDeviceEnumerator), 0,
	CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
	(void**)&pEnumerator);
	CHECK_ERROR(ret);
	IMMDevice* audio_output_device;
	pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audio_output_device);
	if (0 == audio_output_device)
	{
	ASSERT(false);
	}
	ret = audio_output_device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, 0, (void**)&s_audio_client);
	CHECK_ERROR(ret);
	return 0;
	*/
	return -1;
}
void c_audio::init()
{
	static bool s_flag = false;
	if (s_flag)
	{
		return;
	}
	register_wav_resouce(AUDIO_HEART_BEAT, L"heart_beat.wav");
	
	if (0 > init_audio_client())
	{
		return;
	}
	int ret = s_audio_client->Initialize(AUDCLNT_SHAREMODE_SHARED,
									AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
									AUDIO_OUTPUT_BUF_LEN * 2, 0, &s_wav_format,	0);
	CHECK_ERROR(ret);
	//s_audio_event = CreateEventEx(0, 0, 0, EVENT_ALL_ACCESS);
	s_audio_event = CreateEvent(0, 0, 0, 0);
	ret = s_audio_client->SetEventHandle(s_audio_event);
	CHECK_ERROR(ret);
	ret = s_audio_client->GetService(__uuidof(IAudioRenderClient), (void**)&s_audio_render_client);
	CHECK_ERROR(ret);
	unsigned long pid;
	create_thread(&pid, 0, render_thread, 0);
	s_flag = true;
}
int c_audio::play(AUDIO_TYPE type)
{
	if (AUDIO_MAX <= type)
	{
		return -1;
	}
	init();
	if (!s_audio_client || !s_audio_render_client)
	{
		return -2;
	}
	AUDIO_REQUEST request;
	request.type = type;
	s_request_fifo.write(&request, sizeof(request));
	return 0;
}
#endif
void c_button::pre_create_wnd()
{
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
	m_font_type = c_theme::get_font(FONT_DEFAULT);
	m_font_color = c_theme::get_color(COLOR_WND_FONT);
}
void c_button::on_focus()
{
	m_status = STATUS_FOCUSED;
	on_paint();
}
void c_button::on_kill_focus()
{
	m_status = STATUS_NORMAL;
	on_paint();
}
bool c_button::on_touch(int x, int y, TOUCH_ACTION action)
{
	if (action == TOUCH_DOWN)
	{
		m_parent->set_child_focus(this);
		m_status = STATUS_PUSHED;
		on_paint();
	}
	else
	{
		m_status = STATUS_FOCUSED;
		on_paint();
		notify_parent(GL_BN_CLICKED, 0);
	}
	return true;
}
bool c_button::on_key(KEY_TYPE key)
{
	if (key == KEY_ENTER)
	{
		notify_parent(GL_BN_CLICKED, 0);
		return false;// Do not handle KEY_ENTER by other wnd.
	}
	return true;// Handle KEY_FOWARD/KEY_BACKWARD by parent wnd.
}
void c_button::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	switch(m_status)
	{
	case STATUS_NORMAL:
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
		if (m_str)
		{
			c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
		}
		break;
	case STATUS_FOCUSED:
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
		if (m_str)
		{
			c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
		}
		break;
	case STATUS_PUSHED:
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_PUSHED), m_z_order);
		m_surface->draw_rect(rect, c_theme::get_color(COLOR_WND_BORDER), 2, m_z_order);
		if (m_str)
		{
			c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_PUSHED), ALIGN_HCENTER | ALIGN_VCENTER);
		}
		break;
	default:
		ASSERT(false);
		break;
	}
}
DIALOG_ARRAY c_dialog::ms_the_dialogs[SURFACE_CNT_MAX];
void c_dialog::pre_create_wnd()
{
	m_attr = WND_ATTRIBUTION(0);// no focus/visible
	m_z_order = Z_ORDER_LEVEL_1;
	m_bg_color = GL_RGB(33, 42, 53);
}
void c_dialog::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	m_surface->fill_rect(rect, m_bg_color, m_z_order);
	if (m_str)
	{
		c_word::draw_string(m_surface, m_z_order, m_str, rect.m_left+35, rect.m_top, c_theme::get_font(FONT_DEFAULT), GL_RGB(255, 255, 255), GL_ARGB(0, 0, 0, 0), ALIGN_LEFT);
	}
}
c_dialog* c_dialog::get_the_dialog(c_surface* surface)
{
	for(int i = 0; i < SURFACE_CNT_MAX; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			return ms_the_dialogs[i].dialog;
		}
	}
	return 0;
}
int c_dialog::open_dialog(c_dialog* p_dlg, bool modal_mode)
{
	if (0 == p_dlg)
	{
		ASSERT(false);
		return 0;
	}
	c_dialog* cur_dlg = get_the_dialog(p_dlg->get_surface());
	if (cur_dlg == p_dlg)
	{
		return 1;
	}
	if(cur_dlg)
	{
		cur_dlg->set_attr(WND_ATTRIBUTION(0));
	}
	c_rect rc;
	p_dlg->get_screen_rect(rc);
	p_dlg->get_surface()->set_frame_layer_visible_rect(rc, Z_ORDER_LEVEL_1);
	p_dlg->set_attr(modal_mode ? (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_MODAL) : (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS));
	p_dlg->show_window();
	p_dlg->set_me_the_dialog();
	return 1;
}
int c_dialog::close_dialog(c_surface* surface)
{
	c_dialog* dlg = get_the_dialog(surface);
	if (0 == dlg)
	{
		return 0;
	}
	c_rect rc;
	
	dlg->set_attr(WND_ATTRIBUTION(0));
	surface->set_frame_layer_visible_rect(rc, dlg->m_z_order);
	//clear the dialog
	for(int i = 0; i < SURFACE_CNT_MAX; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			ms_the_dialogs[i].dialog = 0;
			return 1;
		}
	}
	ASSERT(false);
	return -1;
}
int c_dialog::set_me_the_dialog()
{
	c_surface* surface = get_surface();
	for(int i = 0; i < SURFACE_CNT_MAX; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			ms_the_dialogs[i].dialog = this;
			return 0;
		}
	}
	for(int i = 0; i < SURFACE_CNT_MAX; i++)
	{
		if(ms_the_dialogs[i].surface == 0)
		{
			ms_the_dialogs[i].dialog = this;
			if(this)
			{
				ms_the_dialogs[i].surface = surface;
			}
			return 1;
		}
	}
	ASSERT(false);
	return -2;
}
#include <string.h>
#define IDD_KEY_BOARD		0x1
GL_BEGIN_MESSAGE_MAP(c_edit)
ON_KEYBORAD_UPDATE(IDD_KEY_BOARD, c_edit::on_key_board_click)
GL_END_MESSAGE_MAP()
static c_keyboard  s_keyboard;
void c_edit::pre_create_wnd()
{
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
	m_kb_style = STYLE_ALL_BOARD;
	m_font_type = c_theme::get_font(FONT_DEFAULT);
	m_font_color = c_theme::get_color(COLOR_WND_FONT);
	memset(m_str_input, 0, sizeof(m_str_input));
	memset(m_str, 0, sizeof(m_str));
	set_text(c_wnd::m_str);
}
void c_edit::set_text(const char* str)
{
	if (str != 0 && strlen(str) < sizeof(m_str))
	{
		strcpy(m_str, str);
	}
}
bool c_edit::on_touch(int x, int y, TOUCH_ACTION action)
{
	(action == TOUCH_DOWN) ? on_touch_down(x, y) : on_touch_up(x, y);
	return true;
}
void c_edit::on_touch_down(int x, int y)
{
	c_rect kb_rect_relate_2_edit_parent;
	s_keyboard.get_wnd_rect(kb_rect_relate_2_edit_parent);
	kb_rect_relate_2_edit_parent.m_left += m_wnd_rect.m_left;
	kb_rect_relate_2_edit_parent.m_right += m_wnd_rect.m_left;
	kb_rect_relate_2_edit_parent.m_top += m_wnd_rect.m_top;
	kb_rect_relate_2_edit_parent.m_bottom += m_wnd_rect.m_top;
	if (m_wnd_rect.PtInRect(x, y))
	{//click edit box
		if (STATUS_NORMAL == m_status)
		{
			m_parent->set_child_focus(this);
		}
	}
	else if (kb_rect_relate_2_edit_parent.PtInRect(x,y))
	{//click key board
        c_wnd::on_touch(x, y, TOUCH_DOWN);
	}
	else
	{
		if (STATUS_PUSHED == m_status)
		{
			m_status = STATUS_FOCUSED;
			on_paint();
		}        
	}
}
void c_edit::on_touch_up(int x, int y)
{
	if (STATUS_FOCUSED == m_status)
	{
		m_status = STATUS_PUSHED;
		on_paint();
	}
	else if (STATUS_PUSHED == m_status)
	{
		if (m_wnd_rect.PtInRect(x,y))
		{//click edit box
			m_status = STATUS_FOCUSED;
			on_paint();
		}
		else
		{
			c_wnd::on_touch(x, y, TOUCH_UP);
		}
	}
}
void c_edit::on_focus()
{
	m_status = STATUS_FOCUSED;
	on_paint();
}
void c_edit::on_kill_focus()
{
	m_status = STATUS_NORMAL;
	on_paint();
}
void c_edit::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	c_rect empty_rect;
	empty_rect.Empty();
	switch(m_status)
	{
	case STATUS_NORMAL:
		if (m_z_order > m_parent->get_z_order())
		{
			s_keyboard.disconnect();
			m_surface->set_frame_layer_visible_rect(empty_rect, s_keyboard.get_z_order());
			m_z_order = m_parent->get_z_order();
			m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
		}
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
		c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
		break;
	case STATUS_FOCUSED:
		if (m_z_order > m_parent->get_z_order())
		{
			s_keyboard.disconnect();
			m_surface->set_frame_layer_visible_rect(empty_rect, s_keyboard.get_z_order());
			m_z_order = m_parent->get_z_order();
			m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
		}
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
		c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
		break;
	case STATUS_PUSHED:
		if (m_z_order == m_parent->get_z_order())
		{
			m_z_order++;
			m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_MODAL);
			show_keyboard();
		}
		m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_PUSHED), m_parent->get_z_order());
		m_surface->draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_BORDER), m_parent->get_z_order(), 2);
		strlen(m_str_input) ? c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str_input, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_PUSHED), ALIGN_HCENTER | ALIGN_VCENTER) :
			c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_PUSHED), ALIGN_HCENTER | ALIGN_VCENTER);
		break;
	default:
		ASSERT(false);
	}
}
void c_edit::show_keyboard()
{
	s_keyboard.connect(this, IDD_KEY_BOARD, m_kb_style);
	c_rect kb_rect;
	s_keyboard.get_screen_rect(kb_rect);
	m_surface->set_frame_layer_visible_rect(kb_rect, s_keyboard.get_z_order());
	s_keyboard.show_window();
}
void c_edit::on_key_board_click(unsigned int ctrl_id, long param)
{
	switch (param)
	{
	case CLICK_CHAR:
		strcpy(m_str_input, s_keyboard.get_str());
		on_paint();
		break;
	case CLICK_ENTER:
		if (strlen(m_str_input))
		{
			memcpy(m_str, m_str_input, sizeof(m_str_input));
		}
		m_status = STATUS_FOCUSED;
		on_paint();
		break;
	case CLICK_ESC:
		memset(m_str_input, 0, sizeof(m_str_input));
		m_status = STATUS_FOCUSED;
		on_paint();
		break;
	default:
		ASSERT(false);
		break;
	}
}
#include <stdlib.h>
//#define SWIPE_STEP			300//for arm
#define SWIPE_STEP			10//for PC & ANDROID
#define MOVE_THRESHOLD		10
c_gesture::c_gesture(c_slide_group* group)
{
	m_slide_group = group;
	m_state = TOUCH_IDLE;
	m_down_x = m_down_y = m_move_x = m_move_y = 0;
}
bool c_gesture::handle_swipe(int x, int y, TOUCH_ACTION action)
{
	if(action == TOUCH_DOWN)//MOUSE_LBUTTONDOWN
	{
		if(m_state == TOUCH_IDLE)
		{
			m_state = TOUCH_MOVE;
			m_move_x = m_down_x = x;
			return true;
		}
		else//TOUCH_MOVE
		{
			return on_move(x);
		}
	}
	else if(action == TOUCH_UP)//MOUSE_LBUTTONUP
	{
		if(m_state == TOUCH_MOVE)
		{
			m_state = TOUCH_IDLE;
			return on_swipe(x);
		}
		else
		{
			return false;
			//ASSERT(false);
		}
	}
	return true;
}
bool c_gesture::on_move(int x)
{
	if (m_slide_group == 0)
	{
		return true;
	}
	if (abs(x - m_move_x) < MOVE_THRESHOLD)
	{
		return false;
	}
	m_slide_group->disabel_all_slide();
	m_move_x = x;
	if ((m_move_x - m_down_x) > 0)
	{
		move_right();
	}
	else
	{
		move_left();
	}
	return false;
}
bool c_gesture::on_swipe(int x)
{
	if (m_slide_group == 0)
	{
		return true;
	}
	if ((m_down_x == m_move_x) && (abs(x - m_down_x) < MOVE_THRESHOLD))
	{
		return true;
	}
	m_slide_group->disabel_all_slide();
	int page = -1;
	m_move_x = x;
	if ((m_move_x - m_down_x) > 0)
	{
		page = swipe_right();
	}
	else
	{
		page = swipe_left();
	}
	if (page >= 0)
	{
		m_slide_group->set_active_slide(page);
	}
	else
	{
		m_slide_group->set_active_slide(m_slide_group->get_active_slide_index(), false);
	}
	return false;
}
int c_gesture::swipe_left()
{
	if (m_slide_group == 0)
	{
		return -1;
	}
	int index = m_slide_group->get_active_slide_index();
	if((index + 1) >= MAX_PAGES ||
		m_slide_group->get_slide(index + 1) == 0 ||
		m_slide_group->get_slide(index) == 0)
	{
		return -2;
	}
	c_surface* s1 = m_slide_group->get_slide(index + 1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	if (s1->get_display() != s2->get_display())
	{
		return -3;
	}
	int step = m_down_x - m_move_x;
	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	while(step < rc.Width())
	{
		s1->get_display()->swipe_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, step);
		step += SWIPE_STEP;
	}
	if (step != rc.Width())
	{
		s1->get_display()->swipe_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, rc.Width());
	}
	return (index + 1);
}
int c_gesture::swipe_right()
{
	if (m_slide_group == 0)
	{
		return -1;
	}
	int index = m_slide_group->get_active_slide_index();
	if(index <= 0 ||
			m_slide_group->get_slide(index - 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
	{
		return -2;
	}
	c_surface* s1 = m_slide_group->get_slide(index -1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	if (s1->get_display() != s2->get_display())
	{
		return -3;
	}
	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	int step = rc.Width() - (m_move_x - m_down_x);
	while(step > 0)
	{
		s1->get_display()->swipe_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, step);
		step -= SWIPE_STEP;
	}
	if (step != 0)
	{
		s1->get_display()->swipe_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, 0);
	}
	return (index - 1);
}
void c_gesture::move_left()
{
	int index = m_slide_group->get_active_slide_index();
	if((index + 1) >= MAX_PAGES ||
			m_slide_group->get_slide(index + 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
	{
		return;
	}
	c_surface* s1 = m_slide_group->get_slide(index +1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	if(s1->get_display() == s2->get_display())
	{
		s1->get_display()->swipe_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, (m_down_x - m_move_x));
	}
}
void c_gesture::move_right()
{
	int index = m_slide_group->get_active_slide_index();
	if(index <= 0 ||
			m_slide_group->get_slide(index - 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
	{
		return;
	}
	c_surface* s1 = m_slide_group->get_slide(index -1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	if(s1->get_display() == s2->get_display())
	{
		s1->get_display()->swipe_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, (rc.Width() - (m_move_x - m_down_x)));
	}
}
#include <string.h>
//Changing key width/height will change the width/height of keyboard
#define KEY_WIDTH          65
#define KEY_HEIGHT         38
#define KEYBOARD_WIDTH		((KEY_WIDTH + 2) * 10)
#define KEYBOARD_HEIGHT		((KEY_HEIGHT + 2) * 4)
#define NUM_BOARD_WIDTH		((KEY_WIDTH + 2) * 4)
#define NUM_BOARD_HEIGHT	((KEY_HEIGHT + 2) * 4)
#define CAPS_WIDTH			(KEY_WIDTH * 3 / 2)
#define DEL_WIDTH			(KEY_WIDTH * 3 / 2 + 1)
#define ESC_WIDTH			(KEY_WIDTH * 2 + 2)
#define SWITCH_WIDTH		(KEY_WIDTH * 3 / 2 )
#define SPACE_WIDTH			(KEY_WIDTH * 3 + 2 * 2)
#define DOT_WIDTH			(KEY_WIDTH * 3 / 2 + 3)
#define ENTER_WIDTH			(KEY_WIDTH * 2 + 2)
#define POS_X(c)			((KEY_WIDTH * c) + (c + 1) * 2)
#define POS_Y(r)			((KEY_HEIGHT * r) + (r + 1) * 2)
static c_keyboard_button s_button_0,s_button_1, s_button_2, s_button_3, s_button_4, s_button_5, s_button_6, s_button_7, s_button_8, s_button_9;
static c_keyboard_button s_button_A, s_button_B, s_button_C, s_button_D, s_button_E, s_button_F, s_button_G, s_button_H, s_button_I, s_button_J;
static c_keyboard_button s_button_K, s_button_L, s_button_M, s_button_N, s_button_O, s_button_P, s_button_Q, s_button_R, s_button_S, s_button_T;
static c_keyboard_button s_button_U, s_button_V, s_button_W, s_button_X, s_button_Y, s_button_Z;
static c_keyboard_button s_button_dot, s_button_caps, s_button_space, s_button_enter, s_button_del, s_button_esc, s_button_num_switch;
WND_TREE g_key_board_children[] =
{
	//Row 1
	{&s_button_Q, 'Q', 0, POS_X(0), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_W, 'W', 0, POS_X(1), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_E, 'E', 0, POS_X(2), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_R, 'R', 0, POS_X(3), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_T, 'T', 0, POS_X(4), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_Y, 'Y', 0, POS_X(5), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_U, 'U', 0, POS_X(6), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_I, 'I', 0, POS_X(7), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_O, 'O', 0, POS_X(8), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_P, 'P', 0, POS_X(9), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	//Row 2 
	{&s_button_A, 'A', 0, ((KEY_WIDTH / 2) + POS_X(0)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_S, 'S', 0, ((KEY_WIDTH / 2) + POS_X(1)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_D, 'D', 0, ((KEY_WIDTH / 2) + POS_X(2)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_F, 'F', 0, ((KEY_WIDTH / 2) + POS_X(3)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_G, 'G', 0, ((KEY_WIDTH / 2) + POS_X(4)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_H, 'H', 0, ((KEY_WIDTH / 2) + POS_X(5)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_J, 'J', 0, ((KEY_WIDTH / 2) + POS_X(6)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_K, 'K', 0, ((KEY_WIDTH / 2) + POS_X(7)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_L, 'L', 0, ((KEY_WIDTH / 2) + POS_X(8)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	//Row 3
	{&s_button_caps, 0x14,	0, POS_X(0),						POS_Y(2), CAPS_WIDTH,	KEY_HEIGHT},
	{&s_button_Z,	'Z',	0, ((KEY_WIDTH / 2) + POS_X(1)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_X,	'X',	0, ((KEY_WIDTH / 2) + POS_X(2)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_C,	'C',	0, ((KEY_WIDTH / 2) + POS_X(3)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_V,	'V',	0, ((KEY_WIDTH / 2) + POS_X(4)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_B,	'B',	0, ((KEY_WIDTH / 2) + POS_X(5)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_N,	'N',	0, ((KEY_WIDTH / 2) + POS_X(6)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_M,	'M',	0, ((KEY_WIDTH / 2) + POS_X(7)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_del,	0x7F,	0, ((KEY_WIDTH / 2) + POS_X(8)),	POS_Y(2), DEL_WIDTH,	KEY_HEIGHT},
	//Row 4
	{&s_button_esc,			0x1B,	0, POS_X(0),						POS_Y(3), ESC_WIDTH,	KEY_HEIGHT},
	{&s_button_num_switch,	0x90,	0, POS_X(2),						POS_Y(3), SWITCH_WIDTH,	KEY_HEIGHT},
	{&s_button_space,		' ',	0, ((KEY_WIDTH / 2) + POS_X(3)),	POS_Y(3), SPACE_WIDTH,	KEY_HEIGHT},
	{&s_button_dot,			'.',	0, ((KEY_WIDTH / 2) + POS_X(6)),	POS_Y(3), DOT_WIDTH,	KEY_HEIGHT},
	{&s_button_enter,		'\n',	0, POS_X(8),						POS_Y(3), ENTER_WIDTH,	KEY_HEIGHT},
	{0,0,0,0,0,0,0}
};
WND_TREE g_number_board_children[] =
{
	{&s_button_1,	'1',	0, POS_X(0), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_2,	'2',	0, POS_X(1), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_3,	'3',	0, POS_X(2), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_del, 0x7F,	0, POS_X(3), POS_Y(0), KEY_WIDTH, KEY_HEIGHT * 2 + 2},
	{&s_button_4,	'4',	0, POS_X(0), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_5,	'5',	0, POS_X(1), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_6,	'6',	0, POS_X(2), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_7,	'7',	0, POS_X(0), POS_Y(2), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_8,	'8',	0, POS_X(1), POS_Y(2), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_9,	'9',	0, POS_X(2), POS_Y(2), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_enter,'\n',	0, POS_X(3), POS_Y(2), KEY_WIDTH, KEY_HEIGHT * 2 + 2},
	{&s_button_esc,	0x1B,	0, POS_X(0), POS_Y(3), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_0,	'0',	0, POS_X(1), POS_Y(3), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_dot,	'.',	0, POS_X(2), POS_Y(3), KEY_WIDTH, KEY_HEIGHT},
	{0,0,0,0,0,0,0}
};
GL_BEGIN_MESSAGE_MAP(c_keyboard)
ON_GL_BN_CLICKED('A', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('B', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('C', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('D', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('E', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('F', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('G', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('H', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('I', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('J', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('K', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('L', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('M', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('N', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('O', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('P', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('Q', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('R', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('S', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('T', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('U', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('V', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('W', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('X', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('Y', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('Z', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('0', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('1', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('2', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('3', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('4', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('5', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('6', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('7', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('8', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('9', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED(' ', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('.', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED(0x7F, c_keyboard::on_del_clicked)
ON_GL_BN_CLICKED(0x14, c_keyboard::on_caps_clicked)
ON_GL_BN_CLICKED('\n', c_keyboard::on_enter_clicked)
ON_GL_BN_CLICKED(0x1B, c_keyboard::on_esc_clicked)
GL_END_MESSAGE_MAP()
int c_keyboard::connect(c_wnd *user, unsigned short resource_id, KEYBOARD_STYLE style)
{
	c_rect user_rect;
	user->get_wnd_rect(user_rect);
	if (style == STYLE_ALL_BOARD)
	{//Place keyboard at the bottom of user's parent window.
		c_rect user_parent_rect;
		user->get_parent()->get_wnd_rect(user_parent_rect);
		return c_wnd::connect(user, resource_id, 0, (0 - user_rect.m_left), (user_parent_rect.Height() - user_rect.m_top - KEYBOARD_HEIGHT), KEYBOARD_WIDTH, KEYBOARD_HEIGHT, g_key_board_children);
	}
	else if(style == STYLE_NUM_BOARD)
	{//Place keyboard below the user window.
		return c_wnd::connect(user, resource_id, 0, 0, user_rect.Height(), NUM_BOARD_WIDTH, NUM_BOARD_HEIGHT, g_number_board_children);
	}
	else
	{
		ASSERT(false);
	}
	return -1;
}
void c_keyboard::pre_create_wnd()
{	
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
	m_cap_status = STATUS_UPPERCASE;
	memset(m_str, 0, sizeof(m_str));
	m_str_len = 0;
}
void c_keyboard::on_caps_clicked(unsigned int ctrl_id)
{
	m_cap_status = (m_cap_status == STATUS_LOWERCASE) ? STATUS_UPPERCASE : STATUS_LOWERCASE;
	show_window();
}
void c_keyboard::on_enter_clicked(unsigned int ctrl_id)
{
	memset(m_str, 0, sizeof(m_str));
    notify_parent(KEYBORAD_CLICK, CLICK_ENTER);
}
void c_keyboard::on_esc_clicked(unsigned int ctrl_id)
{
	memset(m_str, 0, sizeof(m_str));
	notify_parent(KEYBORAD_CLICK, CLICK_ESC);
}
void c_keyboard::on_del_clicked(unsigned int ctrl_id)
{
	if (m_str_len <= 0)
	{
		return;
	}
	m_str[--m_str_len] = 0;
	notify_parent(KEYBORAD_CLICK, CLICK_CHAR);
}
void c_keyboard::on_char_clicked(unsigned int ctrl_id)
{//ctrl_id = char ascii code.
	if (m_str_len >= sizeof(m_str))
	{
		return;
	}
	if ((ctrl_id >= '0' && ctrl_id <= '9') || ctrl_id == ' ' || ctrl_id == '.')
	{
		goto InputChar;
	}
	if (ctrl_id >= 'A' && ctrl_id <= 'Z')
	{
		if (STATUS_LOWERCASE == m_cap_status)
		{
			ctrl_id += 0x20;
		}
		goto InputChar;
	}
	ASSERT(false);
InputChar:
	m_str[m_str_len++] = ctrl_id;
	notify_parent(KEYBORAD_CLICK, CLICK_CHAR);
}
void c_keyboard::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	m_surface->fill_rect(rect, GL_RGB(0, 0, 0), m_z_order);
}
void c_keyboard_button::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	switch(m_status)
	{
	case STATUS_NORMAL:
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
		break;
	case STATUS_FOCUSED:
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
		break;
	case STATUS_PUSHED:
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_PUSHED), m_z_order);
		m_surface->draw_rect(rect, c_theme::get_color(COLOR_WND_BORDER), 2, m_z_order);
		break;
	default:
		ASSERT(false);
		break;
	}
	
	if (m_resource_id == 0x14)
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Caps", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == 0x1B)
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Esc", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == ' ')
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Space", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == '\n')
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Enter", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == '.')
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, ".", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == 0x7F)
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Back", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == 0x90)
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "?123", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	
	char letter[] = { 0, 0 };
	if (m_resource_id >= 'A' && m_resource_id <= 'Z')
	{
		letter[0] = (((c_keyboard*)m_parent)->get_cap_status() == STATUS_UPPERCASE) ? m_resource_id : (m_resource_id + 0x20);
	}
	else if (m_resource_id >= '0' && m_resource_id <= '9')
	{
		letter[0] = m_resource_id;
	}
	c_word::draw_string_in_rect(m_surface, m_z_order, letter, rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
}
void c_label::pre_create_wnd()
{
	m_attr = ATTR_VISIBLE;
	m_font_color = c_theme::get_color(COLOR_WND_FONT);
	m_font_type = c_theme::get_font(FONT_DEFAULT);
}
void c_label::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	if (m_str)
	{
		m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, m_parent->get_bg_color(), m_z_order);
		c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, m_parent->get_bg_color(), ALIGN_LEFT | ALIGN_VCENTER);
	}
}
#include <string.h>
#define ITEM_HEIGHT				45
void c_list_box::pre_create_wnd()
{
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
	memset(m_item_array, 0, sizeof(m_item_array));
	m_item_total = 0;
	m_selected_item = 0;
	m_font_type = c_theme::get_font(FONT_DEFAULT);
	m_font_color = c_theme::get_color(COLOR_WND_FONT);
}
void c_list_box::on_focus()
{
	m_status = STATUS_FOCUSED;
	on_paint();
}
void c_list_box::on_kill_focus()
{
	m_status = STATUS_NORMAL;
	on_paint();
}
void c_list_box::on_paint()
{
	c_rect rect, empty_rect;
	get_screen_rect(rect);
	switch(m_status)
	{
	case STATUS_NORMAL:
		if (m_z_order > m_parent->get_z_order())
		{
			m_surface->set_frame_layer_visible_rect(empty_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
			m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
		}
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
		c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
		break;
	case STATUS_FOCUSED:
		if (m_z_order > m_parent->get_z_order())
		{
			m_surface->set_frame_layer_visible_rect(empty_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
			m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
		}
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
		c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
		break;
	case STATUS_PUSHED:
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_PUSHED), m_z_order);
		m_surface->draw_rect(rect, c_theme::get_color(COLOR_WND_BORDER), 2, m_z_order);
		c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], rect, m_font_type, GL_RGB(2, 124, 165), GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER | ALIGN_VCENTER);
		//draw list
		if (m_item_total > 0)
		{
			if (m_z_order == m_parent->get_z_order())
			{
				m_z_order++;
			}
			m_surface->set_frame_layer_visible_rect(m_list_screen_rect, m_z_order);
			m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_MODAL);
			show_list();
		}
		break;
	default:
		ASSERT(false);
	}
}
bool c_list_box::on_touch(int x, int y, TOUCH_ACTION action)
{
	(action == TOUCH_DOWN) ? on_touch_down(x, y) : on_touch_up(x, y);
	return true;
}
void c_list_box::on_touch_down(int x, int y)
{
	if (m_wnd_rect.PtInRect(x, y) )
	{//click base
		if (STATUS_NORMAL == m_status)
		{
			m_parent->set_child_focus(this);
		}
	}
	else if (m_list_wnd_rect.PtInRect(x, y))
	{//click extend list
		c_wnd::on_touch(x, y, TOUCH_DOWN);
	}
	else
	{
		if (STATUS_PUSHED == m_status)
		{
			m_status = STATUS_FOCUSED;
			on_paint();
			notify_parent(GL_LIST_CONFIRM, m_selected_item);
		}        
	}
}
void c_list_box::on_touch_up(int x, int y)
{
	if (STATUS_FOCUSED == m_status)
	{
		m_status = STATUS_PUSHED;
		on_paint();
	}
	else if (STATUS_PUSHED == m_status)
	{
		if (m_wnd_rect.PtInRect(x, y))
		{//click base
			m_status = STATUS_FOCUSED;
			on_paint();
		}
		else if (m_list_wnd_rect.PtInRect(x, y))
		{//click extend list
			m_status = STATUS_FOCUSED;
			select_item((y - m_list_wnd_rect.m_top) / ITEM_HEIGHT);
			on_paint();
			notify_parent(GL_LIST_CONFIRM, m_selected_item);
		}
		else
		{
			c_wnd::on_touch(x, y, TOUCH_UP);
		}
	}
}
void c_list_box::update_list_size()
{
	m_list_wnd_rect = m_wnd_rect;
	m_list_wnd_rect.m_top = m_wnd_rect.m_bottom + 1;
	m_list_wnd_rect.m_bottom = m_list_wnd_rect.m_top + m_item_total * ITEM_HEIGHT;
	get_screen_rect(m_list_screen_rect);
	m_list_screen_rect.m_top = m_list_screen_rect.m_bottom + 1;
	m_list_screen_rect.m_bottom = m_list_screen_rect.m_top + m_item_total * ITEM_HEIGHT;
}
void c_list_box::show_list()
{
	//draw all items
	c_rect tmp_rect;
	for (int i = 0; i < m_item_total; i++)
	{
		tmp_rect.m_left = m_list_screen_rect.m_left;
		tmp_rect.m_right = m_list_screen_rect.m_right;
		tmp_rect.m_top = m_list_screen_rect.m_top + i * ITEM_HEIGHT;
		tmp_rect.m_bottom = tmp_rect.m_top + ITEM_HEIGHT;
		if (m_selected_item == i)
		{
			m_surface->fill_rect(tmp_rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
			c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[i], tmp_rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
		}
		else
		{
			m_surface->fill_rect(tmp_rect, GL_RGB(17, 17, 17), m_z_order);
			c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[i], tmp_rect, m_font_type, m_font_color, GL_RGB(17, 17, 17), ALIGN_HCENTER | ALIGN_VCENTER);
		}
	}
}
int c_list_box::add_item(char* str)
{
	if (m_item_total >= MAX_ITEM_NUM)
	{
		ASSERT(false);
		return -1;
	}
	m_item_array[m_item_total++] = str;
	update_list_size();
	return 0;
}
void c_list_box::clear_item()
{
	m_selected_item = m_item_total = 0;
	memset(m_item_array, 0, sizeof(m_item_array));
	update_list_size();
}
void c_list_box::select_item(short index)
{
	if (index < 0 || index >= m_item_total)
	{
		ASSERT(false);
	}
	m_selected_item = index;
}
c_slide_group::c_slide_group()
{
	m_gesture = new c_gesture(this);
	for(int i = 0; i < MAX_PAGES; i++)
	{
		m_slides[i] = 0;
	}
	m_active_slide_index = 0;
}
int c_slide_group::set_active_slide(int index, bool is_redraw)
{
	if(index >= MAX_PAGES || index < 0)
	{
		return -1;
	}
	if(0 == m_slides[index])
	{
		return -2;
	}
	m_active_slide_index = index;
	for(int i = 0; i < MAX_PAGES; i++)
	{
		if(m_slides[i] == 0)
		{
			continue;
		}
		if(i == index)
		{
			m_slides[i]->get_surface()->set_active(true);
			add_child_2_tail(m_slides[i]);
			if(is_redraw)
			{
				c_rect rc;
				get_screen_rect(rc);
				m_slides[i]->get_surface()->flush_screen(rc.m_left, rc.m_top, rc.m_right, rc.m_bottom);
			}
		}
		else
		{
			m_slides[i]->get_surface()->set_active(false);
		}
	}
	return 0;
}
int c_slide_group::add_slide(c_wnd* slide, unsigned short resource_id, short x, short y,
			short width, short height, WND_TREE* p_child_tree, Z_ORDER_LEVEL max_zorder)
{
	if(0 == slide)
	{
		return -1;
	}
	c_surface* old_surface = get_surface();
	c_surface* new_surface = old_surface->get_display()->alloc_surface(max_zorder);
	new_surface->set_active(false);
	set_surface(new_surface);
	slide->connect(this, resource_id ,0 , x, y, width, height, p_child_tree);
	set_surface(old_surface);
	int i = 0;
	while(i < MAX_PAGES)
	{
		if(m_slides[i] == slide)
		{//slide has lived
			ASSERT(false);
			return -2;
		}
		i++;
	}
	//new slide
	i = 0;
	while(i < MAX_PAGES)
	{
		if(m_slides[i] == 0)
		{
			m_slides[i] = slide;
			slide->show_window();
			return 0;
		}
		i++;
	}
	//no more slide can be add
	ASSERT(false);
	return -3;
}
int c_slide_group::add_clone_silde(c_wnd* slide, unsigned short resource_id, short x, short y,
			short width, short height, WND_TREE* p_child_tree, Z_ORDER_LEVEL max_zorder)
{
	if(0 == slide)
	{
		return -1;
	}
	c_surface* old_surface = get_surface();
	c_surface* new_surface = old_surface->get_display()->alloc_surface(max_zorder);
	new_surface->set_active(false);
	set_surface(new_surface);
	c_wnd* page_tmp = slide->connect_clone(this,resource_id,0,x,y,width,height,p_child_tree);
	set_surface(old_surface);
	int i = 0;
	while(i < MAX_PAGES)
	{
		if(m_slides[i] == page_tmp)
		{//slide has lived
			ASSERT(false);
			return -2;
		}
		i++;
	}
	//new slide
	i = 0;
	while(i < MAX_PAGES)
	{
		if(m_slides[i] == 0)
		{
			m_slides[i] = page_tmp;
			page_tmp->show_window();
			return 0;
		}
		i++;
	}
	//no more slide can be add
	ASSERT(false);
	return -3;
}
void c_slide_group::disabel_all_slide()
{
	for(int i = 0; i < MAX_PAGES; i++)
	{
		if(m_slides[i])
		{
			m_slides[i]->get_surface()->set_active(false);
		}
	}
}
bool c_slide_group::on_touch(int x, int y, TOUCH_ACTION action)
{
	x -= m_wnd_rect.m_left;
	y -= m_wnd_rect.m_top;
	if (m_gesture->handle_swipe(x, y, action))
	{
		if (m_slides[m_active_slide_index])
		{
			m_slides[m_active_slide_index]->on_touch(x, y, action);
		}
	}
	return true;
}
bool c_slide_group::on_key(KEY_TYPE key)
{
	if (m_slides[m_active_slide_index])
	{
		m_slides[m_active_slide_index]->on_key(key);
	}
	return true;
}
#define ARROW_BT_HEIGHT		55
#define ID_BT_ARROW_UP      1
#define ID_BT_ARROW_DOWN    2
GL_BEGIN_MESSAGE_MAP(c_spin_box)
ON_GL_BN_CLICKED(ID_BT_ARROW_UP, c_spin_box::on_arrow_up_bt_click)
ON_GL_BN_CLICKED(ID_BT_ARROW_DOWN, c_spin_box::on_arrow_down_bt_click)
GL_END_MESSAGE_MAP()
void c_spin_box::pre_create_wnd()
{
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
	m_font_type = c_theme::get_font(FONT_DEFAULT);
	m_font_color = c_theme::get_color(COLOR_WND_FONT);
	m_max = 6;
	m_min = 1;
	m_digit = 0;
	m_step = 1;
	//set arrow button position.
	c_rect rect;
	get_screen_rect(rect);
	m_bt_up_rect.m_left = rect.m_left;
	m_bt_up_rect.m_right = rect.m_left + rect.Width() / 2 - 1;
	m_bt_up_rect.m_top = rect.m_bottom + 1;
	m_bt_up_rect.m_bottom = m_bt_up_rect.m_top + ARROW_BT_HEIGHT;
	m_bt_down_rect.m_left = rect.m_left + rect.Width() / 2;
	m_bt_down_rect.m_right = rect.m_right;
	m_bt_down_rect.m_top = rect.m_bottom + 1;
	m_bt_down_rect.m_bottom = m_bt_down_rect.m_top + ARROW_BT_HEIGHT;
}
bool c_spin_box::on_touch(int x, int y, TOUCH_ACTION action)
{
	(action == TOUCH_DOWN) ? on_touch_down(x, y) : on_touch_up(x, y);
	return c_wnd::on_touch(x, y, action);
}
void c_spin_box::on_touch_down(int x, int y)
{
	if (false == m_wnd_rect.PtInRect(x, y))
	{//maybe click on up/down arrow button
		return;
	}
	if (STATUS_NORMAL == m_status)
	{
		m_parent->set_child_focus(this);
	}
}
void c_spin_box::on_touch_up(int x, int y)
{
	if (false == m_wnd_rect.PtInRect(x, y))
	{//maybe click on up/down arrow button
		return;
	}
	if (STATUS_FOCUSED == m_status)
	{
		m_status = STATUS_PUSHED;
		on_paint();
	}
	else if (STATUS_PUSHED == m_status)
	{
		m_value = m_cur_value;
		m_status = STATUS_FOCUSED;
		on_paint();
		notify_parent(GL_SPIN_CONFIRM, m_value);
	}
}
void c_spin_box::on_focus()
{
	m_status = STATUS_FOCUSED;
	on_paint();
}
void c_spin_box::on_kill_focus()
{
	m_cur_value = m_value;
	m_status = STATUS_NORMAL;
	on_paint();
}
void c_spin_box::show_arrow_button()
{
	m_bt_up.connect(this, ID_BT_ARROW_UP, "\xe2\x96\xb2"/*unicode of up arrow*/, 0, m_wnd_rect.Height(), m_bt_up_rect.Width(),m_bt_up_rect.Height());
	m_bt_up.show_window();
	m_bt_down.connect(this, ID_BT_ARROW_DOWN, "\xe2\x96\xbc"/*unicode of down arrow*/, m_bt_up_rect.Width(), m_wnd_rect.Height(), m_bt_down_rect.Width(),m_bt_down_rect.Height());
	m_bt_down.show_window();
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_MODAL);
}
void c_spin_box::hide_arrow_button()
{
	m_bt_up.disconnect();
	m_bt_down.disconnect();
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
}
void c_spin_box::on_paint()
{
	c_rect rect, tmp_rect, empty_rect;
	get_screen_rect(rect);
	tmp_rect.m_left = rect.m_left;
	tmp_rect.m_right = rect.m_right;
	switch(m_status)
	{
	case STATUS_NORMAL:
		if (m_z_order > m_parent->get_z_order())
		{
			hide_arrow_button();
			m_surface->set_frame_layer_visible_rect(empty_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
		}
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
		c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
		break;
	case STATUS_FOCUSED:
		if (m_z_order > m_parent->get_z_order())
		{
			hide_arrow_button();
			m_surface->set_frame_layer_visible_rect(empty_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
		}
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
		c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
		break;
	case STATUS_PUSHED:
		if (m_z_order == m_parent->get_z_order())
		{
			m_z_order++;
		}
		tmp_rect.m_top = m_bt_down_rect.m_top;
		tmp_rect.m_bottom = m_bt_down_rect.m_bottom;
		m_surface->set_frame_layer_visible_rect(tmp_rect, m_z_order);
		show_arrow_button();
		m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_PUSHED), m_parent->get_z_order());
		m_surface->draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_BORDER), m_parent->get_z_order(), 2);
		c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_PUSHED), ALIGN_HCENTER | ALIGN_VCENTER);
		break;
	default:
		ASSERT(false);
	}
}
void c_spin_box::on_arrow_up_bt_click(unsigned int ctr_id)
{
	if (m_cur_value + m_step > m_max)
	{
		return;
	}
	m_cur_value += m_step;
	notify_parent(GL_SPIN_CHANGE, m_cur_value);
	on_paint();
}
void c_spin_box::on_arrow_down_bt_click(unsigned int ctr_id)
{
	if (m_cur_value - m_step < m_min)
	{
		return;
	}
	m_cur_value -= m_step;
	notify_parent(GL_SPIN_CHANGE, m_cur_value);
	on_paint();
}
void c_table::set_item(int row, int col, char* str, unsigned int color)
{
	draw_item( row, col, str, color);
}
void c_table::draw_item(int row, int col, const char* str, unsigned int color)
{
	c_rect rect = get_item_rect(row, col);
	m_surface->fill_rect(rect.m_left+1, rect.m_top+1, rect.m_right-1, rect.m_bottom-1, color, m_z_order);
	c_word::draw_string_in_rect(m_surface, m_z_order, str, rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_align_type);
}
void c_table::set_row_height(unsigned int height)
{
	for (unsigned int i = 0; i < m_row_num; i ++)
	{
		m_row_height[i] = height;
	}
}
void c_table::set_col_width(unsigned int width)
{
	for (unsigned int i = 0; i < m_col_num; i ++)
	{
		m_col_width[i] = width;
	}
}
int c_table::set_row_height(unsigned int index, unsigned int height)
{
	if (m_row_num > index)
	{
		m_row_height[index] = height;
		return index;
	}
	return -1;
}
int c_table::set_col_width(unsigned int index, unsigned int width)
{
	if (m_col_num > index)
	{
		m_col_width[index] = width;
		return index;
	}
	return -1;
}
c_rect c_table::get_item_rect(int row, int col)
{
	static c_rect rect;
	if (row >= MAX_ROW_NUM || col >= MAX_COL_NUM)
	{
		return rect;
	}
	unsigned int width = 0;
	unsigned int height = 0;
	for (int i = 0; i < col; i++)
	{
		width += m_col_width[i];
	}
	for (int j = 0; j < row; j++)
	{
		height += m_row_height[j];
	}
	c_rect wRect;
	get_screen_rect(wRect);
	rect.m_left = wRect.m_left + width;
	rect.m_right = rect.m_left + m_col_width[col];
	if (rect.m_right > wRect.m_right)
	{
		rect.m_right = wRect.m_right;
	}
	rect.m_top = wRect.m_top + height;
	rect.m_bottom = rect.m_top + m_row_height[row];
	if (rect.m_bottom > wRect.m_bottom)
	{
		rect.m_bottom = wRect.m_bottom;
	}
	return rect;
}
#include <string.h>
#include <stdio.h>
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
c_wave_buffer::c_wave_buffer()
{
	m_head = m_tail = m_min_old = m_max_old =
	m_min_older = m_max_older = m_last_data = m_read_cache_sum = m_refresh_sequence = 0;
	memset(m_wave_buf, 0, sizeof(m_wave_buf));
	memset(m_read_cache_min, 0, sizeof(m_read_cache_min));
	memset(m_read_cache_mid, 0, sizeof(m_read_cache_mid));
	memset(m_read_cache_max, 0, sizeof(m_read_cache_max));
}
short c_wave_buffer::get_cnt()
{
	return (m_tail >= m_head)?(m_tail - m_head):(m_tail - m_head + WAVE_BUFFER_LEN);
}
int c_wave_buffer::write_wave_data(short data)
{
	if ((m_tail + 1) % WAVE_BUFFER_LEN == m_head)
	{//full
		//log_out("wave buf full\n");
		return BUFFER_FULL;
	}
	m_wave_buf[m_tail] = data;
	m_tail = (m_tail + 1) % WAVE_BUFFER_LEN;
	return 1;
}
int c_wave_buffer::read_data()
{
	if (m_head == m_tail)
	{//empty
		//log_out("wave buf empty\n");
		return BUFFER_EMPTY;
	}
	int ret = m_wave_buf[m_head];
	m_head = (m_head + 1) % WAVE_BUFFER_LEN;
	return ret;
}
int c_wave_buffer::read_wave_data_by_frame(short &max, short &min, short frame_len, unsigned int sequence, short offset)
{
	if (m_refresh_sequence != sequence)
	{
		m_refresh_sequence = sequence;
		m_read_cache_sum = 0;
	}
	else if(offset < m_read_cache_sum)//(m_refresh_sequence == sequence && offset < m_fb_sum)
	{
		max = m_read_cache_max[offset];
		min = m_read_cache_min[offset];
		return m_read_cache_mid[offset];
	}
	
	m_read_cache_sum++;
	ASSERT(m_read_cache_sum <= WAVE_READ_CACHE_LEN);
	int i, data;
	int tmp_min = m_last_data;
	int tmp_max = m_last_data;
	int mid = (m_min_old + m_max_old)>>1;
	i = 0;
	while(i++ < frame_len)
	{
		data = read_data();
		if(BUFFER_EMPTY == data)
		{
			break;
		}
		m_last_data = data;
		if(data < tmp_min){tmp_min = data;}
		if(data > tmp_max){tmp_max = data;}
	}
	min = m_read_cache_min[offset] = MIN(m_min_old, MIN(tmp_min, m_min_older));
	max = m_read_cache_max[offset] = MAX(m_max_old, MAX(tmp_max, m_max_older));
	m_min_older = m_min_old;
	m_max_older = m_max_old;
	m_min_old = tmp_min;
	m_max_old = tmp_max;
	return (m_read_cache_mid[offset] = mid);
}
void c_wave_buffer::clear_data()
{
	m_head = m_tail = 0;
	memset(m_wave_buf, 0, sizeof(m_wave_buf));
}
void c_wave_buffer::reset()
{
	m_head = m_tail;
}
#include <stdlib.h>
#include <string.h>
#define CORRECT(x, high_limit, low_limit)	{\
	x = (x > high_limit) ? high_limit : x;\
	x = (x < low_limit) ? low_limit : x;\
}while(0)
#define WAVE_CURSOR_WIDTH		8
#define	WAVE_LINE_WIDTH			1
#define	WAVE_MARGIN				5
c_wave_ctrl::c_wave_ctrl()
{
	m_wave = 0;
	m_bg_fb = 0;
	m_wave_name_font  = m_wave_unit_font = 0;
	m_wave_name = m_wave_unit = 0;
	m_max_data = 500;
	m_min_data = 0;
	m_wave_speed = 1;
	m_wave_data_rate = 0;
	m_wave_refresh_rate = 1000;
	m_frame_len_map_index = 0;
	m_wave_name_color  = m_wave_unit_color = m_wave_color = GL_RGB(255,0,0);
	m_back_color = GL_RGB(0,0,0);
}
void c_wave_ctrl::on_init_children()
{
	c_rect rect;
	get_screen_rect(rect);
	m_wave_left 	= rect.m_left + WAVE_MARGIN;
	m_wave_right	= rect.m_right - WAVE_MARGIN;
	m_wave_top		= rect.m_top + WAVE_MARGIN;
	m_wave_bottom	= rect.m_bottom - WAVE_MARGIN;
	m_wave_cursor	= m_wave_left;
	m_bg_fb = (unsigned int*)calloc(rect.Width() * rect.Height(), 4);
}
void c_wave_ctrl::set_max_min(short max_data, short min_data)
{
	m_max_data = max_data;
	m_min_data = min_data;
}
void c_wave_ctrl::set_wave_in_out_rate(unsigned int data_rate, unsigned int refresh_rate)
{
	m_wave_data_rate = data_rate;
	m_wave_refresh_rate = refresh_rate;
	int read_times_per_second = m_wave_speed * 1000 / m_wave_refresh_rate;
	memset(m_frame_len_map, 0, sizeof(m_frame_len_map));
	for (unsigned int i = 1; i < sizeof(m_frame_len_map) + 1; i++)
	{
		m_frame_len_map[i-1] = data_rate * i / read_times_per_second - data_rate * (i-1) / read_times_per_second;
	}
	m_frame_len_map_index = 0;
}
void c_wave_ctrl::set_wave_speed(unsigned int speed)
{
	m_wave_speed = speed;
	set_wave_in_out_rate(m_wave_data_rate, m_wave_refresh_rate);
}
void c_wave_ctrl::clear_data()
{
	if(m_wave == 0)
	{
		ASSERT(false);
		return;
	}
	m_wave->clear_data();
}
bool c_wave_ctrl::is_data_enough()
{
	if(m_wave == 0)
	{
		ASSERT(false);
		return false;
	}
	return (m_wave->get_cnt() - m_frame_len_map[m_frame_len_map_index] * m_wave_speed);
}
void c_wave_ctrl::refresh_wave(unsigned char frame)
{
	if(m_wave == 0)
	{
		ASSERT(false);
		return;
	}
	short max, min, mid;
	for(short offset = 0; offset < m_wave_speed; offset++)
	{
		//get wave value
		mid = m_wave->read_wave_data_by_frame(max, min,
							m_frame_len_map[m_frame_len_map_index++],
							frame, offset);
		m_frame_len_map_index %= sizeof(m_frame_len_map);
		
		//map to wave ctrl
		int y_min,y_max;
		if(m_max_data == m_min_data)
		{
			ASSERT(false);
		}
		y_max = m_wave_bottom + WAVE_LINE_WIDTH - (m_wave_bottom - m_wave_top)*(min - m_min_data)/(m_max_data - m_min_data);
		y_min = m_wave_bottom - WAVE_LINE_WIDTH - (m_wave_bottom - m_wave_top)*(max - m_min_data)/(m_max_data - m_min_data);
		mid = m_wave_bottom - (m_wave_bottom - m_wave_top)*(mid - m_min_data)/(m_max_data - m_min_data);
		CORRECT(y_min, m_wave_bottom, m_wave_top);
		CORRECT(y_max, m_wave_bottom, m_wave_top);
		CORRECT(mid, m_wave_bottom, m_wave_top);
		if (m_wave_cursor > m_wave_right)
		{
			m_wave_cursor = m_wave_left;
		}
		draw_smooth_vline(y_min, y_max, mid, m_wave_color);
		restore_background();
		m_wave_cursor++;
	}
}
void c_wave_ctrl::draw_smooth_vline(int y_min, int y_max, int mid, unsigned int rgb)
{
	int dy = y_max - y_min;
	short r = GL_RGB_R(rgb);
	short g = GL_RGB_G(rgb);
	short b = GL_RGB_B(rgb);
	int  index = (dy >> 1) + 2;
	int  y;
	m_surface->draw_pixel(m_wave_cursor, mid, rgb, m_z_order);
	if (dy < 1)
	{
		return;
	}
	unsigned char cur_r,cur_g,cur_b;
	unsigned int cur_rgb;
	for (int i = 1; i <= (dy >> 1) + 1; ++i )
	{
		if ( (mid + i) <= y_max )
		{
			y = mid + i;
			cur_r = r * (index - i) / index;
			cur_g = g * (index - i) / index;
			cur_b = b * (index - i) / index;
			cur_rgb = GL_RGB(cur_r, cur_g, cur_b);
			m_surface->draw_pixel(m_wave_cursor, y, cur_rgb, m_z_order);
		}
		if ( (mid - i) >= y_min )
		{
			y = mid - i;
			cur_r = r * (index - i) / index;
			cur_g = g * (index - i) / index;
			cur_b = b * (index - i) / index;
			cur_rgb = GL_RGB(cur_r, cur_g, cur_b);
			m_surface->draw_pixel(m_wave_cursor, y, cur_rgb, m_z_order);
		}
	}
}
void c_wave_ctrl::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	
	m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, m_back_color, m_z_order);
	
	//show name
	c_word::draw_string(m_surface, m_z_order, m_wave_name, m_wave_left + 10, rect.m_top, m_wave_name_font, m_wave_name_color, GL_ARGB(0, 0, 0, 0), ALIGN_LEFT);
	//show unit
	c_word::draw_string(m_surface, m_z_order, m_wave_unit, m_wave_left + 60, rect.m_top, m_wave_unit_font, m_wave_unit_color, GL_ARGB(0, 0, 0, 0), ALIGN_LEFT);
	save_background();
}
void c_wave_ctrl::clear_wave(void)
{
	m_surface->fill_rect(m_wave_left, m_wave_top, m_wave_right, m_wave_bottom, m_back_color, m_z_order);
	m_wave_cursor = m_wave_left;
}
void c_wave_ctrl::restore_background()
{
	int  x = m_wave_cursor + WAVE_CURSOR_WIDTH;
	if (x > m_wave_right)
	{
		x -= (m_wave_right - m_wave_left + 1);
	}
	c_rect rect;
	get_screen_rect(rect);
	register int width = rect.Width();
	register int top = rect.m_top;
	register int left = rect.m_left;
	for (int y_pos = (m_wave_top - 1); y_pos <= (m_wave_bottom + 1); y_pos++)
	{
		(m_bg_fb) ? m_surface->draw_pixel(x, y_pos, m_bg_fb[(y_pos - top) * width + (x - left)], m_z_order) : m_surface->draw_pixel(x, y_pos, 0, m_z_order);
	}
}
void c_wave_ctrl::save_background()
{
	if (!m_bg_fb)
	{
		return;
	}
	c_rect rect;
	get_screen_rect(rect);
	
	register unsigned int* p_des = m_bg_fb;
	for (int y = rect.m_top; y <= rect.m_bottom; y++)
	{
		for (int x = rect.m_left; x <= rect.m_right; x++)
		{
			*p_des++ = m_surface->get_pixel(x, y, m_z_order);
		}
	}
}
