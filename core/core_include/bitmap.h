#ifndef _BITMAP_UNIT_H_
#define _BITMAP_UNIT_H_

class c_surface;
class c_bitmap
{
public:
	static void draw_bitmap(c_surface* surface, int z_order, const GUI_BITMAP *pBitmap, int x, int y);
	static void draw_bitmap_in_rect(c_surface* surface, int z_order, const GUI_BITMAP *pBitmap, c_rect rect, unsigned int align_type);
private:
	static void draw_bitmap_565(c_surface* surface, int z_order, int x, int y, int xsize, int ysize, const unsigned char* pPixel);
	static void get_bitmap_pos(const GUI_BITMAP *pBitmap, c_rect rect, unsigned int align_type, int &x, int &y);
	static void draw_bitmap_565_inrect(c_surface* surface, int z_order, int x, int y, int width, int height, int xsize, int ysize, const unsigned char* pPixel);

	c_bitmap(){}
	c_bitmap(const c_bitmap&);
	c_bitmap& operator=(const c_bitmap&);
	~c_bitmap(){}
};

#endif
