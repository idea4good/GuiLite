#ifndef _WORD_UNIT_H_
#define _WORD_UNIT_H_

#define COLOR_TRANPARENT 0xFF000000
#define BUFFER_LEN	16
#define MAX_TASK	6

class c_surface;
class c_word
{
public:
	static c_word* get_instance(unsigned int z_order, c_surface* gal);
	static void initiallize(E_LANGUAGE language);

	void draw_string(const char *s, int x, int y, unsigned int align_type = ALIGN_LEFT);
	void draw_string(unsigned long str_id, int x, int y, unsigned int align_type = ALIGN_LEFT);
	void draw_string_in_rect(const char *s, c_rect rect, unsigned int align_type = ALIGN_LEFT);
	void draw_string_in_rect(unsigned long str_id, c_rect rect, unsigned int align_type = ALIGN_LEFT);
	void draw_value(int value, int dot_position, int x, int y, unsigned int align_type = ALIGN_LEFT);
	void draw_value_in_rect(int value, int dot_position, c_rect rect, unsigned int align_type = ALIGN_LEFT);

	void set_font(const GUI_FONT* font_type);
	void set_color(const unsigned int font_color, const unsigned int back_color, const unsigned int is_transparent_aa = 1);
	int get_str_pixel_length(const char *s);
	const char* get_string(unsigned long str_id);
	const char* get_string(int value, int dot_position);
	const int get_font_ysize(const GUI_FONT* font_type);
	void set_z_oder(int z_order){m_z_order = z_order;}
	void set_surface(c_surface* surface){m_surface = surface;}

private:
	int get_char_cnt_in_single_line(const char *s);
	void value_2_string(int value, int dot_position);
	void draw_single_line_string(const char *s, int x, int y, int len);
	bool is_AA_font(const GUI_CHARINFO* pCharInfo);
	const GUI_FONT_PROP* find_char(const GUI_FONT_PROP* pProp, unsigned short code);
	int draw_single_char(unsigned short code, int x, int y );
	void draw_lattice(int x, int y, int width, int height, int bytes_per_line, const unsigned char* pData, unsigned int font_aa_flag);
	void draw_bit_line(int x, int y, unsigned char const*p, int width );
	void draw_bit_line_AA(int x, int y, unsigned char const*p, int width);
	void get_string_pos(const char *s, c_rect rect, unsigned int align_type, int &x, int &y);

	unsigned int	m_task_id;
	const GUI_FONT *m_pfont;
	unsigned int	m_font_color, m_back_color, m_is_transparent;
	char			m_buff[BUFFER_LEN];
	c_rect			m_rect;
	int				m_z_order;
	c_surface*		m_surface;
	static const MULTI_LAN_STRINGS *m_lang_map;
	static E_LANGUAGE m_lang_type;
	static c_word s_wordunit_array[MAX_TASK];
private:
	c_word();
	c_word(const c_word&);
	c_word& operator=(const c_word&);
	~c_word(){}
};

#endif
