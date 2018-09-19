#include "core_include/rect.h"
#include "core_include/resource.h"
#include "../gui_include/my_resource.h"
#include <map>

static std::map<int, const GUI_FONT*> s_font_map;
static std::map<int, const GUI_BITMAP*> s_bmp_map;
static std::map<int, const COLOR_RECT*> s_shape_map;

int c_my_resource::add_font(FONT_TYPE index, const GUI_FONT* font_resource)
{
	s_font_map[index] = font_resource;
	return 0;
}

const GUI_FONT* c_my_resource::get_font(FONT_TYPE index)
{
	return s_font_map[index];
}

int c_my_resource::add_bitmap(BITMAP_TYPE index, const GUI_BITMAP* bmp_resource)
{
	s_bmp_map[index] = bmp_resource;
	return 0;
}

const GUI_BITMAP* c_my_resource::get_bmp(BITMAP_TYPE index)
{
	return s_bmp_map[index];
}

int c_my_resource::add_shape(SHAPE_TYPE index, const COLOR_RECT* shape_resource)
{
	s_shape_map[index] = shape_resource;
	return 0;
}

const COLOR_RECT* c_my_resource::get_shape(SHAPE_TYPE index)
{
	return s_shape_map[index];
}