#include "../core_include/api.h"
#include "../core_include/resource.h"
#include "../core_include/rect.h"
#include "../core_include/bitmap.h"
#include "../core_include/surface.h"

void c_bitmap::draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, int x, int y, unsigned int mask_rgb)
{
	if (0 == pBitmap) {	return;	}
	draw_bitmap_565(surface, z_order, x, y, pBitmap->XSize, pBitmap->YSize,
		(unsigned char const *)pBitmap->pData, mask_rgb);
}

void c_bitmap::draw_bitmap_in_rect(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, c_rect rect, unsigned int align_type, unsigned int mask_rgb)
{
	if (0 == pBitmap) {	return;	}
	int x, y;
	get_bitmap_pos(pBitmap, rect, align_type, x, y);
	draw_bitmap_565_in_rect(surface, z_order, rect.m_left + x, rect.m_top + y,
			(rect.m_right - rect.m_left + 1), (rect.m_bottom - rect.m_top + 1),
			pBitmap->XSize, pBitmap->YSize, (unsigned char const *)pBitmap->pData, mask_rgb);
}

void c_bitmap::get_bitmap_pos(const BITMAP_INFO *pBitmap, c_rect rect, unsigned int align_type, int &x, int &y)
{
	int x_size = pBitmap->XSize;
	int y_size = pBitmap->YSize;

	int height = rect.m_bottom - rect.m_top + 1;
	int width  = rect.m_right - rect.m_left + 1;

	x = y = 0;

	switch (align_type & ALIGN_HMASK)
	{
	case ALIGN_HCENTER:
		if (width > x_size)
		{
			x = (width - x_size)/2;
		}
		break;
	case ALIGN_LEFT:
		x = 0;
		break;
	case ALIGN_RIGHT:
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
		if (height > y_size)
		{
			y = (height - y_size)/2;
		}
		break;
	case ALIGN_TOP:
		y = 0;
		break;
	case ALIGN_BOTTOM:
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

void c_bitmap::draw_bitmap_565(c_surface* surface, int z_order, int x, int y, int xsize, int ysize, const unsigned char* pPixel, unsigned int mask_rgb)
{
	unsigned short* lower_fb = NULL;
	int lower_fb_width = surface->m_width;
	if (z_order >= Z_ORDER_LEVEL_1)
	{
		lower_fb = surface->m_frame_layers[z_order - 1].fb;
	}
	unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
	const unsigned short* pData = (const unsigned short*)pPixel;
	for (int j = 0; j < ysize; j++) 
	{
		const unsigned short * p = pData;
		for (int i = 0; i < xsize; i++) 
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
		pData += xsize;
	}
}

void c_bitmap::draw_bitmap_565_in_rect(c_surface* surface, int z_order, int x, int y, int width, int height, int xsize, int ysize, const unsigned char* pPixel, unsigned int mask_rgb)
{
	unsigned short* lower_fb = NULL;
	int lower_fb_width = surface->m_width;
	if (z_order >= Z_ORDER_LEVEL_1)
	{
		lower_fb = surface->m_frame_layers[z_order - 1].fb;
	}
	unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
	const unsigned short* pData = (const unsigned short*)pPixel;
	for (int j = 0; j < ysize; j++)
	{
		if (j >= height) { break; }
		const unsigned short * p = pData;
		for (int i = 0; i < xsize; i++)
		{
			if (i >= width) { break; }
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
		pData += xsize;
	}
}
