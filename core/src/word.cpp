#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/surface.h"
#include <string.h>
#include <stdio.h>

#define BUFFER_LEN	16
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
	if(NULL == s)
	{
		return;
	}
	if (NULL == font)
	{
		ASSERT(FALSE);
	}
	int x, y;
	get_string_pos(s, font, rect, align_type, x, y);
	draw_string(surface, z_order, s, rect.m_left + x, rect.m_top + y, font, font_color, bg_color, ALIGN_LEFT);
}

void c_word::draw_string(c_surface* surface, int z_order, const char *s, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
	if (NULL == s)
	{
		return;
	}
	if (NULL == font)
	{
		ASSERT(FALSE);
	}
	for (; *s; s++)
	{
		int line_len = get_char_cnt_in_single_line(s);
		int str_pixel_length = get_str_pixel_length(s, font);
		if (align_type == ALIGN_RIGHT)
		{
			x = x - str_pixel_length + 1;
		}
		else if (align_type == ALIGN_HCENTER)
		{
			x = x - str_pixel_length / 2;
		}
		draw_single_line_string(surface, z_order, s, x, y , line_len, font, font_color, bg_color);
		s += line_len;
		if (*s == '\n')
		{
			y += font->height;
		}
		if (*s==0)
			break;
	}
}

const int c_word::get_font_ysize(const FONT_INFO* font_type)
{
	if (!font_type)
	{
		return 0;
	}
	return font_type->height;
}

void c_word::value_2_string(int value, int dot_position, char* buf, int len)
{
	memset(buf, 0, len);
	if (XXX == value)
	{
		sprintf(buf, "%s", "---");
		return;
	}
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
		ASSERT(FALSE);
		break;
	}
}

int c_word::get_char_cnt_in_single_line(const char *s) 
{
	int len = 0;
	unsigned char code_high;
	while (((code_high = *(unsigned char*)s) != 0)) 
	{
		if (code_high > 127) 
		{
			len++; s++;
		} 
		else 
		{
			switch (code_high) 
			{
			case '\n': return len;
			}
		}
		len++;
		s++;
	}
	return len;
}

void c_word::draw_single_line_string(c_surface* surface, int z_order, const char *s, int x, int y, int len, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color)
{
	int offset = 0;
	while (--len >= 0) 
	{
		offset = draw_single_char(surface, z_order, *s++, x, y, font, font_color, bg_color);
		x += offset;
	}
}

const LATTICE* c_word::get_lattice(const FONT_INFO* font, unsigned int utf8_code)
{
	int first = 0;
	int last = font->count - 1;
	int middle = (first + last) / 2;

	while (first <= last) 
	{
		if (font->lattice[middle].utf8_code < utf8_code)
			first = middle + 1;
		else if (font->lattice[middle].utf8_code == utf8_code)
		{
			return &font->lattice[middle];
		}
		else
		{
			last = middle - 1;
		}
		middle = (first + last) / 2;
	}
	return NULL;
}

int c_word::draw_single_char(c_surface* surface, int z_order, unsigned int utf8_code, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color)
{
	if (NULL == font)
	{
		ASSERT(FALSE);
		return 0;
	}

	const LATTICE* p_lattice = get_lattice(font, utf8_code);
	if (p_lattice)
	{
		draw_lattice(surface, z_order, x, y, p_lattice->width, font->height,
			p_lattice->width, (unsigned char const *)p_lattice->p_data, font_color, bg_color);
		return p_lattice->width;
	}
	else
	{
		ASSERT(FALSE);
	}
	return 0;
}

void c_word::draw_lattice(c_surface* surface, int z_order, int x, int y, int width, int height,
						int bytes_per_line, const unsigned char* pData,
						unsigned int font_color, unsigned int bg_color)
{
	for (int i = 0; i < height; i++)
	{
		draw_bit_line_AA(surface, z_order, x, i + y, pData, width, font_color, bg_color);
		pData += bytes_per_line;
	}
}

void  c_word::draw_bit_line_AA(c_surface* surface, int z_order, int x, int y, unsigned char const*p, int width, unsigned int font_color, unsigned int bg_color)
{
	unsigned int color, current_bg_color;
	unsigned int b, g, r;

	if(COLOR_TRANPARENT == bg_color)
	{
		current_bg_color = surface->get_pixel(x, y, z_order);
	}
	else
	{
		current_bg_color = bg_color;
	}

	do 
	{
		if (0x00 == *p)
		{
			if(bg_color != COLOR_TRANPARENT)
			{
				surface->set_pixel(x, y, current_bg_color, z_order);
			}
		}
		else
		{
			b = (GLT_RGB_B(font_color) * (*p) + GLT_RGB_B(current_bg_color) * (255 - *p)) >> 8;
			g = (GLT_RGB_G(font_color) * (*p) + GLT_RGB_G(current_bg_color) * (255 - *p)) >> 8;
			r = (GLT_RGB_R(font_color) * (*p) + GLT_RGB_R(current_bg_color) * (255 - *p)) >> 8;
			color = GLT_RGB(r, g, b);
			surface->set_pixel(x, y, color, z_order);
		}
		x++;
		p++;
	} while (--width);
}

int c_word::get_str_pixel_length(const char *s, const FONT_INFO* font)
{
	int ret = 0;
	if(NULL == s)
	{
		return ret;
	}
	if (NULL == font)
	{
		ASSERT(FALSE);
	}

	for (; *s; s++)
	{
		int len = get_char_cnt_in_single_line(s);
		int lattice_width = 0;

		while (--len >= 0) 
		{
			const LATTICE* p_lattice = get_lattice(font, *s);
			if (p_lattice)
			{
				lattice_width = p_lattice->width;
			}
			ret += lattice_width;
			s++;
		}
		if (*s==0)
			break;
	}

	return ret;
}

void c_word::get_string_pos(const char *s, const FONT_INFO* font, c_rect rect, unsigned int align_type, int &x, int &y)
{
	int x_size = get_str_pixel_length(s, font);
	int y_size = font->height;

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
