#include "core_include/resource_type.h"
#include "../gui_include/font.h"
#include <map>

static std::map<int, const GUI_FONT*> s_font_map;

int c_font::add_font(int index, const GUI_FONT* font_resource)
{
	s_font_map[index] = font_resource;
	return 0;
}

const GUI_FONT* c_font::get_font(int index)
{
	return s_font_map[index];
}