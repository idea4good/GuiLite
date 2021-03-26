#pragma once

#include "../core_include/api.h"
#include "../core_include/resource.h"

//Rebuild gui library once you change this file
enum FONT_LIST
{
	FONT_NULL,
	FONT_DEFAULT,
	FONT_CUSTOM1,
	FONT_CUSTOM2,
	FONT_CUSTOM3,
	FONT_CUSTOM4,
	FONT_CUSTOM5,
	FONT_CUSTOM6,

	FONT_MAX
};

enum BITMAP_LIST
{
	BITMAP_CUSTOM1,
	BITMAP_CUSTOM2,
	BITMAP_CUSTOM3,
	BITMAP_CUSTOM4,
	BITMAP_CUSTOM5,
	BITMAP_CUSTOM6,

	BITMAP_MAX
};

enum COLOR_LIST
{
	COLOR_WND_FONT,
	COLOR_WND_NORMAL,
	COLOR_WND_PUSHED,
	COLOR_WND_FOCUS,
	COLOR_WND_BORDER,

	COLOR_CUSTOME1,
	COLOR_CUSTOME2,
	COLOR_CUSTOME3,
	COLOR_CUSTOME4,
	COLOR_CUSTOME5,
	COLOR_CUSTOME6,

	COLOR_MAX
};

class c_theme
{
public:
	static int add_font(FONT_LIST index, const void* font)
	{
		if (index >= FONT_MAX)
		{
			ASSERT(false);
			return -1;
		}
		s_font_map[index].font = font;
		return 0;
	}
	static const void* get_font(FONT_LIST index)
	{
		if (index >= FONT_MAX)
		{
			ASSERT(false);
			return 0;
		}
		return s_font_map[index].font;
	}
	static int add_bitmap(BITMAP_LIST index, const BITMAP_INFO* bmp)
	{
		if (index >= BITMAP_MAX)
		{
			ASSERT(false);
			return -1;
		}
		s_bmp_map[index] = bmp;
		return 0;
	}
	static const BITMAP_INFO* get_bmp(BITMAP_LIST index)
	{
		if (index >= BITMAP_MAX)
		{
			ASSERT(false);
			return 0;
		}
		return s_bmp_map[index];
	}
	static int add_color(COLOR_LIST index, const unsigned int color)
	{
		if (index >= COLOR_MAX)
		{
			ASSERT(false);
			return -1;
		}
		s_color_map[index] = color;
		return 0;
	}
	static const unsigned int get_color(COLOR_LIST index)
	{
		if (index >= COLOR_MAX)
		{
			ASSERT(false);
			return 0;
		}
		return s_color_map[index];
	}
private:
	static FONT_INFO s_font_map[FONT_MAX];
	static const BITMAP_INFO* s_bmp_map[BITMAP_MAX];
	static unsigned int s_color_map[COLOR_MAX];
};
