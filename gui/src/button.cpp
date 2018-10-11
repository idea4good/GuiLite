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
#include "../gui_include/dialog.h"

void c_button::pre_create_wnd()
{
	m_style |= GLT_ATTR_VISIBLE | GLT_ATTR_FOCUS | ALIGN_HCENTER | ALIGN_VCENTER;
	m_font_type = c_my_resource::get_font(FONT_ENG_SMB);
	m_font_color = c_my_resource::get_color(CTRL_FORE_GROUND);
	m_bg_color = c_my_resource::get_color(CTRL_BACK_GROUND);
}

void c_button::on_focus()
{
	modify_status(STATUS_FOCUSED);
	on_paint();
}

void c_button::on_kill_focus()
{
	modify_status(STATUS_NORMAL);
	on_paint();
}

void c_button::on_touch_down(int x, int y)
{
	get_parent()->set_focus(this);
	modify_status(STATUS_PUSHED);
	on_paint();
}

void c_button::on_touch_up(int x, int y)
{
	if (STATUS_PUSHED == m_status)
	{
		modify_status(STATUS_FOCUSED);
		on_paint();

		notify_parent(GLT_BN_CLICKED, get_id(), 0);
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
			fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(BUTTON_NORMAL));
		}
		break;
	case STATUS_FOCUSED:
		if (m_bitmap_focus)
		{
			c_bitmap::draw_bitmap_in_rect(m_surface, m_z_order, m_bitmap_focus, rect, m_style);
		}
		else
		{
			fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(BUTTON_FOCUS));
		}
		break;
	case STATUS_PUSHED:
		if (m_bitmap_pushed)
		{
			c_bitmap::draw_bitmap_in_rect(m_surface, m_z_order, m_bitmap_pushed, rect, m_style);
		}
		else
		{
			fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(BUTTON_PUSH));
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
