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
		if (0 == pBitmap)
		{
			return;
		}
		unsigned short* lower_fb_16 = 0;
		unsigned int* lower_fb_32 = 0;
		int lower_fb_width = surface->m_width;
		if (z_order >= Z_ORDER_LEVEL_1)
		{
			lower_fb_16 = (unsigned short*)surface->m_frame_layers[z_order - 1].fb;
			lower_fb_32 = (unsigned int*)surface->m_frame_layers[z_order - 1].fb;
		}
		unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
		int xsize = pBitmap->width;
		int ysize = pBitmap->height;
		const unsigned short* pData = (const unsigned short*)pBitmap->pixel_color_array;
		int color_bytes = surface->m_color_bytes;
		for (int j = 0; j < ysize; j++)
		{
			for (int i = 0; i < xsize; i++)
			{
				unsigned int rgb = *pData++;
				if (mask_rgb_16 == rgb)
				{
					if (lower_fb_16)
					{//restore lower layer
						surface->draw_pixel(x + i, y + j, (color_bytes == 4) ? lower_fb_32[(y + j) * lower_fb_width + x + i] : GL_RGB_16_to_32(lower_fb_16[(y + j) * lower_fb_width + x + i]), z_order);
					}
				}
				else
				{
					surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(rgb), z_order);
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
		int lower_fb_width = surface->m_width;
		if (z_order >= Z_ORDER_LEVEL_1)
		{
			lower_fb_16 = (unsigned short*)surface->m_frame_layers[z_order - 1].fb;
			lower_fb_32 = (unsigned int*)surface->m_frame_layers[z_order - 1].fb;
		}
		unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
		const unsigned short* pData = (const unsigned short*)pBitmap->pixel_color_array;
		int color_bytes = surface->m_color_bytes;
		for (int j = 0; j < height; j++)
		{
			const unsigned short* p = &pData[src_x + (src_y + j) * pBitmap->width];
			for (int i = 0; i < width; i++)
			{
				unsigned int rgb = *p++;
				if (mask_rgb_16 == rgb)
				{
					if (lower_fb_16)
					{//restore lower layer
						surface->draw_pixel(x + i, y + j, (color_bytes == 4) ? lower_fb_32[(y + j) * lower_fb_width + x + i] : GL_RGB_16_to_32(lower_fb_16[(y + j) * lower_fb_width + x + i]), z_order);
					}
				}
				else
				{
					surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(rgb), z_order);
				}
			}
		}
	}

};

#endif
