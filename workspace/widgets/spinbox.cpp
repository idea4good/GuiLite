#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/surface.h"
#include "../core_include/theme.h"
#include "../widgets_include/button.h"
#include "../widgets_include/spinbox.h"

#define ARROW_BT_WIDTH		55
#define ID_BT_ARROW_UP      0x1111
#define ID_BT_ARROW_DOWN    0x2222

void c_spin_button::on_touch(int x, int y, TOUCH_ACTION action)
{
	if (action == TOUCH_UP)
	{
		(m_id == ID_BT_ARROW_UP) ? m_spin_box->on_arrow_up_bt_click() : m_spin_box->on_arrow_down_bt_click();
	}
	c_button::on_touch(x, y, action);
}

void c_spin_box::pre_create_wnd()
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
	get_screen_rect(rect);
	m_bt_down.m_spin_box = m_bt_up.m_spin_box = this;
	m_bt_down.connect(m_parent, ID_BT_ARROW_DOWN, "-", rect.m_left - ARROW_BT_WIDTH, rect.m_top, ARROW_BT_WIDTH, rect.Height());
	m_bt_up.connect(m_parent, ID_BT_ARROW_UP, "+", rect.m_right, rect.m_top, ARROW_BT_WIDTH, rect.Height());
}

void c_spin_box::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);

	m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
	c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font_type, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
}

void c_spin_box::on_arrow_up_bt_click()
{
	if (m_cur_value + m_step > m_max)
	{
		return;
	}
	m_cur_value += m_step;
	notify_parent(GL_SPIN_CHANGE, m_cur_value);
	on_paint();
}

void c_spin_box::on_arrow_down_bt_click()
{
	if (m_cur_value - m_step < m_min)
	{
		return;
	}
	m_cur_value -= m_step;
	notify_parent(GL_SPIN_CHANGE, m_cur_value);
	on_paint();
}
