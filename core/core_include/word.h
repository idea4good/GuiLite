#ifndef _WORD_UNIT_H_
#define _WORD_UNIT_H_

class c_surface;
class c_word
{
public:
	static void draw_string(c_surface* surface, int z_order, const char *s, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void draw_string_in_rect(c_surface* surface, int z_order, const char *s, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void draw_value(c_surface* surface, int z_order, int value, int dot_position, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void draw_value_in_rect(c_surface* surface, int z_order, int value, int dot_position, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	static void value_2_string(int value, int dot_position, char* buf, int len);

	static int get_str_pixel_length(const char *s, const FONT_INFO* font);
	static const int get_font_ysize(const FONT_INFO* font_type);
private:
	static int get_char_cnt_in_single_line(const char *s);
	static void draw_single_line_string(c_surface* surface, int z_order, const char *s, int x, int y, int len, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color);
	static int draw_single_char(c_surface* surface, int z_order, unsigned int utf8_code, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color);
	static void draw_lattice(c_surface* surface, int z_order, int x, int y, int width, int height, int bytes_per_line, const unsigned char* pData, unsigned int font_color, unsigned int bg_color);
	static void draw_bit_line_AA(c_surface* surface, int z_order, int x, int y, unsigned char const*p, int width, unsigned int font_color, unsigned int bg_color);

	static const LATTICE* get_lattice(const FONT_INFO* font, unsigned int utf8_code);
	static void get_string_pos(const char *s, const FONT_INFO* font, c_rect rect, unsigned int align_type, int &x, int &y);
private:
	c_word(){};
	c_word(const c_word&);
	c_word& operator=(const c_word&);
	~c_word(){}
};

#endif
