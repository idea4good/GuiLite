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

class c_font
{
public:
	static int add_font(int index, const GUI_FONT* font_resource);
	static const GUI_FONT* get_font(int index);
};

#endif
