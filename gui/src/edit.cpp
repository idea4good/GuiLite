#include "core_include/api.h"
#include "core_include/rect.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "core_include/resource.h"
#include "core_include/word.h"
#include "core_include/surface.h"
#include "../gui_include/button.h"
#include "../gui_include/label.h"
#include "../gui_include/edit.h"
#include "../gui_include/my_resource.h"
#include "../gui_include/dialog.h"
#include "../gui_include/keyboard.h"
#include <string.h>

#define IDD_ALL_KEY_BOARD		0x5012
#define IDD_NUM_KEY_BOARD		0x5013

GLT_BEGIN_MESSAGE_MAP(c_edit)
ON_KEYBORAD_UPDATE(IDD_ALL_KEY_BOARD, c_edit::on_key_board_click)
ON_KEYBORAD_UPDATE(IDD_NUM_KEY_BOARD, c_edit::on_key_board_click)
GLT_END_MESSAGE_MAP()

static c_keyboard  s_keyboard;

void c_edit::pre_create_wnd()
{
	m_style |= GLT_ATTR_VISIBLE | GLT_ATTR_FOCUS | ALIGN_HCENTER | ALIGN_VCENTER | KEY_BOARD_STYLE;
	m_font_type = c_my_resource::get_font(FONT_ENG_SMB_AA);
	m_font_color = c_my_resource::get_color(CTRL_FORE_GROUND);
	m_bg_color = c_my_resource::get_color(CTRL_BACK_GROUND);

	memset(m_str_input, 0, sizeof(m_str_input));
	memset(m_str, 0, sizeof(m_str));
}

void c_edit::set_text(const char* str)
{
	if (str != NULL && strlen(str) < sizeof(m_str))
	{
		strcpy(m_str, str);
	}
}

void c_edit::on_touch_down(int x, int y)
{
	c_rect kb_rect_relate_2_edit_parent;
	s_keyboard.get_wnd_rect(kb_rect_relate_2_edit_parent);
	kb_rect_relate_2_edit_parent.m_left += m_wnd_rect.m_left;
	kb_rect_relate_2_edit_parent.m_right += m_wnd_rect.m_left;
	kb_rect_relate_2_edit_parent.m_top += m_wnd_rect.m_top;
	kb_rect_relate_2_edit_parent.m_bottom += m_wnd_rect.m_top;

	if (m_wnd_rect.PtInRect(x, y))
	{//click edit box
		if (STATUS_NORMAL == m_status)
		{
			get_parent()->set_focus(this);
		}
	}
	else if (kb_rect_relate_2_edit_parent.PtInRect(x,y))
	{//click key board
        c_wnd::on_touch_down(x, y);
	}
	else
	{
		if (STATUS_PUSHED == m_status)
		{
			modify_status(STATUS_FOCUSED);
			on_paint();
		}        
	}
}

void c_edit::on_touch_up(int x, int y)
{
	if (STATUS_FOCUSED == m_status)
	{
		modify_status(STATUS_PUSHED);
		on_paint();
	}
	else if (STATUS_PUSHED == m_status)
	{
		if (m_wnd_rect.PtInRect(x,y))
		{//click edit box
			modify_status(STATUS_FOCUSED);
			on_paint();
		}
		else
		{
			c_wnd::on_touch_up(x, y);
		}
	}
}

void c_edit::on_focus()
{
	modify_status(STATUS_FOCUSED);
	on_paint();
}

void c_edit::on_kill_focus()
{
	modify_status(STATUS_NORMAL);
	on_paint();
}

void c_edit::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	c_rect empty_rect;
	empty_rect.Empty();
	switch(m_status)
	{
	case STATUS_NORMAL:
		if (m_z_order > m_parent->get_z_order())
		{
			s_keyboard.disconnect();
			m_surface->set_frame_layer(empty_rect, s_keyboard.get_z_order());
			m_z_order = m_parent->get_z_order();
		}
		fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(BUTTON_NORMAL));
		break;
	case STATUS_FOCUSED:
		if (m_z_order > m_parent->get_z_order())
		{
			s_keyboard.disconnect();
			m_surface->set_frame_layer(empty_rect, s_keyboard.get_z_order());
			m_z_order = m_parent->get_z_order();
		}
		fill_rect_ex(rect, m_bg_color, c_my_resource::get_shape(BUTTON_FOCUS));
		break;
	case STATUS_PUSHED:
		if (m_z_order == m_parent->get_z_order())
		{
			m_z_order++;
			show_keyboard();
		}
		m_surface->fill_rect_ex(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, m_bg_color, c_my_resource::get_shape(LIST_BOX_SELECT), m_parent->get_z_order());
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	if (strlen(m_str_input))
	{
		c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str_input, rect, m_font_type, m_font_color, COLOR_TRANPARENT, m_style);
	}
	else
	{
		c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font_type, m_font_color, COLOR_TRANPARENT, m_style);
	}
}

void c_edit::show_keyboard()
{
	if (s_keyboard.get_id())
	{
		ASSERT(FALSE);
		return;
	}

	if ((get_style()&KEY_BOARD_STYLE) == KEY_BOARD_STYLE )
	{
		c_rect parent_rc;
		m_parent->get_wnd_rect(parent_rc);
		s_keyboard.set_style(STYLE_ALL_BOARD);
		s_keyboard.create(this, IDD_ALL_KEY_BOARD, 0, m_wnd_rect.m_left, m_wnd_rect.m_top, parent_rc.Width(), parent_rc.Height(), NULL);
	}
	else
	{
		s_keyboard.set_style(STYLE_NUM_BOARD);
		s_keyboard.create(this, IDD_NUM_KEY_BOARD, 0, 0, m_wnd_rect.Height(), 0, 0, NULL);
	}

	c_rect kb_rect;
	s_keyboard.get_screen_rect(kb_rect);
	m_surface->set_frame_layer(kb_rect, s_keyboard.get_z_order());
	s_keyboard.show_window();
}

void c_edit::on_key_board_click(unsigned int ctrl_id, long param)
{
	switch (param)
	{
	case CLICK_CHAR:
		strcpy(m_str_input, s_keyboard.get_str());
		on_paint();
		break;
	case CLICK_ENTER:
		if (strlen(m_str_input))
		{
			memcpy(m_str, m_str_input, sizeof(m_str_input));
		}
		modify_status(STATUS_FOCUSED);
		on_paint();
		break;
	case CLICK_ESC:
		memset(m_str_input, 0, sizeof(m_str_input));
		modify_status(STATUS_FOCUSED);
		on_paint();
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}
