#pragma once

#include "../core/api.h"
#include "../core/resource.h"
#include "../core/word.h"
#include "../core/wnd.h"
#include "../core/display.h"
#include "../core/theme.h"
#include "../widgets/button.h"
#include <string.h>

//Changing key width/height will change the width/height of keyboard
#define KEY_WIDTH          65
#define KEY_HEIGHT         38

#define KEYBOARD_WIDTH		((KEY_WIDTH + 2) * 10)
#define KEYBOARD_HEIGHT		((KEY_HEIGHT + 2) * 4)
#define NUM_BOARD_WIDTH		((KEY_WIDTH + 2) * 4)
#define NUM_BOARD_HEIGHT	((KEY_HEIGHT + 2) * 4)

#define CAPS_WIDTH			(KEY_WIDTH * 3 / 2)
#define DEL_WIDTH			(KEY_WIDTH * 3 / 2 + 1)
#define ESC_WIDTH			(KEY_WIDTH * 2 + 2)
#define SWITCH_WIDTH		(KEY_WIDTH * 3 / 2 )
#define SPACE_WIDTH			(KEY_WIDTH * 3 + 2 * 2)
#define DOT_WIDTH			(KEY_WIDTH * 3 / 2 + 3)
#define ENTER_WIDTH			(KEY_WIDTH * 2 + 2)

#define POS_X(c)			((KEY_WIDTH * c) + (c + 1) * 2)
#define POS_Y(r)			((KEY_HEIGHT * r) + (r + 1) * 2)

#define KEYBOARD_CLICK			0x5014
#define ON_KEYBORAD_UPDATE(func)  \
{MSG_TYPE_WND, KEYBOARD_CLICK, 0,  msgCallback(&func)},

typedef enum
{
	STATUS_UPPERCASE,
	STATUS_LOWERCASE
}KEYBOARD_STATUS;

typedef enum
{
	STYLE_ALL_BOARD,
	STYLE_NUM_BOARD
}KEYBOARD_STYLE;

typedef enum
{
	CLICK_CHAR,
	CLICK_ENTER,
	CLICK_ESC
}CLICK_STATUS;

extern WND_TREE g_key_board_children[];
extern WND_TREE g_number_board_children[];
class c_keyboard: public c_wnd
{
public:
	c_keyboard() { m_attr = WND_ATTRIBUTION(0); }
	int open_keyboard(c_wnd *user, unsigned short resource_id, KEYBOARD_STYLE style, WND_CALLBACK on_click)
	{
		c_rect user_rect;
		user->get_wnd_rect(user_rect);
		if ((style != STYLE_ALL_BOARD) && (style != STYLE_NUM_BOARD))
		{
			ASSERT(false);
			return -1;
		}
		if (style == STYLE_ALL_BOARD)
		{//Place keyboard at the bottom of user's parent window.
			c_rect user_parent_rect;
			user->get_parent()->get_wnd_rect(user_parent_rect);
			c_wnd::connect(user, resource_id, 0, (0 - user_rect.m_left), (user_parent_rect.height() - user_rect.m_top - KEYBOARD_HEIGHT - 1), KEYBOARD_WIDTH, KEYBOARD_HEIGHT, g_key_board_children);
		}
		else if (style == STYLE_NUM_BOARD)
		{//Place keyboard below the user window.
			c_wnd::connect(user, resource_id, 0, 0, user_rect.height(), NUM_BOARD_WIDTH, NUM_BOARD_HEIGHT, g_number_board_children);
		}

		m_on_click = on_click;
		c_rect rc;
		get_screen_rect(rc);
		m_surface->activate_layer(rc, m_z_order);
		show_window();
		return 0;
	}

	void close_keyboard()
	{
		c_wnd::disconnect();
		m_surface->activate_layer(c_rect(), m_z_order);//inactivate the layer of keyboard by empty rect.
	}
	
	virtual void on_init_children()
	{
		c_wnd* child = m_top_child;
		if (0 != child)
		{
			while (child)
			{
				((c_button*)child)->set_on_click(WND_CALLBACK(&c_keyboard::on_key_clicked));
				child = child->get_next_sibling();
			}
		}
	}

	KEYBOARD_STATUS get_cap_status(){return m_cap_status;}
	char* get_str() { return m_str; }
protected:
	virtual void pre_create_wnd()
	{
		m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_PRIORITY);
		m_cap_status = STATUS_UPPERCASE;
		m_z_order = m_surface->get_max_z_order();
		memset(m_str, 0, sizeof(m_str));
		m_str_len = 0;
	}
	virtual void on_paint()
	{
		c_rect rect;
		get_screen_rect(rect);
		m_surface->fill_rect(rect, GL_RGB(0, 0, 0), m_z_order);
	}

	void on_key_clicked(int id, int param)
	{
		switch (id)
		{
		case 0x14:
			on_caps_clicked(id, param);
			break;
		case '\n':
			on_enter_clicked(id, param);
			break;
		case 0x1B:
			on_esc_clicked(id, param);
			break;
		case 0x7F:
			on_del_clicked(id, param);
			break;
		default:
			on_char_clicked(id, param);
			break;
		}
	}
	void on_char_clicked(int id, int param)
	{//id = char ascii code.
		if (m_str_len >= sizeof(m_str))
		{
			return;
		}
		if ((id >= '0' && id <= '9') || id == ' ' || id == '.')
		{
			goto InputChar;
		}

		if (id >= 'A' && id <= 'Z')
		{
			if (STATUS_LOWERCASE == m_cap_status)
			{
				id += 0x20;
			}
			goto InputChar;
		}
		if (id == 0x90) return;//TBD
		ASSERT(false);
	InputChar:
		m_str[m_str_len++] = id;
		(m_parent->*(m_on_click))(m_id, CLICK_CHAR);
	}
	void on_del_clicked(int id, int param)
	{
		if (m_str_len <= 0)
		{
			return;
		}
		m_str[--m_str_len] = 0;
		(m_parent->*(m_on_click))(m_id, CLICK_CHAR);
	}
	void on_caps_clicked(int id, int param)
	{
		m_cap_status = (m_cap_status == STATUS_LOWERCASE) ? STATUS_UPPERCASE : STATUS_LOWERCASE;
		show_window();
	}
	void on_enter_clicked(int id, int param)
	{
		memset(m_str, 0, sizeof(m_str));
		(m_parent->*(m_on_click))(m_id, CLICK_ENTER);
	}
	void on_esc_clicked(int id, int param)
	{
		memset(m_str, 0, sizeof(m_str));
		(m_parent->*(m_on_click))(m_id, CLICK_ESC);
	}
private:
	char m_str[32];
	int	 m_str_len;
	KEYBOARD_STATUS m_cap_status;
	WND_CALLBACK m_on_click;
};

class c_keyboard_button : public c_button
{
protected:
	virtual void on_paint()
	{
		c_rect rect;
		get_screen_rect(rect);
		switch (m_status)
		{
		case STATUS_NORMAL:
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
			break;
		case STATUS_FOCUSED:
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
			break;
		case STATUS_PUSHED:
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_PUSHED), m_z_order);
			m_surface->draw_rect(rect, c_theme::get_color(COLOR_WND_BORDER), 2, m_z_order);
			break;
		default:
			ASSERT(false);
			break;
		}

		if (m_id == 0x14)
		{
			return c_word::draw_string_in_rect(m_surface, m_z_order, "Caps", rect, m_font, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER);
		}
		else if (m_id == 0x1B)
		{
			return c_word::draw_string_in_rect(m_surface, m_z_order, "Esc", rect, m_font, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER);
		}
		else if (m_id == ' ')
		{
			return c_word::draw_string_in_rect(m_surface, m_z_order, "Space", rect, m_font, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER);
		}
		else if (m_id == '\n')
		{
			return c_word::draw_string_in_rect(m_surface, m_z_order, "Enter", rect, m_font, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER);
		}
		else if (m_id == '.')
		{
			return c_word::draw_string_in_rect(m_surface, m_z_order, ".", rect, m_font, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER);
		}
		else if (m_id == 0x7F)
		{
			return c_word::draw_string_in_rect(m_surface, m_z_order, "Back", rect, m_font, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER);
		}
		else if (m_id == 0x90)
		{
			return c_word::draw_string_in_rect(m_surface, m_z_order, "?123", rect, m_font, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER);
		}

		char letter[] = { 0, 0 };
		if (m_id >= 'A' && m_id <= 'Z')
		{
			letter[0] = (((c_keyboard*)m_parent)->get_cap_status() == STATUS_UPPERCASE) ? m_id : (m_id + 0x20);
		}
		else if (m_id >= '0' && m_id <= '9')
		{
			letter[0] = (char)m_id;
		}
		c_word::draw_string_in_rect(m_surface, m_z_order, letter, rect, m_font, m_font_color, GL_ARGB(0, 0, 0, 0), ALIGN_HCENTER);
	}
};
