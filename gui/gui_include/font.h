#ifndef _FONT_H_
#define _FONT_H_

enum FONT_TYPE
{
	FONT_ENG_S,
	FONT_ENG_SB,
	FONT_ENG_SBM,
	FONT_ENG_SMB,
	FONT_ENG_SMB_AA,
	FONT_ENG_MB_AA,
	FONT_ENG_MMB_AA,
	FONT_ENG_MB,
	FONT_ENG_LLB,
	FONT_ENG_LLLB,
	FONT_ENG_LLLLB
};

enum BITMAP_TYPE
{
	BITMAP_UP_BT_NORMAL,
	BITMAP_UP_BT_FOCUS,
	BITMAP_DOWN_BT_NORMAL,
	BITMAP_DOWN_BT_FOCUS
};

enum SHAPE_TYPE
{
	BUTTON_NORMAL,
	BUTTON_FOCUS,
	BUTTON_PUSH,
	BUTTON_DISABLE,

	LIST_BOX_SELECT,
	LIST_BOX_PUSH,
	LIST_BOX_EXTEND,

	KEY_BUTTON_NORMAL,
	KEY_BUTTON_PUSH
};

class c_font
{
public:
	static int add_font(FONT_TYPE index, const GUI_FONT* font_resource);
	static const GUI_FONT* get_font(FONT_TYPE index);

	static int add_bitmap(BITMAP_TYPE index, const GUI_BITMAP* bmp_resource);
	static const GUI_BITMAP* get_bmp(BITMAP_TYPE index);

	static int add_shape(SHAPE_TYPE index, const CUSTOM_SHAPE* shape_resource);
	static const CUSTOM_SHAPE* get_shape(SHAPE_TYPE index);
};

#endif
