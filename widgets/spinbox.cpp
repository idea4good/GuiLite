#include "core_include/api.h"
#include "core_include/rect.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "core_include/resource.h"
#include "core_include/word.h"
#include "core_include/surface.h"
#include "core_include/theme.h"
#include "../widgets_include/button.h"
#include "../widgets_include/spinbox.h"

#define ARROW_BT_HEIGHT		55
#define ID_BT_ARROW_UP      1
#define ID_BT_ARROW_DOWN    2

GL_BEGIN_MESSAGE_MAP(c_spin_box)
ON_GL_BN_CLICKED(ID_BT_ARROW_UP, c_spin_box::on_arrow_up_bt_click)
ON_GL_BN_CLICKED(ID_BT_ARROW_DOWN, c_spin_box::on_arrow_down_bt_click)
GL_END_MESSAGE_MAP()

void c_spin_box::pre_create_wnd()
{
	m_style = GL_ATTR_VISIBLE | GL_ATTR_FOCUS | ALIGN_HCENTER | ALIGN_VCENTER;
	m_font_type = c_theme::get_font(FONT_DEFAULT);
	m_font_color = c_theme::get_color(COLOR_WND_FONT);

	m_max = 6;
	m_min = 1;
	m_digit = 0;
	m_step = 1;

	//set arrow button position.
	c_rect rect;
	get_screen_rect(rect);

	m_bt_up_rect.m_left = rect.m_left;
	m_bt_up_rect.m_right = rect.m_left + rect.Width() / 2 - 1;
	m_bt_up_rect.m_top = rect.m_bottom + 1;
	m_bt_up_rect.m_bottom = m_bt_up_rect.m_top + ARROW_BT_HEIGHT;

	m_bt_down_rect.m_left = rect.m_left + rect.Width() / 2;
	m_bt_down_rect.m_right = rect.m_right;
	m_bt_down_rect.m_top = rect.m_bottom + 1;
	m_bt_down_rect.m_bottom = m_bt_down_rect.m_top + ARROW_BT_HEIGHT;
}

bool c_spin_box::on_touch(int x, int y, TOUCH_ACTION action)
{
	(action == TOUCH_DOWN) ? on_touch_down(x, y) : on_touch_up(x, y);
	return c_wnd::on_touch(x, y, action);
}

void c_spin_box::on_touch_down(int x, int y)
{
	if (FALSE == m_wnd_rect.PtInRect(x, y))
	{//maybe click on up/down arrow button
		return;
	}
	if (STATUS_NORMAL == m_status)
	{
		m_parent->set_child_focus(this);
	}
}

void c_spin_box::on_touch_up(int x, int y)
{
	if (FALSE == m_wnd_rect.PtInRect(x, y))
	{//maybe click on up/down arrow button
		return;
	}

	if (STATUS_FOCUSED == m_status)
	{
		m_status = STATUS_PUSHED;
		on_paint();
	}
	else if (STATUS_PUSHED == m_status)
	{
		m_value = m_cur_value;
		m_status = STATUS_FOCUSED;
		on_paint();
		notify_parent(GL_SPIN_CONFIRM, get_id(), m_value);
	}
}

void c_spin_box::on_focus()
{
	m_status = STATUS_FOCUSED;
	on_paint();
}

void c_spin_box::on_kill_focus()
{
	m_cur_value = m_value;
	m_status = STATUS_NORMAL;
	on_paint();
}

void c_spin_box::show_arrow_button()
{
	m_bt_up.connect(this, ID_BT_ARROW_UP, "\xe2\x96\xb2"/*▲*/, 0, m_wnd_rect.Height(), m_bt_up_rect.Width(),m_bt_up_rect.Height());
	m_bt_up.show_window();
	m_bt_down.connect(this, ID_BT_ARROW_DOWN, "\xe2\x96\xbc"/*▼*/, m_bt_up_rect.Width(), m_wnd_rect.Height(), m_bt_down_rect.Width(),m_bt_down_rect.Height());
	m_bt_down.show_window();

	m_style |= GL_ATTR_PRIORITY;
}

void c_spin_box::hide_arrow_button()
{
	m_bt_up.disconnect();
	m_bt_down.disconnect();
	m_style &= ~GL_ATTR_PRIORITY;
}

void c_spin_box::on_paint()
{
	c_rect rect, tmp_rect, empty_rect;
	get_screen_rect(rect);
	tmp_rect.m_left = rect.m_left;
	tmp_rect.m_right = rect.m_right;

	switch(m_status)
	{
	case STATUS_NORMAL:
		if (m_z_order > m_parent->get_z_order())
		{
			hide_arrow_button();
			m_surface->set_frame_layer(empty_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
		}
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
		break;
	case STATUS_FOCUSED:
		if (m_z_order > m_parent->get_z_order())
		{
			hide_arrow_button();
			m_surface->set_frame_layer(empty_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
		}
		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
		break;
	case STATUS_PUSHED:
		if (m_z_order == m_parent->get_z_order())
		{
			m_z_order++;
		}
		tmp_rect.m_top = m_bt_down_rect.m_top;
		tmp_rect.m_bottom = m_bt_down_rect.m_bottom;
		m_surface->set_frame_layer(tmp_rect, m_z_order);
		show_arrow_button();

		m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_PUSHED), m_parent->get_z_order());
		m_surface->draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_BORDER), m_parent->get_z_order(), 2);
		c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font_type, GL_RGB(2, 124, 165), GL_ARGB(0, 0, 0, 0), m_style);
		return;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_style);
}

void c_spin_box::on_arrow_up_bt_click(unsigned int ctr_id)
{
	if (m_cur_value + m_step > m_max)
	{
		return;
	}
	m_cur_value += m_step;
	notify_parent(GL_SPIN_CHANGE, get_id(), m_cur_value);
	on_paint();
}

void c_spin_box::on_arrow_down_bt_click(unsigned int ctr_id)
{
	if (m_cur_value - m_step < m_min)
	{
		return;
	}
	m_cur_value -= m_step;
	notify_parent(GL_SPIN_CHANGE, get_id(), m_cur_value);
	on_paint();
}
