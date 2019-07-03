#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/surface.h"
#include <string.h>
#include <stdio.h>

#define BUFFER_LEN	16
unsigned char s_utf8_length_table[256] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

inline static int get_utf8_code(const char* s, unsigned int& output_utf8_code)
{
	unsigned char* us = (unsigned char*)s;
	int utf8_bytes = s_utf8_length_table[*us];
	switch (utf8_bytes)
	{
	case 1:
		output_utf8_code = *us;
		break;
	case 2:
		output_utf8_code = (*us << 8) | (*(us + 1));
		break;
	case 3:
		output_utf8_code = (*us << 16) | ((*(us + 1)) << 8) | *(us + 2);
		break;
	case 4:
		output_utf8_code = (*us << 24) | ((*(us + 1)) << 16) | (*(us + 2) << 8) | *(us + 3);
		break;
	default:
		ASSERT(false);
		break;
	}
	return utf8_bytes;
}

void c_word::draw_value_in_rect(c_surface* surface, int z_order, int value, int dot_position, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
	char buf[BUFFER_LEN];
	value_2_string(value, dot_position, buf, BUFFER_LEN);
	draw_string_in_rect(surface, z_order, buf, rect, font, font_color, bg_color, align_type);
}

void c_word::draw_value(c_surface* surface, int z_order, int value, int dot_position, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
	char buf[BUFFER_LEN];
	value_2_string(value, dot_position, buf, BUFFER_LEN);
	draw_string(surface, z_order, buf, x, y, font, font_color, bg_color, align_type);
}

void c_word::draw_string_in_rect(c_surface* surface, int z_order, const char *s, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
	if(0 == s)
	{
		return;
	}
	int x, y;
	get_string_pos(s, font, rect, align_type, x, y);
	draw_string(surface, z_order, s, rect.m_left + x, rect.m_top + y, font, font_color, bg_color, ALIGN_LEFT);
}

void c_word::draw_string(c_surface* surface, int z_order, const char *s, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
	if (0 == s)
	{
		return;
	}

	int offset = 0;
	unsigned int utf8_code;
	while (*s)
	{
		s += get_utf8_code(s, utf8_code);
		offset += draw_single_char(surface, z_order, utf8_code, (x + offset), y, font, font_color, bg_color);
	}
}

void c_word::value_2_string(int value, int dot_position, char* buf, int len)
{
	memset(buf, 0, len);
	switch (dot_position)
	{
	case 0:
		sprintf(buf, "%d", value);
		break;
	case 1:
		sprintf(buf, "%.1f", value*1.0 / 10);
		break;
	case 2:
		sprintf(buf, "%.2f", value*1.0 / 100);
		break;
	case 3:
		sprintf(buf, "%.3f", value*1.0 / 1000);
		break;
	default:
		ASSERT(false);
		break;
	}
}

const LATTICE* c_word::get_lattice(const FONT_INFO* font, unsigned int utf8_code)
{
	int first = 0;
	int last = font->count - 1;
	int middle = (first + last) / 2;

	while (first <= last) 
	{
		if (font->lattice_array[middle].utf8_code < utf8_code)
			first = middle + 1;
		else if (font->lattice_array[middle].utf8_code == utf8_code)
		{
			return &font->lattice_array[middle];
		}
		else
		{
			last = middle - 1;
		}
		middle = (first + last) / 2;
	}
	return 0;
}

int c_word::draw_single_char(c_surface* surface, int z_order, unsigned int utf8_code, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color)
{
	unsigned int error_color = 0xFFFFFFFF;
	if (font)
	{
		const LATTICE* p_lattice = get_lattice(font, utf8_code);
		if (p_lattice)
		{
			draw_lattice(surface, z_order, x, y, p_lattice->width, font->height, p_lattice->p_data, font_color, bg_color);
			return p_lattice->width;
		}
	}
	else
	{
		error_color = GL_RGB(255, 0, 0);
	}
	
	//lattice/font not found, draw "X"
	int len = 16;
	for (int y_ = 0; y_ < len; y_++)
	{
		for (int x_ = 0; x_ < len; x_++)
		{
			int diff = (x_ - y_);
			int sum = (x_ + y_);
			(diff == 0 || diff == -1 || diff == 1 || sum == len || sum == (len - 1) || sum == (len + 1)) ?
			surface->draw_pixel((x + x_), (y + y_), error_color, z_order) : surface->draw_pixel((x + x_), (y + y_), 0, z_order);
		}
	}
	return len;
}

void c_word::draw_lattice(c_surface* surface, int z_order, int x, int y, int width, int height,
						const unsigned char* p_data, unsigned int font_color, unsigned int bg_color)
{
	unsigned int r, g, b, rgb;
	unsigned char blk_value = *p_data++;
	unsigned char blk_cnt = *p_data++;
	b = (GL_RGB_B(font_color) * blk_value + GL_RGB_B(bg_color) * (255 - blk_value)) >> 8;
	g = (GL_RGB_G(font_color) * blk_value + GL_RGB_G(bg_color) * (255 - blk_value)) >> 8;
	r = (GL_RGB_R(font_color) * blk_value + GL_RGB_R(bg_color) * (255 - blk_value)) >> 8;
	rgb = GL_RGB(r, g, b);
	for (int y_ = 0; y_ < height; y_++)
	{
		for (int x_ = 0; x_ < width; x_++)
		{
			ASSERT(blk_cnt);
			if (0x00 == blk_value)
			{
				if (GL_ARGB_A(bg_color))
				{
					surface->draw_pixel(x + x_, y + y_, bg_color, z_order);
				}
			}
			else
			{
				surface->draw_pixel((x + x_), (y + y_), rgb, z_order);
			}
			if (--blk_cnt == 0)
			{//reload new block
				blk_value = *p_data++;
				blk_cnt = *p_data++;
				b = (GL_RGB_B(font_color) * blk_value + GL_RGB_B(bg_color) * (255 - blk_value)) >> 8;
				g = (GL_RGB_G(font_color) * blk_value + GL_RGB_G(bg_color) * (255 - blk_value)) >> 8;
				r = (GL_RGB_R(font_color) * blk_value + GL_RGB_R(bg_color) * (255 - blk_value)) >> 8;
				rgb = GL_RGB(r, g, b);
			}
		}
	}
}

int c_word::get_str_size(const char *s, const FONT_INFO* font, int& width, int& height)
{
	if(0 == s || 0 == font)
	{
		width = height = 0;
		return -1;
	}

	int lattice_width = 0;
	unsigned int utf8_code;
	int utf8_bytes;
	while (*s)
	{
		utf8_bytes = get_utf8_code(s, utf8_code);
		const LATTICE* p_lattice = get_lattice(font, utf8_code);
		lattice_width += p_lattice?p_lattice->width:font->height;
		s += utf8_bytes;
	}
	width = lattice_width;
	height = font->height;
	return 0;
}

void c_word::get_string_pos(const char *s, const FONT_INFO* font, c_rect rect, unsigned int align_type, int &x, int &y)
{
	int x_size, y_size;
	get_str_size(s, font, x_size, y_size);

	int height = rect.m_bottom - rect.m_top + 1;
	int width  = rect.m_right - rect.m_left + 1;

	x = y = 0;

	switch (align_type & ALIGN_HMASK)
	{
	case ALIGN_HCENTER:
		//m_text_org_x=0
		if (width > x_size)
		{
			x = (width - x_size)/2;
		}
		break;

	case ALIGN_LEFT:
		x = 0;
		break;

	case ALIGN_RIGHT:
		//m_text_org_x=0
		if (width > x_size)
		{
			x = width - x_size;
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	switch (align_type & ALIGN_VMASK)
	{
	case ALIGN_VCENTER:
		//m_text_org_y=0
		if (height > y_size)
		{
			y = (height - y_size)/2;
		}
		break;

	case ALIGN_TOP:
		y = 0;
		break;

	case ALIGN_BOTTOM:
		//m_text_org_y=0
		if (height > y_size)
		{
			y = height - y_size;
		}
		break;

	default:
		ASSERT(0);
		break;
	}
}
