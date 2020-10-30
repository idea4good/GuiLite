#pragma once

#include "../core_include/api.h"
#include "../core_include/resource.h"
#include "../core_include/wnd.h"
#include "../core_include/display.h"
#include "../core_include/bitmap.h"
#include "../core_include/word.h"
#include "../core_include/theme.h"
#include "../widgets_include/button.h"
#include <string.h>

#define MAX_ITEM_NUM			4
#define ITEM_HEIGHT				45

class c_list_box : public c_wnd
{
public:
	void set_on_change(WND_CALLBACK on_change) { this->on_change = on_change; }
	short get_item_count() { return m_item_total; }

	int add_item(char* str)
	{
		if (m_item_total >= MAX_ITEM_NUM)
		{
			ASSERT(false);
			return -1;
		}
		m_item_array[m_item_total++] = str;
		update_list_size();
		return 0;
	}
	void clear_item()
	{
		m_selected_item = m_item_total = 0;
		memset(m_item_array, 0, sizeof(m_item_array));
		update_list_size();
	}
	void  select_item(short index)
	{
		if (index < 0 || index >= m_item_total)
		{
			ASSERT(false);
		}
		m_selected_item = index;
	}
	
protected:
	virtual void pre_create_wnd()
	{
		m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
		memset(m_item_array, 0, sizeof(m_item_array));
		m_item_total = 0;
		m_selected_item = 0;
		m_font_type = c_theme::get_font(FONT_DEFAULT);
		m_font_color = c_theme::get_color(COLOR_WND_FONT);
	}

	virtual void on_paint()
	{
		c_rect rect;
		get_screen_rect(rect);

		switch (m_status)
		{
		case STATUS_NORMAL:
			if (m_z_order > m_parent->get_z_order())
			{
				m_z_order = m_parent->get_z_order();
				m_surface->show_layer(m_list_screen_rect, m_z_order);
				m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
			}
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
			c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
			break;
		case STATUS_FOCUSED:
			if (m_z_order > m_parent->get_z_order())
			{
				m_z_order = m_parent->get_z_order();
				m_surface->show_layer(m_list_screen_rect, m_z_order);
				m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
			}
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
			c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
			break;
		case STATUS_PUSHED:
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_PUSHED), m_z_order);
			m_surface->draw_rect(rect, c_theme::get_color(COLOR_WND_BORDER), 2, m_z_order);
			c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], rect, m_font_type, GL_RGB(2, 124, 165), GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER | ALIGN_VCENTER);
			//draw list
			if (m_item_total > 0)
			{
				if (m_z_order == m_parent->get_z_order())
				{
					m_z_order++;
				}
				m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_PRIORITY);
				show_list();
			}
			break;
		default:
			ASSERT(false);
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
	virtual void on_navigate(NAVIGATION_KEY key)
	{
		switch (key)
		{
		case NAV_ENTER:
			on_touch(m_wnd_rect.m_left, m_wnd_rect.m_top, TOUCH_DOWN);
			on_touch(m_wnd_rect.m_left, m_wnd_rect.m_top, TOUCH_UP);
			return;
		case NAV_BACKWARD:
			if (m_status != STATUS_PUSHED)
			{
				return c_wnd::on_navigate(key);
			}
			m_selected_item = (m_selected_item > 0) ? (m_selected_item - 1) : m_selected_item;
			return show_list();
		case NAV_FORWARD:
			if (m_status != STATUS_PUSHED)
			{
				return c_wnd::on_navigate(key);
			}
			m_selected_item = (m_selected_item < (m_item_total - 1)) ? (m_selected_item + 1) : m_selected_item;
			return show_list();
		}
	}
	virtual void on_touch(int x, int y, TOUCH_ACTION action)
	{
		(action == TOUCH_DOWN) ? on_touch_down(x, y) : on_touch_up(x, y);
	}
	
private:
	void update_list_size()
	{
		m_list_wnd_rect = m_wnd_rect;
		m_list_wnd_rect.m_top = m_wnd_rect.m_bottom + 1;
		m_list_wnd_rect.m_bottom = m_list_wnd_rect.m_top + m_item_total * ITEM_HEIGHT;

		get_screen_rect(m_list_screen_rect);
		m_list_screen_rect.m_top = m_list_screen_rect.m_bottom + 1;
		m_list_screen_rect.m_bottom = m_list_screen_rect.m_top + m_item_total * ITEM_HEIGHT;
	}
	void show_list()
	{
		//draw all items
		c_rect tmp_rect;
		for (int i = 0; i < m_item_total; i++)
		{
			tmp_rect.m_left = m_list_screen_rect.m_left;
			tmp_rect.m_right = m_list_screen_rect.m_right;
			tmp_rect.m_top = m_list_screen_rect.m_top + i * ITEM_HEIGHT;
			tmp_rect.m_bottom = tmp_rect.m_top + ITEM_HEIGHT;

			if (m_selected_item == i)
			{
				m_surface->fill_rect(tmp_rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
				c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[i], tmp_rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
			}
			else
			{
				m_surface->fill_rect(tmp_rect, GL_RGB(17, 17, 17), m_z_order);
				c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[i], tmp_rect, m_font_type, m_font_color, GL_RGB(17, 17, 17), ALIGN_HCENTER | ALIGN_VCENTER);
			}
		}
	}
	void on_touch_down(int x, int y)
	{
		if (m_wnd_rect.pt_in_rect(x, y))
		{//click base
			if (STATUS_NORMAL == m_status)
			{
				m_parent->set_child_focus(this);
			}
		}
		else if (m_list_wnd_rect.pt_in_rect(x, y))
		{//click extend list
			c_wnd::on_touch(x, y, TOUCH_DOWN);
		}
		else
		{
			if (STATUS_PUSHED == m_status)
			{
				m_status = STATUS_FOCUSED;
				on_paint();
				if(on_change)
				{
					(m_parent->*(on_change))(m_id, m_selected_item);
				}
			}
		}
	}
	void on_touch_up(int x, int y)
	{
		if (STATUS_FOCUSED == m_status)
		{
			m_status = STATUS_PUSHED;
			on_paint();
		}
		else if (STATUS_PUSHED == m_status)
		{
			if (m_wnd_rect.pt_in_rect(x, y))
			{//click base
				m_status = STATUS_FOCUSED;
				on_paint();
			}
			else if (m_list_wnd_rect.pt_in_rect(x, y))
			{//click extend list
				m_status = STATUS_FOCUSED;
				select_item((y - m_list_wnd_rect.m_top) / ITEM_HEIGHT);
				on_paint();
				if(on_change)
				{
					(m_parent->*(on_change))(m_id, m_selected_item);
				}
			}
			else
			{
				c_wnd::on_touch(x, y, TOUCH_UP);
			}
		}
	}

	short			m_selected_item;
	short			m_item_total;
	char*			m_item_array[MAX_ITEM_NUM];
	c_rect			m_list_wnd_rect;	//rect relative to parent wnd.
	c_rect			m_list_screen_rect;	//rect relative to physical screen(frame buffer)
	WND_CALLBACK 	on_change;
};
