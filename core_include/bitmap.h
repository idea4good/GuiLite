#ifndef BITMAP_UNIT_H
#define BITMAP_UNIT_H

#define	DEFAULT_MASK_COLOR 0xFF080408
class c_surface;
class c_bitmap
{
public:
	static void draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, int x, int y, unsigned int mask_rgb = DEFAULT_MASK_COLOR);
	static void draw_bitmap_in_rect(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, c_rect rect, unsigned int align_type, unsigned int mask_rgb = DEFAULT_MASK_COLOR);
private:
	static void draw_bitmap_565(c_surface* surface, int z_order, int x, int y, int xsize, int ysize, const unsigned char* pPixel, unsigned int mask_color);
	static void get_bitmap_pos(const BITMAP_INFO *pBitmap, c_rect rect, unsigned int align_type, int &x, int &y);
	static void draw_bitmap_565_in_rect(c_surface* surface, int z_order, int x, int y, int width, int height, int xsize, int ysize, const unsigned char* pPixel, unsigned int mask_color);
};

#endif
