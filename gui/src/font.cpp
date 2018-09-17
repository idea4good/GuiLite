#include "core_include/rect.h"
#include "core_include/resource_type.h"
#include "../gui_include/font.h"
#include <map>

static std::map<int, const GUI_FONT*> s_font_map;
static std::map<int, const GUI_BITMAP*> s_bmp_map;
static std::map<int, const CUSTOM_SHAPE*> s_shape_map;

int c_font::add_font(FONT_TYPE index, const GUI_FONT* font_resource)
{
	s_font_map[index] = font_resource;
	return 0;
}

const GUI_FONT* c_font::get_font(FONT_TYPE index)
{
	return s_font_map[index];
}

int c_font::add_bitmap(BITMAP_TYPE index, const GUI_BITMAP* bmp_resource)
{
	s_bmp_map[index] = bmp_resource;
	return 0;
}

const GUI_BITMAP* c_font::get_bmp(BITMAP_TYPE index)
{
	return s_bmp_map[index];
}

int c_font::add_shape(SHAPE_TYPE index, const CUSTOM_SHAPE* shape_resource)
{
	s_shape_map[index] = shape_resource;
	return 0;
}

const CUSTOM_SHAPE* c_font::get_shape(SHAPE_TYPE index)
{
	return s_shape_map[index];
}