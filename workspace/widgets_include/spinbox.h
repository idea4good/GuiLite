#pragma once

#include "../core_include/api.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/display.h"
#include "../core_include/theme.h"
#include "../widgets_include/button.h"

#define ID_BT_ARROW_UP      	0x1111
#define ID_BT_ARROW_DOWN    	0x2222

class c_spin_box;
class c_spin_button : public c_button
{
	friend class c_spin_box;
	inline virtual void on_touch(int x, int y, TOUCH_ACTION action);
	c_spin_box* m_spin_box;
};

class c_spin_box : public c_wnd
{
	friend class c_spin_button;
public:
	short get_value() { return m_value; }
	void set_value(unsigned short value) { m_value = m_cur_value = value; }
	void set_max_min(short max, short min) { m_max = max; m_min = min; }
	void set_step(short step) { m_step = step; }
	short get_min() { return m_min; }
	short get_max() { return m_max; }
	short get_step() { return m_step; }
	void set_value_digit(short digit) { m_digit = digit; }
	short get_value_digit() { return m_digit; }
	void set_on_change(WND_CALLBACK on_change) { this->on_change = on_change; }
protected:
	virtual void on_paint()
	{
		c_rect rect;
		get_screen_rect(rect);
		rect.m_right = rect.m_left + (rect.width() * 2 / 3);

		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
		c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
	}
	virtual void pre_create_wnd()
	{
		m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE);
		m_font_type = c_theme::get_font(FONT_DEFAULT);
		m_font_color = c_theme::get_color(COLOR_WND_FONT);
		m_max = 6;
		m_min = 1;
		m_digit = 0;
		m_step = 1;

		//link arrow button position.
		c_rect rect;
		get_wnd_rect(rect);
		m_bt_down.m_spin_box = m_bt_up.m_spin_box = this;
		m_bt_up.connect(m_parent, ID_BT_ARROW_UP, "+", (rect.m_left + rect.width() * 2 / 3), rect.m_top, (rect.width() / 3), (rect.height() / 2));
		m_bt_down.connect(m_parent, ID_BT_ARROW_DOWN, "-", (rect.m_left + rect.width() * 2 / 3), (rect.m_top + rect.height() / 2), (rect.width() / 3), (rect.height() / 2));
	}
	void on_arrow_up_bt_click()
	{
		if (m_cur_value + m_step > m_max)
		{
			return;
		}
		m_cur_value += m_step;
		if(on_change)
		{
			(m_parent->*(on_change))(m_id, m_cur_value);
		}
		on_paint();
	}
	void on_arrow_down_bt_click()
	{
		if (m_cur_value - m_step < m_min)
		{
			return;
		}
		m_cur_value -= m_step;
		if(on_change)
		{
			(m_parent->*(on_change))(m_id, m_cur_value);
		}
		on_paint();
	}

	short			m_cur_value;
	short			m_value;
	short			m_step;
	short			m_max;
	short			m_min;
	short			m_digit;
	c_spin_button  	m_bt_up;
	c_spin_button  	m_bt_down;
	WND_CALLBACK 	on_change;
};

inline void c_spin_button::on_touch(int x, int y, TOUCH_ACTION action)
{
	if (action == TOUCH_UP)
	{
		(m_id == ID_BT_ARROW_UP) ? m_spin_box->on_arrow_up_bt_click() : m_spin_box->on_arrow_down_bt_click();
	}
	c_button::on_touch(x, y, action);
}
