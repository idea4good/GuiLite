#include "core_include/api.h"
#include "core_include/rect.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "core_include/resource.h"
#include "core_include/bitmap.h"
#include "core_include/word.h"
#include "core_include/surface.h"
#include "../gui_include/my_resource.h"
#include "../gui_include/button.h"

void c_button::pre_create_wnd()
{
	m_style = GL_ATTR_VISIBLE | GL_ATTR_FOCUS | ALIGN_HCENTER | ALIGN_VCENTER;
	m_font_type = c_my_resource::get_font(FONT_DEFAULT);
	m_font_color = c_my_resource::get_color(COLOR_WND_FONT);
}

void c_button::on_focus()
{
	m_status = STATUS_FOCUSED;
	on_paint();
}

void c_button::on_kill_focus()
{
	m_status = STATUS_NORMAL;
	on_paint();
}

void c_button::on_touch_down(int x, int y)
{
	get_parent()->set_focus(this);
	m_status = STATUS_PUSHED;
	on_paint();
}

void c_button::on_touch_up(int x, int y)
{
	if (STATUS_PUSHED == m_status)
	{
		m_status = STATUS_FOCUSED;
		on_paint();

		notify_parent(GL_BN_CLICKED, get_id(), 0);
	}
}

void c_button::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);

	switch(m_status)
	{
	case STATUS_NORMAL:
		if (m_bitmap_normal)
		{
			c_bitmap::draw_bitmap_in_rect(m_surface, m_z_order, m_bitmap_normal, rect, m_style);
		}
		else
		{
			fill_rect(rect, c_my_resource::get_color(COLOR_WND_NORMAL));
		}
		break;
	case STATUS_FOCUSED:
		if (m_bitmap_focus)
		{
			c_bitmap::draw_bitmap_in_rect(m_surface, m_z_order, m_bitmap_focus, rect, m_style);
		}
		else
		{
			fill_rect(rect, c_my_resource::get_color(COLOR_WND_FOCUS));
		}
		break;
	case STATUS_PUSHED:
		if (m_bitmap_pushed)
		{
			c_bitmap::draw_bitmap_in_rect(m_surface, m_z_order, m_bitmap_pushed, rect, m_style);
		}
		else
		{
			fill_rect(rect, c_my_resource::get_color(COLOR_WND_PUSHED));
			draw_rect(rect, c_my_resource::get_color(COLOR_WND_BORDER), 2);
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	if (m_str)
	{
		c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_style);
	}
}
