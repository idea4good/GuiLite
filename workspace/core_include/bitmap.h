#ifndef GUILITE_CORE_INCLUDE_BITMAP_H
#define GUILITE_CORE_INCLUDE_BITMAP_H

#include "../core_include/api.h"
#include "../core_include/resource.h"
#include "../core_include/rect.h"
#include "../core_include/display.h"

#define	DEFAULT_MASK_COLOR 0xFF080408
class c_surface;
class c_bitmap
{
public:
	static void draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, int x, int y, unsigned int mask_rgb = DEFAULT_MASK_COLOR)
	{
		ASSERT(pBitmap);
		unsigned short* lower_fb_16 = 0;
		unsigned int* lower_fb_32 = 0;
		int lower_fb_width = 0;
		c_rect lower_fb_rect;
		if (z_order >= Z_ORDER_LEVEL_1)
		{
			lower_fb_16 = (unsigned short*)surface->m_overlap_zones[z_order - 1].fb;
			lower_fb_32 = (unsigned int*)surface->m_overlap_zones[z_order - 1].fb;
			lower_fb_rect = surface->m_overlap_zones[z_order - 1].rect;
			lower_fb_width = lower_fb_rect.Width();
		}
		unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
		int xsize = pBitmap->width;
		int ysize = pBitmap->height;
		const unsigned short* pData = (const unsigned short*)pBitmap->pixel_color_array;
		int color_bytes = surface->m_color_bytes;
		for (int y_ = y; y_ < y + ysize; y_++)
		{
			for (int x_ = x; x_ < x + xsize; x_++)
			{
				unsigned int rgb = *pData++;
				if (mask_rgb_16 == rgb)
				{
					if (lower_fb_rect.PtInRect(x_, y_))
					{//show lower layer
						surface->draw_pixel(x_, y_, (color_bytes == 4) ? lower_fb_32[(y_ - lower_fb_rect.m_top) * lower_fb_width + (x_ - lower_fb_rect.m_left)] : GL_RGB_16_to_32(lower_fb_16[(y_ - lower_fb_rect.m_top) * lower_fb_width + (x_ - lower_fb_rect.m_left)]), z_order);
					}
				}
				else
				{
					surface->draw_pixel(x_, y_, GL_RGB_16_to_32(rgb), z_order);
				}
			}
		}
	}

	static void draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO* pBitmap, int x, int y, int src_x, int src_y, int width, int height, unsigned int mask_rgb = DEFAULT_MASK_COLOR)
	{
		if (0 == pBitmap || (src_x + width > pBitmap->width) || (src_y + height > pBitmap->height))
		{
			return;
		}

		unsigned short* lower_fb_16 = 0;
		unsigned int* lower_fb_32 = 0;
		int lower_fb_width = 0;
		c_rect lower_fb_rect;
		if (z_order >= Z_ORDER_LEVEL_1)
		{
			lower_fb_16 = (unsigned short*)surface->m_overlap_zones[z_order - 1].fb;
			lower_fb_32 = (unsigned int*)surface->m_overlap_zones[z_order - 1].fb;
			lower_fb_rect = surface->m_overlap_zones[z_order - 1].rect;
			lower_fb_width = lower_fb_rect.Width();
		}
		unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
		const unsigned short* pData = (const unsigned short*)pBitmap->pixel_color_array;
		int color_bytes = surface->m_color_bytes;
		for (int y_ = 0; y_ < height; y_++)
		{
			const unsigned short* p = &pData[src_x + (src_y + y_) * pBitmap->width];
			for (int x_ = 0; x_ < width; x_++)
			{
				unsigned int rgb = *p++;
				if (mask_rgb_16 == rgb)
				{
					if (lower_fb_rect.PtInRect(x + x_, y + y_))
					{//show lower layer
						surface->draw_pixel(x + x_, y + y_, (color_bytes == 4) ? lower_fb_32[(y + y_ - lower_fb_rect.m_top) * lower_fb_width + x + x_ - lower_fb_rect.m_left] : GL_RGB_16_to_32(lower_fb_16[(y + y_ - lower_fb_rect.m_top) * lower_fb_width + x + x_ - lower_fb_rect.m_left]), z_order);
					}
				}
				else
				{
					surface->draw_pixel(x + x_, y + y_, GL_RGB_16_to_32(rgb), z_order);
				}
			}
		}
	}

};

#endif
