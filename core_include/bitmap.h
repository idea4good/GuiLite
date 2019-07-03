#ifndef BITMAP_UNIT_H
#define BITMAP_UNIT_H

#define	DEFAULT_MASK_COLOR 0xFF080408
class c_surface;
class c_bitmap
{
public:
	static void draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, int x, int y, unsigned int mask_rgb = DEFAULT_MASK_COLOR);
	static void draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO* pBitmap, int x, int y, int src_x, int src_y, int width, int height, unsigned int mask_rgb = DEFAULT_MASK_COLOR);
};

#endif
