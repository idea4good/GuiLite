#pragma once

#include "../core/api.h"
#include "../core/resource.h"

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

enum IMAGE_LIST
{
	IMAGE_CUSTOM1,
	IMAGE_CUSTOM2,
	IMAGE_CUSTOM3,
	IMAGE_CUSTOM4,
	IMAGE_CUSTOM5,
	IMAGE_CUSTOM6,

	IMAGE_MAX
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
		s_font_map[index] = font;
		return 0;
	}

	static const void* get_font(FONT_LIST index)
	{
		if (index >= FONT_MAX)
		{
			ASSERT(false);
			return 0;
		}
		return s_font_map[index];
	}

	static int add_image(IMAGE_LIST index, const void* image_info)
	{
		if (index >= IMAGE_MAX)
		{
			ASSERT(false);
			return -1;
		}
		s_image_map[index] = image_info;
		return 0;
	}

	static const void* get_image(IMAGE_LIST index)
	{
		if (index >= IMAGE_MAX)
		{
			ASSERT(false);
			return 0;
		}
		return s_image_map[index];
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
	static const void* s_font_map[FONT_MAX];
	static const void* s_image_map[IMAGE_MAX];
	static unsigned int s_color_map[COLOR_MAX];
};
