#pragma once

#include "../core_include/api.h"
#include "../core_include/wnd.h"
#include "../core_include/display.h"
#include "../core_include/resource.h"
#include "../core_include/bitmap.h"
#include "../core_include/theme.h"
#include "../core_include/word.h"

class c_label : public c_wnd
{
public:
	virtual void on_paint()
	{
		c_rect rect;
		unsigned int bg_color = m_bg_color ? m_bg_color : m_parent->get_bg_color();
		get_screen_rect(rect);
		if (m_str)
		{
			m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, bg_color, m_z_order);
			c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, bg_color, ALIGN_LEFT | ALIGN_VCENTER);
		}
	}
protected:
	virtual void pre_create_wnd()
	{
		m_attr = ATTR_VISIBLE;
		m_font_color = c_theme::get_color(COLOR_WND_FONT);
		m_font_type = c_theme::get_font(FONT_DEFAULT);
	}
};
