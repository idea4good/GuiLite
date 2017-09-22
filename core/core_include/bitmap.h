#ifndef _BITMAP_UNIT_H_
#define _BITMAP_UNIT_H_

class c_surface;
class c_bitmap
{
public:
	static c_bitmap* get_instance(c_surface* surface)
	{
		static c_bitmap s_the_bitmap;
		ms_surface = surface;
		return &s_the_bitmap;
	}
	static void set_z_oder(int z_order) { ms_z_order = z_order; }

	void draw_bitmap(const GUI_BITMAP *pBitmap, int x, int y);
	void draw_bitmap_in_rect(const GUI_BITMAP *pBitmap, c_rect rect, unsigned int align_type);
private:
	void draw_bitmap_565(int x, int y, int xsize, int ysize, const unsigned char* pPixel);
	void get_bitmap_pos(const GUI_BITMAP *pBitmap, c_rect rect, unsigned int align_type, int &x, int &y);
	void draw_bitmap_565_inrect(int x, int y, int width, int height, int xsize, int ysize, const unsigned char* pPixel);

	static c_surface*	ms_surface;
	static int ms_z_order;

	c_bitmap(){}
	c_bitmap(const c_bitmap&);
	c_bitmap& operator=(const c_bitmap&);
	~c_bitmap(){}
};

#endif
