#ifndef _WAVE_CTRL_H_
#define _WAVE_CTRL_H_

typedef enum
{
	FILL_MODE,
	SCAN_MODE
}E_WAVE_DRAW_MODE;

typedef enum
{
	GAIN_025,
	GAIN_050,
	GAIN_100,
	GAIN_200
}E_WAVE_GAIN;

class c_wave_buffer;
class c_wave_ctrl : public c_wnd
{
public:
	c_wave_ctrl();
	virtual c_wnd* clone(){return new c_wave_ctrl();}
	virtual void on_init_children();
	virtual void on_paint();

	void set_wave_name(unsigned short wave_name_id){ m_wave_name = wave_name_id;}
	unsigned short get_wave_name(){return m_wave_name;}
	void set_wave_unit(unsigned short wave_unit_id){ m_wave_unit = wave_unit_id;}

	void set_wave_name_font_type(const GUI_FONT* wave_name_font_type){ m_wave_name_font_type = wave_name_font_type;}
	void set_wave_unit_font_type(const GUI_FONT* wave_unit_font_type){ m_wave_unit_font_type = wave_unit_font_type;}

	void set_wave_name_color(unsigned int wave_name_color){ m_wave_name_color = wave_name_color;}
	void set_wave_unit_color(unsigned int wave_unit_color){ m_wave_unit_color = wave_unit_color;}
	void set_wave_color(unsigned int color){ m_wave_color = color;}
	void set_wave_sample_rate(unsigned int rate);//speed = module sample rate
	void set_wave_speed(unsigned int speed);

	void set_max_min_base(short max_data, short min_data, short data_base);
	void set_wave_gain(E_WAVE_GAIN gain);
	void set_wave(c_wave_buffer* wave){m_wave = wave;}
	c_wave_buffer* get_wave(){return m_wave;}
	void clear_data();
	bool is_data_enough();
	void refresh_wave(unsigned char frame);
	void clear_wave();
	
protected:
	void draw_smooth_vline(int y_min, int y_max, int mid, unsigned int rgb);
	void erase_oldest_vline();
	void save_foreground();

	unsigned short m_wave_name;
	unsigned short m_wave_unit;

	const GUI_FONT* m_wave_name_font_type;
	const GUI_FONT* m_wave_unit_font_type;

	unsigned int m_wave_name_color;
	unsigned int m_wave_unit_color;

	unsigned int m_wave_color;
	unsigned int m_back_color;

	int m_wave_left;
	int m_wave_right;
	int m_wave_top;
	int m_wave_bottom;

	short m_max_data;
	short m_min_data;
	short m_pivot_data;					//for wave gain and fill mode
	E_WAVE_GAIN		m_gain;				//wave gain
	
private:
	c_wave_buffer*	m_wave;
	void*			m_bg_fb;			//background frame buffer, could be used to draw scale line.
	int 			m_wave_cursor;
	int 			m_wave_speed;		//pixels per refresh
	int				m_wave_sample_rate;
	unsigned char 	m_frame_len_map[64];
	unsigned char 	m_frame_len_map_index;
};
#endif
