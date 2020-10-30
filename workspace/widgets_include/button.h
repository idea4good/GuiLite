#pragma once

#include "../core_include/api.h"
#include "../core_include/wnd.h"
#include "../core_include/resource.h"
#include "../core_include/bitmap.h"
#include "../core_include/word.h"
#include "../core_include/display.h"
#include "../core_include/theme.h"

class c_button : public c_wnd
{
public:
	void set_on_click(WND_CALLBACK on_click) { this->on_click = on_click; }
protected:
	virtual void on_paint()
	{
		c_rect rect;
		get_screen_rect(rect);

		switch (m_status)
		{
		case STATUS_NORMAL:
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
			if (m_str)
			{
				c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
			}
			break;
		case STATUS_FOCUSED:
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
			if (m_str)
			{
				c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
			}
			break;
		case STATUS_PUSHED:
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_PUSHED), m_z_order);
			m_surface->draw_rect(rect, c_theme::get_color(COLOR_WND_BORDER), 2, m_z_order);
			if (m_str)
			{
				c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_PUSHED), ALIGN_HCENTER | ALIGN_VCENTER);
			}
			break;
		default:
			ASSERT(false);
			break;
		}
	}
	virtual void on_focus()
	{
		m_status = STATUS_FOCUSED;
		on_paint();
	}
	virtual void on_kill_focus()
	{
		m_status = STATUS_NORMAL;
		on_paint();
	}
	virtual void pre_create_wnd()
	{
		on_click = 0;
		m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
		m_font_type = c_theme::get_font(FONT_DEFAULT);
		m_font_color = c_theme::get_color(COLOR_WND_FONT);
	}

	virtual void on_touch(int x, int y, TOUCH_ACTION action)
	{
		if (action == TOUCH_DOWN)
		{
			m_parent->set_child_focus(this);
			m_status = STATUS_PUSHED;
			on_paint();
		}
		else
		{
			m_status = STATUS_FOCUSED;
			on_paint();
			if(on_click)
			{
				(m_parent->*(on_click))(m_id, 0);
			}
		}
	}
	virtual void on_navigate(NAVIGATION_KEY key)
	{
		switch (key)
		{
		case NAV_ENTER:
			on_touch(m_wnd_rect.m_left, m_wnd_rect.m_top, TOUCH_DOWN);
			on_touch(m_wnd_rect.m_left, m_wnd_rect.m_top, TOUCH_UP);
			break;
		case NAV_FORWARD:
		case NAV_BACKWARD:
			break;
		}
		return c_wnd::on_navigate(key);
	}
	WND_CALLBACK on_click;
};
