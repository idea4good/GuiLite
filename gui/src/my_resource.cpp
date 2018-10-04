#include "core_include/rect.h"
#include "core_include/resource.h"
#include "../gui_include/my_resource.h"

static const FONT_INFO* s_font_map[FONT_MAX];
static const BITMAP_INFO* s_bmp_map[BITMAP_MAX];
static const COLOR_RECT* s_shape_map[SHAPE_MAX];
static unsigned int s_color_map[COLOR_MAX];

int c_my_resource::add_font(FONT_TYPE index, const FONT_INFO* font)
{
	s_font_map[index] = font;
	return 0;
}

const FONT_INFO* c_my_resource::get_font(FONT_TYPE index)
{
	return s_font_map[index];
}

int c_my_resource::add_bitmap(BITMAP_TYPE index, const BITMAP_INFO* bmp)
{
	s_bmp_map[index] = bmp;
	return 0;
}

const BITMAP_INFO* c_my_resource::get_bmp(BITMAP_TYPE index)
{
	return s_bmp_map[index];
}

int c_my_resource::add_shape(SHAPE_TYPE index, const COLOR_RECT* shape)
{
	s_shape_map[index] = shape;
	return 0;
}

const COLOR_RECT* c_my_resource::get_shape(SHAPE_TYPE index)
{
	return s_shape_map[index];
}

int c_my_resource::add_color(COLOR_TYPE index, const unsigned int color)
{
	s_color_map[index] = color;
	return 0;
}

const unsigned int c_my_resource::get_color(COLOR_TYPE index)
{
	return s_color_map[index];
}