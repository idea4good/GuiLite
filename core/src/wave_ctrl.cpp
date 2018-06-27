#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/wave_buffer.h"
#include "../core_include/surface.h"
#include "../core_include/resource_type.h"
#include "../core_include/word.h"
#include "../core_include/wave_ctrl.h"
#include <stdlib.h>
#include <string.h>

#define CORRECT(x, high_limit, low_limit)	{\
	x = (x > high_limit) ? high_limit : x;\
	x = (x < low_limit) ? low_limit : x;\
}while(0)

#define WAVE_CURSOR_WIDTH		8
#define DATA_THRESHOLD			4
#define	WAVE_LINE_WIDTH			1

c_wave_ctrl::c_wave_ctrl()
{
	m_wave = NULL;
	m_bg_fb = NULL;
	m_wave_name_font_type  = m_wave_unit_font_type = NULL;
	m_wave_name = m_wave_unit = 0;
	m_max_data = 500;
	m_min_data = 0;
	m_pivot_data  = 250;
	m_wave_speed = 4;
	m_wave_sample_rate = 0;
	m_gain = ZOOM_100;
	m_frame_len_map_index = 0;

	m_wave_name_color  = m_wave_unit_color = m_wave_color = GLT_RGB(255,0,0);
	m_back_color = GLT_RGB(0,0,0);
}

void c_wave_ctrl::on_init_children()
{
	c_rect rect;
	get_screen_rect(rect);

	m_wave_left 	= rect.m_left + 22;
	m_wave_right	= rect.m_right;
	m_wave_top		= rect.m_top + 2;
	m_wave_bottom	= rect.m_bottom - 4;
	m_wave_cursor	= m_wave_left;

	m_bg_fb = calloc(rect.Width() * rect.Height(), 2);
}

void c_wave_ctrl::set_max_min_base(short max_data, short min_data, short data_base)
{
	m_max_data = max_data;
	m_min_data = min_data;
	m_pivot_data = data_base;
}

void c_wave_ctrl::set_wave_gain(E_WAVE_GAIN gain)
{
	m_gain = gain;
	on_paint();
}

void c_wave_ctrl::set_wave_sample_rate(unsigned int rate)
{
	if(rate > 500)
	{
		ASSERT(FALSE);
	}
	m_wave_sample_rate = rate;
	int read_times_per_second = m_wave_speed * 1000 / REAL_TIME_TASK_CYCLE_MS;

	memset(m_frame_len_map, 0, sizeof(m_frame_len_map));
	for (unsigned int i = 1; i < sizeof(m_frame_len_map) + 1; i++)
	{
		m_frame_len_map[i-1] = rate * i / read_times_per_second - rate * (i-1) / read_times_per_second;
	}
	m_frame_len_map_index = 0;
}

void c_wave_ctrl::set_wave_speed(unsigned int speed)
{
	switch(speed)
	{
	case 1:
	case 2:
	case 4:
	case 8:
		m_wave_speed = speed;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	set_wave_sample_rate(m_wave_sample_rate);
}

void c_wave_ctrl::clear_data()
{
	if(m_wave == NULL)
	{
		ASSERT(FALSE);
		return;
	}
	m_wave->clear_data();
}

bool c_wave_ctrl::is_data_enough()
{
	if(m_wave == NULL)
	{
		ASSERT(FALSE);
		return false;
	}
	return ((m_frame_len_map[m_frame_len_map_index] * m_wave_speed - m_wave->get_cnt()) <
			DATA_THRESHOLD)?true:false;
}

void c_wave_ctrl::refresh_wave(unsigned char frame)
{
	if(m_wave == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	short max, min, mid, speed;
	speed = m_wave_speed;
	while(--speed >= 0)
	{
		//get wave value
		mid = m_wave->read_wave_data_by_frame(max, min,
							m_frame_len_map[m_frame_len_map_index++],
							(frame | (speed << 8) | (((unsigned long)this & 0xffff) << 16)));
		m_frame_len_map_index %= sizeof(m_frame_len_map);
		//gain
		switch(m_gain)
		{
		case ZOOM_025:
			mid = ((mid - m_pivot_data) >> 2) + m_pivot_data;
			max = ((max - m_pivot_data) >> 2) + m_pivot_data;
			min = ((min - m_pivot_data) >> 2) + m_pivot_data;
			break;
		case ZOOM_050:
			mid = ((mid - m_pivot_data) >> 1) + m_pivot_data;
			max = ((max - m_pivot_data) >> 1) + m_pivot_data;
			min = ((min - m_pivot_data) >> 1) + m_pivot_data;
			break;
		case ZOOM_200:
			mid = ((mid - m_pivot_data) << 1) + m_pivot_data;
			max = ((max - m_pivot_data) << 1) + m_pivot_data;
			min = ((min - m_pivot_data) << 1) + m_pivot_data;
			break;
		case ZOOM_100:
		default:
			break;
		}
		//map to wave ctrl
		int y_min,y_max;
		if(m_max_data == m_min_data)
		{
			ASSERT(FALSE);
		}
		y_max = WAVE_LINE_WIDTH + m_wave_bottom - (m_wave_bottom - m_wave_top)*(min - m_min_data)/(m_max_data - m_min_data);
		y_min = (-WAVE_LINE_WIDTH) + m_wave_bottom - (m_wave_bottom - m_wave_top)*(max - m_min_data)/(m_max_data - m_min_data);
		mid = m_wave_bottom - (m_wave_bottom - m_wave_top)*(mid - m_min_data)/(m_max_data - m_min_data);

		CORRECT(y_min, m_wave_bottom, m_wave_top);
		CORRECT(y_max, m_wave_bottom, m_wave_top);
		CORRECT(mid, m_wave_bottom, m_wave_top);

		draw_smooth_vline(y_min, y_max, mid, m_wave_color);
		erase_oldest_vline();
		//ring the wave
		if ((m_wave_cursor + 1) > m_wave_right)
		{
			m_wave_cursor = m_wave_left ;
		}
		else
		{
			m_wave_cursor++;
		}
	}
}

void c_wave_ctrl::draw_smooth_vline(int y_min, int y_max, int mid, unsigned int rgb)
{
	int dy = y_max - y_min;
	short r = GLT_RGB_R(rgb);
	short g = GLT_RGB_G(rgb);
	short b = GLT_RGB_B(rgb);
	int  index = dy / 2 + 2;
	int  y;

	set_pixel(m_wave_cursor, mid, rgb);

	if (dy < 1)
	{
		return;
	}

	unsigned char cur_r,cur_g,cur_b;
	unsigned int cur_rgb;
	for (int i = 1; i <= dy/2 + 1; ++i )
	{
		if ( (mid + i) <= y_max )
		{
			y = mid + i;
			cur_r = r*(index - i)/index;
			cur_g = g*(index - i)/index;
			cur_b = b*(index - i)/index;
			cur_rgb = GLT_RGB(cur_r, cur_g, cur_b);
			set_pixel(m_wave_cursor, y, cur_rgb);
		}
		if ( (mid - i) >= y_min )
		{
			y = mid - i;
			cur_r = r*(index - i)/index;
			cur_g = g*(index - i)/index;
			cur_b = b*(index - i)/index;
			cur_rgb = GLT_RGB(cur_r, cur_g, cur_b);
			set_pixel(m_wave_cursor, y, cur_rgb);
		}
	}
}

void c_wave_ctrl::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	
	fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, m_back_color);
	
	//show name
	c_word::draw_string(m_surface, m_z_order, m_wave_name, m_wave_left + 10, rect.m_top, m_wave_name_font_type, m_wave_name_color, COLOR_TRANPARENT, ALIGN_LEFT);
	//show unit
	c_word::draw_string(m_surface, m_z_order, m_wave_unit, m_wave_left + 60, rect.m_top, m_wave_unit_font_type, m_wave_unit_color, COLOR_TRANPARENT, ALIGN_LEFT);

	save_foreground();
}

void c_wave_ctrl::clear_wave(void)
{
	if (!m_is_visible_now)
	{
		return;
	}
	fill_rect(m_wave_left, m_wave_top, m_wave_right, m_wave_bottom, m_back_color);
	m_wave_cursor = m_wave_left;
}

void c_wave_ctrl::erase_oldest_vline()
{
	//earse oldest vline = draw background on foreground.
	int  x = m_wave_cursor + WAVE_CURSOR_WIDTH;
	if (x > m_wave_right)
	{
		x -= (m_wave_right - m_wave_left + 1);
	}

	c_rect rect;
	get_screen_rect(rect);
	register int width = rect.Width();
	register int top = rect.m_top;
	register int left = rect.m_left;
	unsigned short* p_fb = (unsigned short*)m_bg_fb;

	for (int y_pos = (m_wave_top - 1); y_pos <= (m_wave_bottom + 1); y_pos++)
	{
		set_pixel(x, y_pos, p_fb[(y_pos - top) * width + (x - left)]);
	}
}

void c_wave_ctrl::save_foreground()
{
	c_rect rect;
	get_screen_rect(rect);
	//copy foreground to background
	register unsigned short* p_des = (unsigned short*)m_bg_fb;
	for (int y = rect.m_top; y <= rect.m_bottom; y++)
	{
		for (int x = rect.m_left; x <= rect.m_right; x++)
		{
			*p_des++ = m_surface->get_pixel(x, y, m_z_order);
		}
	}
}
