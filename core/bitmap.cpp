#include "../core_include/api.h"
#include "../core_include/resource.h"
#include "../core_include/rect.h"
#include "../core_include/bitmap.h"
#include "../core_include/surface.h"

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
	int xsize = pBitmap->XSize;
	int ysize = pBitmap->YSize;
	const unsigned short* pData = (const unsigned short*)pBitmap->pData;
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
	if (0 == pBitmap || (src_x + width > pBitmap->XSize) || (src_y + height > pBitmap->XSize))
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
	const unsigned short* pData = (const unsigned short*)pBitmap->pData;
	for (int j = 0; j < height; j++)
	{
		const unsigned short* p = &pData[src_x + (src_y + j) * pBitmap->XSize];
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
