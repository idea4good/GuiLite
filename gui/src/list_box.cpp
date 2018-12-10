#include "core_include/api.h"
#include "core_include/rect.h"
#include "core_include/cmd_target.h"
#include "core_include/resource.h"
#include "core_include/wnd.h"
#include "core_include/surface.h"
#include "core_include/bitmap.h"
#include "core_include/word.h"

#include "../gui_include/button.h"
#include "../gui_include/my_resource.h"
#include "../gui_include/list_box.h"
#include <string.h>

#define ITEM_HEIGHT				45

void c_list_box::pre_create_wnd()
{
	m_style = GL_ATTR_VISIBLE | GL_ATTR_FOCUS | ALIGN_HCENTER | ALIGN_VCENTER;
	memset(m_item_array, 0, sizeof(m_item_array));
	m_item_total = 0;
	m_font_color = c_my_resource::get_color(CTRL_FORE_GROUND);
}

void c_list_box::on_init_children()
{
	m_item_total = 0;
	m_selected_item = 0;	
	m_font_type = c_my_resource::get_font(FONT_DEFAULT);
	m_font_color = c_my_resource::get_color(CTRL_FORE_GROUND);
	m_bg_color = c_my_resource::get_color(CTRL_BACK_GROUND);
}

void c_list_box::on_focus()
{
	modify_status(STATUS_FOCUSED);
	on_paint();
}

void c_list_box::on_kill_focus()
{
	modify_status(STATUS_NORMAL);
	on_paint();
}

void c_list_box::on_paint()
{
	c_rect rect, empty_rect;
	get_screen_rect(rect);
	empty_rect.Empty();

	switch(m_status)
	{
	case STATUS_NORMAL:
		if (m_z_order > m_parent->get_z_order())
		{
			m_surface->set_frame_layer(empty_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
		}
		fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(BUTTON_NORMAL));
		break;
	case STATUS_FOCUSED:
		if (m_z_order > m_parent->get_z_order())
		{
			m_surface->set_frame_layer(empty_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
		}
		fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(BUTTON_FOCUS));
		break;
	case STATUS_PUSHED:
		fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(LIST_BOX_PUSH));
		c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], rect, m_font_type, GL_RGB(2, 124, 165), GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER | ALIGN_VCENTER);
		//draw list
		if (m_item_total > 0)
		{
			if (m_z_order == m_parent->get_z_order())
			{
				m_z_order++;
			}
			m_surface->set_frame_layer(m_list_screen_rect, m_z_order);
			show_list();
			return;
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER | ALIGN_VCENTER);

	if (m_item_total)
	{
		c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_style);
	}
}

void c_list_box::on_touch_down(int x, int y)
{
	if (m_wnd_rect.PtInRect(x, y) )
	{//click base
		if (STATUS_NORMAL == m_status)
		{
			get_parent()->set_focus(this);
		}
	}
	else if (m_list_wnd_rect.PtInRect(x, y))
	{//click extend list
		c_wnd::on_touch_down(x, y);
	}
	else
	{
		if (STATUS_PUSHED == m_status)
		{
			modify_status(STATUS_FOCUSED);
			on_paint();
			notify_parent(GL_LIST_CONFIRM, get_id(), 0);
		}        
	}
}

void c_list_box::on_touch_up(int x, int y)
{
	if (STATUS_FOCUSED == m_status)
	{
		modify_status(STATUS_PUSHED);
		on_paint();
		notify_parent(GL_LIST_SELECT, get_id(), 0);
	}
	else if (STATUS_PUSHED == m_status)
	{
		if (m_wnd_rect.PtInRect(x, y))
		{//click base
			modify_status(STATUS_FOCUSED);
			on_paint();
		}
		else if (m_list_wnd_rect.PtInRect(x, y))
		{//click extend list
			modify_status(STATUS_FOCUSED);
			select_item((y - m_list_wnd_rect.m_top) / ITEM_HEIGHT);
			on_paint();
			notify_parent(GL_LIST_CONFIRM, get_id(), 0);
		}
		else
		{
			c_wnd::on_touch_up(x, y);
		}
	}
}

void c_list_box::update_list_size()
{
	m_list_wnd_rect = m_wnd_rect;
	m_list_wnd_rect.m_top = m_wnd_rect.m_bottom + 1;
	m_list_wnd_rect.m_bottom = m_list_wnd_rect.m_top + m_item_total * ITEM_HEIGHT;

	get_screen_rect(m_list_screen_rect);
	m_list_screen_rect.m_top = m_list_screen_rect.m_bottom + 1;
	m_list_screen_rect.m_bottom = m_list_screen_rect.m_top + m_item_total * ITEM_HEIGHT;
}

void c_list_box::show_list()
{
	fill_rect_ex(m_list_screen_rect, m_bg_color, c_my_resource::get_shape(LIST_BOX_EXTEND));

	m_font_color = GL_RGB(255, 255, 255);
	//draw all items
	c_rect tmp_rect;
	for (int i = 0; i < m_item_total; i++)
	{
		tmp_rect.m_left = m_list_screen_rect.m_left;
		tmp_rect.m_right = m_list_screen_rect.m_right;
		tmp_rect.m_top = m_list_screen_rect.m_top + i * ITEM_HEIGHT;
		tmp_rect.m_bottom = tmp_rect.m_top + ITEM_HEIGHT;
		c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[i], tmp_rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER | ALIGN_VCENTER);
		draw_hline(tmp_rect.m_left, tmp_rect.m_right, tmp_rect.m_bottom, GL_RGB(99, 108, 124));
	}
	//draw selected item	
	tmp_rect.m_left = m_list_screen_rect.m_left;
	tmp_rect.m_right = m_list_screen_rect.m_right;
	tmp_rect.m_top = m_list_screen_rect.m_top + m_selected_item * ITEM_HEIGHT;
	tmp_rect.m_bottom = tmp_rect.m_top + ITEM_HEIGHT;

	fill_rect_ex(tmp_rect, GL_RGB(0, 255, 0), c_my_resource::get_shape(LIST_BOX_SELECT));

	m_font_color = GL_RGB(255, 255, 255);
	c_word::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_selected_item], tmp_rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER | ALIGN_VCENTER);
}

int c_list_box::add_item(char* str)
{
	if (m_item_total >= MAX_ITEM_NUM)
	{
		ASSERT(FALSE);
		return -1;
	}
	m_item_array[m_item_total++] = str;
	update_list_size();
	return 0;
}

void c_list_box::clear_item()
{
	m_selected_item = m_item_total = 0;
	memset(m_item_array, 0, sizeof(m_item_array));
	update_list_size();
}

void c_list_box::select_item(short index)
{
	if (index < 0 || index >= m_item_total)
	{
		ASSERT(FALSE);
	}
	m_selected_item = index;
}
