#ifndef WORD_UNIT_H
#define WORD_UNIT_H

class c_surface;
class c_word
{
public:
	static void draw_string(c_surface* surface, int z_order, const char *s, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void draw_string_in_rect(c_surface* surface, int z_order, const char *s, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void draw_value(c_surface* surface, int z_order, int value, int dot_position, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void draw_value_in_rect(c_surface* surface, int z_order, int value, int dot_position, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void value_2_string(int value, int dot_position, char* buf, int len);

	static int get_str_size(const char *s, const FONT_INFO* font, int& width, int& height);
private:
	static int draw_single_char(c_surface* surface, int z_order, unsigned int utf8_code, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color);
	static void draw_lattice(c_surface* surface, int z_order, int x, int y, int width, int height, const unsigned char* p_data, unsigned int font_color, unsigned int bg_color);
	
	static const LATTICE* get_lattice(const FONT_INFO* font, unsigned int utf8_code);
	static void get_string_pos(const char *s, const FONT_INFO* font, c_rect rect, unsigned int align_type, int &x, int &y);
};

#endif
