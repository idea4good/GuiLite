#ifndef _MY_RESOURCE_H_
#define _MY_RESOURCE_H_

typedef struct struct_font_info		FONT_INFO;
typedef struct struct_color_rect	COLOR_RECT;
typedef struct struct_bitmap_info	BITMAP_INFO;

//Rebuild gui library once you change this file
enum FONT_TYPE
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

enum BITMAP_TYPE
{
	BITMAP_UP_ARROW1,
	BITMAP_UP_ARROW2,
	BITMAP_DOWN_ARROW1,
	BITMAP_DOWN_ARROW2,

	BITMAP_CUSTOM1,
	BITMAP_CUSTOM2,
	BITMAP_CUSTOM3,
	BITMAP_CUSTOM4,
	BITMAP_CUSTOM5,
	BITMAP_CUSTOM6,

	BITMAP_MAX
};

enum SHAPE_TYPE
{
	BUTTON_NORMAL,
	BUTTON_FOCUS,
	BUTTON_PUSH,

	LIST_BOX_SELECT,
	LIST_BOX_PUSH,
	LIST_BOX_EXTEND,

	KEY_BUTTON_NORMAL,
	KEY_BUTTON_PUSH,

	SHAPE_CUSTOM1,
	SHAPE_CUSTOM2,
	SHAPE_CUSTOM3,
	SHAPE_MAX
};

enum COLOR_TYPE
{
	WND_BACKCOLOR,
	WND_FORECOLOR,

	COLOR_MAX
};

class c_my_resource
{
public:
	static int add_font(FONT_TYPE index, const FONT_INFO* font);
	static const FONT_INFO* get_font(FONT_TYPE index);

	static int add_bitmap(BITMAP_TYPE index, const BITMAP_INFO* bmp);
	static const BITMAP_INFO* get_bmp(BITMAP_TYPE index);

	static int add_shape(SHAPE_TYPE index, const COLOR_RECT* shape);
	static const COLOR_RECT* get_shape(SHAPE_TYPE index);

	static int add_color(COLOR_TYPE index, const unsigned int color);
	static const unsigned int get_color(COLOR_TYPE index);
};

#endif
