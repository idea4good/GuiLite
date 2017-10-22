#ifndef _WORD_UNIT_H_
#define _WORD_UNIT_H_

#define COLOR_TRANPARENT 0xFF000000

class c_surface;
class c_word
{
public:
	static c_word* get_instance()
	{
		static c_word the_word;
		return &the_word;
	}
	static void initiallize(LANGUAGE_TYPE language);

	void draw_string(c_surface* surface, int z_order, const char *s, int x, int y, const GUI_FONT* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	void draw_string(c_surface* surface, int z_order, unsigned long str_id, int x, int y, const GUI_FONT* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	void draw_string_in_rect(c_surface* surface, int z_order, const char *s, c_rect rect, const GUI_FONT* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	void draw_string_in_rect(c_surface* surface, int z_order, unsigned long str_id, c_rect rect, const GUI_FONT* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	void draw_value(c_surface* surface, int z_order, int value, int dot_position, int x, int y, const GUI_FONT* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
	void draw_value_in_rect(c_surface* surface, int z_order, int value, int dot_position, c_rect rect, const GUI_FONT* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);

	int get_str_pixel_length(const char *s, const GUI_FONT* font);
	const int get_font_ysize(const GUI_FONT* font_type);
	void value_2_string(int value, int dot_position, char* buf);
	const char* get_string(unsigned long str_id);

private:
	int get_char_cnt_in_single_line(const char *s);
	void draw_single_line_string(c_surface* surface, int z_order, const char *s, int x, int y, int len, const GUI_FONT* font, unsigned int font_color, unsigned int bg_color);
	int draw_single_char(c_surface* surface, int z_order, unsigned short code, int x, int y, const GUI_FONT* font, unsigned int font_color, unsigned int bg_color);
	void draw_lattice(c_surface* surface, int z_order, int x, int y, int width, int height, int bytes_per_line, const unsigned char* pData, unsigned int font_color, unsigned int bg_color, unsigned int font_aa_flag);
	void draw_bit_line(c_surface* surface, int z_order, int x, int y, unsigned char const*p, int width, unsigned int font_color, unsigned int bg_color);
	void draw_bit_line_AA(c_surface* surface, int z_order, int x, int y, unsigned char const*p, int width, unsigned int font_color, unsigned int bg_color);

	const GUI_FONT_PROP* find_font_prop(const GUI_FONT_PROP* pProp, unsigned short code);
	void get_string_pos(const char *s, const GUI_FONT* font, c_rect rect, unsigned int align_type, int &x, int &y);
	bool is_AA_font(const GUI_CHARINFO* pCharInfo);

	static const MULTI_LAN_STRINGS *m_lang_map;
	static LANGUAGE_TYPE m_lang_type;
private:
	c_word(){};
	c_word(const c_word&);
	c_word& operator=(const c_word&);
	~c_word(){}
};

#endif
