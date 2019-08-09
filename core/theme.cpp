#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/resource.h"
#include "../core_include/theme.h"

static const FONT_INFO* s_font_map[FONT_MAX];
static const BITMAP_INFO* s_bmp_map[BITMAP_MAX];
static unsigned int s_color_map[COLOR_MAX];

int c_theme::add_font(FONT_TYPE index, const FONT_INFO* font)
{
	if (index >= FONT_MAX)
	{
		ASSERT(false);
		return -1;
	}
	s_font_map[index] = font;
	return 0;
}

const FONT_INFO* c_theme::get_font(FONT_TYPE index)
{
	if (index >= FONT_MAX)
	{
		ASSERT(false);
		return 0;
	}
	return s_font_map[index];
}

int c_theme::add_bitmap(BITMAP_TYPE index, const BITMAP_INFO* bmp)
{
	if (index >= BITMAP_MAX)
	{
		ASSERT(false);
		return -1;
	}
	s_bmp_map[index] = bmp;
	return 0;
}

const BITMAP_INFO* c_theme::get_bmp(BITMAP_TYPE index)
{
	if (index >= BITMAP_MAX)
	{
		ASSERT(false);
		return 0;
	}
	return s_bmp_map[index];
}

int c_theme::add_color(COLOR_TYPE index, const unsigned int color)
{
	if (index >= COLOR_MAX)
	{
		ASSERT(false);
		return -1;
	}
	s_color_map[index] = color;
	return 0;
}

const unsigned int c_theme::get_color(COLOR_TYPE index)
{
	if (index >= COLOR_MAX)
	{
		ASSERT(false);
		return 0;
	}
	return s_color_map[index];
}