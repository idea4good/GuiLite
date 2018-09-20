#include "core_include/api.h"
#include "core_include/rect.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "core_include/resource.h"
#include "core_include/word.h"
#include "core_include/surface.h"
#include "../gui_include/button.h"
#include "../gui_include/my_resource.h"
#include "../gui_include/spinbox.h"

#define ARROW_BT_HEIGHT		55
#define ID_BT_ARROW_UP      1
#define ID_BT_ARROW_DOWN    2

GLT_BEGIN_MESSAGE_MAP(c_spin_box)
ON_GLT_BN_CLICKED(ID_BT_ARROW_UP, c_spin_box::on_arrow_up_bt_click)
ON_GLT_BN_CLICKED(ID_BT_ARROW_DOWN, c_spin_box::on_arrow_down_bt_click)
GLT_END_MESSAGE_MAP()

void c_spin_box::pre_create_wnd()
{
	m_style = GLT_ATTR_VISIBLE | GLT_ATTR_FOCUS | ALIGN_HCENTER | ALIGN_VCENTER;
	m_font_type = c_my_resource::get_font(FONT_ENG_SMB_AA);
	m_bg_color = c_my_resource::get_color(CTRL_BACK_GROUND);
	m_font_color = c_my_resource::get_color(CTRL_FORE_GROUND);

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

void c_spin_box::on_touch_down(int x, int y)
{
	c_rect arrow_rect = m_wnd_rect;
	arrow_rect.m_right = m_bt_down_rect.m_right;
	arrow_rect.m_bottom = m_bt_down_rect.m_bottom;

	if ( TRUE == m_wnd_rect.PtInRect(x, y) )
	{//click spin box
		if (STATUS_NORMAL == m_status)
		{
			get_parent()->set_focus(this);
		}
	}
	else if (TRUE == arrow_rect.PtInRect(x, y))
	{//click arrow button
        c_wnd::on_touch_down(x, y);
	}
	else
	{//click invalid place.
		if (STATUS_PUSHED == m_status)
		{
			if (m_value != m_cur_value)
			{
				m_value = m_cur_value;
			}
			modify_status(STATUS_FOCUSED);
			on_paint();
			notify_parent(GLT_SPIN_CONFIRM, get_id(), 0);
		}        
	}
}

void c_spin_box::on_touch_up(int x, int y)
{
	if (STATUS_FOCUSED == m_status)
	{
		modify_status(STATUS_PUSHED);
		on_paint();
		notify_parent(GLT_SPIN_SELECT, get_id(), 0);
	}
	else if (STATUS_PUSHED == m_status)
	{
		if (m_wnd_rect.PtInRect(x, y))
		{//click spin box.
			if (m_value != m_cur_value)
			{
				m_value = m_cur_value;
			}
			modify_status(STATUS_FOCUSED);
			on_paint();
			notify_parent(GLT_SPIN_CONFIRM, get_id(), 0);
		}
		else
		{//click arrow button.
			c_wnd::on_touch_up(x, y);
		}
	}
}

void c_spin_box::on_focus()
{
	modify_status(STATUS_FOCUSED);
	on_paint();
}

void c_spin_box::on_kill_focus()
{
	m_cur_value = m_value;
	modify_status(STATUS_NORMAL);
	on_paint();
}

void c_spin_box::show_arrow_button()
{
	fill_rect(m_bt_up_rect.m_left, m_bt_up_rect.m_top, m_bt_down_rect.m_right, m_bt_down_rect.m_bottom, GLT_RGB(99,108,124));

	m_bt_up.connect(this, ID_BT_ARROW_UP, 0, 0, m_wnd_rect.Height(), m_bt_up_rect.Width(),m_bt_up_rect.Height());
	m_bt_up.set_bitmap(c_my_resource::get_bmp(BITMAP_UP_BT_NORMAL));
	m_bt_up.set_focus_bitmap(c_my_resource::get_bmp(BITMAP_UP_BT_FOCUS));
	m_bt_up.set_pushed_bitmap(c_my_resource::get_bmp(BITMAP_UP_BT_FOCUS));
	m_bt_up.show_window();

	m_bt_down.connect(this, ID_BT_ARROW_DOWN, 0, m_bt_up_rect.Width(), m_wnd_rect.Height(), m_bt_down_rect.Width(),m_bt_down_rect.Height());
	m_bt_down.set_bitmap(c_my_resource::get_bmp(BITMAP_DOWN_BT_NORMAL));
	m_bt_down.set_focus_bitmap(c_my_resource::get_bmp(BITMAP_DOWN_BT_FOCUS));
	m_bt_down.set_pushed_bitmap(c_my_resource::get_bmp(BITMAP_DOWN_BT_FOCUS));
	m_bt_down.show_window();
}

void c_spin_box::hide_arrow_button()
{
	m_bt_up.disconnect();
	m_bt_down.disconnect();
}

void c_spin_box::on_paint()
{
	c_rect rect,tmp_rect;
	get_screen_rect(rect);
	tmp_rect.m_left = rect.m_left;
	tmp_rect.m_right = rect.m_right;

	switch(m_status)
	{
	case STATUS_NORMAL:
		if (m_z_order > m_parent->get_z_order())
		{
			hide_arrow_button();
			tmp_rect.Empty();
			m_surface->set_frame_layer(tmp_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
		}
		fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(BUTTON_NORMAL));
		break;
	case STATUS_FOCUSED:
		if (m_z_order > m_parent->get_z_order())
		{
			hide_arrow_button();
			tmp_rect.Empty();
			m_surface->set_frame_layer(tmp_rect, m_z_order);
			m_z_order = m_parent->get_z_order();
		}
		fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(BUTTON_FOCUS));
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

		m_surface->fill_rect_ex(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, m_bg_color, c_my_resource::get_shape(LIST_BOX_PUSH),m_parent->get_z_order());
		c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font_type, GLT_RGB(2, 124, 165), COLOR_TRANPARENT, m_style);
		return;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font_type, m_font_color, COLOR_TRANPARENT, m_style);
}

void c_spin_box::on_arrow_up_bt_click(unsigned int ctr_id)
{
	m_cur_value += m_step;
	if (m_cur_value > m_max)
	{
		m_cur_value = m_max;
	}
	else
	{
		on_paint();
	}
}

void c_spin_box::on_arrow_down_bt_click(unsigned int ctr_id)
{
	m_cur_value -= m_step;
	if (m_cur_value < m_min)
	{
		m_cur_value = m_min;
	}
	else
	{
		on_paint();
	}
}
