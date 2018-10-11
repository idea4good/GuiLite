#ifndef _MY_RESOURCE_H_
#define _MY_RESOURCE_H_

typedef struct struct_font_info		FONT_INFO;
typedef struct struct_color_rect	COLOR_RECT;
typedef struct struct_bitmap_info	BITMAP_INFO;

//Redefine them to meet your need.
#define DEFAULT_NORMAL_COLOR	GL_RGB(211, 211, 211)
#define DEFAULT_FOCUS_COLOR		GL_RGB(188, 185, 182)
#define DEFAULT_PUSH_COLOR		GL_RGB(165, 162, 159)
#define DEFAULT_FONT_COLOR		GL_RGB(0, 0, 0)

enum FONT_TYPE
{
	FONT_NULL,
	FONT_ENG_S,
	FONT_ENG_SB,
	FONT_ENG_SBM,
	FONT_ENG_SMB,
	FONT_ENG_MB,
	FONT_ENG_LLB,
	FONT_ENG_LLLB,

	FONT_MAX
};

enum BITMAP_TYPE
{
	BITMAP_UP_BT_NORMAL,
	BITMAP_UP_BT_FOCUS,
	BITMAP_DOWN_BT_NORMAL,
	BITMAP_DOWN_BT_FOCUS,

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

	SHAPE_MAX
};

enum COLOR_TYPE
{
	CTRL_BACK_GROUND,
	CTRL_FORE_GROUND,

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
