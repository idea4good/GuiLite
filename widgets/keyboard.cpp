#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/surface.h"
#include "../core_include/theme.h"
#include "../widgets_include/button.h"
#include "../widgets_include/keyboard.h"
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

static c_keyboard_button s_button_0,s_button_1, s_button_2, s_button_3, s_button_4, s_button_5, s_button_6, s_button_7, s_button_8, s_button_9;
static c_keyboard_button s_button_A, s_button_B, s_button_C, s_button_D, s_button_E, s_button_F, s_button_G, s_button_H, s_button_I, s_button_J;
static c_keyboard_button s_button_K, s_button_L, s_button_M, s_button_N, s_button_O, s_button_P, s_button_Q, s_button_R, s_button_S, s_button_T;
static c_keyboard_button s_button_U, s_button_V, s_button_W, s_button_X, s_button_Y, s_button_Z;
static c_keyboard_button s_button_dot, s_button_caps, s_button_space, s_button_enter, s_button_del, s_button_esc, s_button_num_switch;

WND_TREE g_key_board_children[] =
{
	//Row 1
	{&s_button_Q, 'Q', 0, POS_X(0), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_W, 'W', 0, POS_X(1), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_E, 'E', 0, POS_X(2), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_R, 'R', 0, POS_X(3), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_T, 'T', 0, POS_X(4), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_Y, 'Y', 0, POS_X(5), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_U, 'U', 0, POS_X(6), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_I, 'I', 0, POS_X(7), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_O, 'O', 0, POS_X(8), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_P, 'P', 0, POS_X(9), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	//Row 2 
	{&s_button_A, 'A', 0, ((KEY_WIDTH / 2) + POS_X(0)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_S, 'S', 0, ((KEY_WIDTH / 2) + POS_X(1)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_D, 'D', 0, ((KEY_WIDTH / 2) + POS_X(2)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_F, 'F', 0, ((KEY_WIDTH / 2) + POS_X(3)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_G, 'G', 0, ((KEY_WIDTH / 2) + POS_X(4)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_H, 'H', 0, ((KEY_WIDTH / 2) + POS_X(5)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_J, 'J', 0, ((KEY_WIDTH / 2) + POS_X(6)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_K, 'K', 0, ((KEY_WIDTH / 2) + POS_X(7)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_L, 'L', 0, ((KEY_WIDTH / 2) + POS_X(8)), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	//Row 3
	{&s_button_caps, 0x14,	0, POS_X(0),						POS_Y(2), CAPS_WIDTH,	KEY_HEIGHT},
	{&s_button_Z,	'Z',	0, ((KEY_WIDTH / 2) + POS_X(1)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_X,	'X',	0, ((KEY_WIDTH / 2) + POS_X(2)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_C,	'C',	0, ((KEY_WIDTH / 2) + POS_X(3)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_V,	'V',	0, ((KEY_WIDTH / 2) + POS_X(4)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_B,	'B',	0, ((KEY_WIDTH / 2) + POS_X(5)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_N,	'N',	0, ((KEY_WIDTH / 2) + POS_X(6)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_M,	'M',	0, ((KEY_WIDTH / 2) + POS_X(7)),	POS_Y(2), KEY_WIDTH,	KEY_HEIGHT},
	{&s_button_del,	0x7F,	0, ((KEY_WIDTH / 2) + POS_X(8)),	POS_Y(2), DEL_WIDTH,	KEY_HEIGHT},
	//Row 4
	{&s_button_esc,			0x1B,	0, POS_X(0),						POS_Y(3), ESC_WIDTH,	KEY_HEIGHT},
	{&s_button_num_switch,	0x90,	0, POS_X(2),						POS_Y(3), SWITCH_WIDTH,	KEY_HEIGHT},
	{&s_button_space,		' ',	0, ((KEY_WIDTH / 2) + POS_X(3)),	POS_Y(3), SPACE_WIDTH,	KEY_HEIGHT},
	{&s_button_dot,			'.',	0, ((KEY_WIDTH / 2) + POS_X(6)),	POS_Y(3), DOT_WIDTH,	KEY_HEIGHT},
	{&s_button_enter,		'\n',	0, POS_X(8),						POS_Y(3), ENTER_WIDTH,	KEY_HEIGHT},
	{0,0,0,0,0,0,0}
};

WND_TREE g_number_board_children[] =
{
	{&s_button_1,	'1',	0, POS_X(0), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_2,	'2',	0, POS_X(1), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_3,	'3',	0, POS_X(2), POS_Y(0), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_del, 0x7F,	0, POS_X(3), POS_Y(0), KEY_WIDTH, KEY_HEIGHT * 2 + 2},

	{&s_button_4,	'4',	0, POS_X(0), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_5,	'5',	0, POS_X(1), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_6,	'6',	0, POS_X(2), POS_Y(1), KEY_WIDTH, KEY_HEIGHT},

	{&s_button_7,	'7',	0, POS_X(0), POS_Y(2), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_8,	'8',	0, POS_X(1), POS_Y(2), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_9,	'9',	0, POS_X(2), POS_Y(2), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_enter,'\n',	0, POS_X(3), POS_Y(2), KEY_WIDTH, KEY_HEIGHT * 2 + 2},

	{&s_button_esc,	0x1B,	0, POS_X(0), POS_Y(3), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_0,	'0',	0, POS_X(1), POS_Y(3), KEY_WIDTH, KEY_HEIGHT},
	{&s_button_dot,	'.',	0, POS_X(2), POS_Y(3), KEY_WIDTH, KEY_HEIGHT},
	{0,0,0,0,0,0,0}
};

GL_BEGIN_MESSAGE_MAP(c_keyboard)
ON_GL_BN_CLICKED('A', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('B', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('C', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('D', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('E', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('F', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('G', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('H', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('I', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('J', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('K', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('L', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('M', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('N', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('O', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('P', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('Q', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('R', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('S', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('T', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('U', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('V', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('W', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('X', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('Y', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('Z', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('0', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('1', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('2', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('3', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('4', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('5', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('6', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('7', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('8', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('9', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED(' ', c_keyboard::on_char_clicked)
ON_GL_BN_CLICKED('.', c_keyboard::on_char_clicked)

ON_GL_BN_CLICKED(0x7F, c_keyboard::on_del_clicked)
ON_GL_BN_CLICKED(0x14, c_keyboard::on_caps_clicked)
ON_GL_BN_CLICKED('\n', c_keyboard::on_enter_clicked)
ON_GL_BN_CLICKED(0x1B, c_keyboard::on_esc_clicked)
GL_END_MESSAGE_MAP()

int c_keyboard::connect(c_wnd *user, unsigned short resource_id, KEYBOARD_STYLE style)
{
	c_rect user_rect;
	user->get_wnd_rect(user_rect);
	if (style == STYLE_ALL_BOARD)
	{//Place keyboard at the bottom of user's parent window.
		c_rect user_parent_rect;
		user->get_parent()->get_wnd_rect(user_parent_rect);
		return c_wnd::connect(user, resource_id, 0, (0 - user_rect.m_left), (user_parent_rect.Height() - user_rect.m_top - KEYBOARD_HEIGHT), KEYBOARD_WIDTH, KEYBOARD_HEIGHT, g_key_board_children);
	}
	else if(style == STYLE_NUM_BOARD)
	{//Place keyboard below the user window.
		return c_wnd::connect(user, resource_id, 0, 0, user_rect.Height(), NUM_BOARD_WIDTH, NUM_BOARD_HEIGHT, g_number_board_children);
	}
	else
	{
		ASSERT(false);
	}
	return -1;
}

void c_keyboard::pre_create_wnd()
{	
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
	m_cap_status = STATUS_UPPERCASE;
	memset(m_str, 0, sizeof(m_str));
	m_str_len = 0;
}

void c_keyboard::on_caps_clicked(unsigned int ctrl_id)
{
	m_cap_status = (m_cap_status == STATUS_LOWERCASE) ? STATUS_UPPERCASE : STATUS_LOWERCASE;
	show_window();
}

void c_keyboard::on_enter_clicked(unsigned int ctrl_id)
{
	memset(m_str, 0, sizeof(m_str));
    notify_parent(KEYBORAD_CLICK, CLICK_ENTER);
}

void c_keyboard::on_esc_clicked(unsigned int ctrl_id)
{
	memset(m_str, 0, sizeof(m_str));
	notify_parent(KEYBORAD_CLICK, CLICK_ESC);
}

void c_keyboard::on_del_clicked(unsigned int ctrl_id)
{
	if (m_str_len <= 0)
	{
		return;
	}
	m_str[--m_str_len] = 0;
	notify_parent(KEYBORAD_CLICK, CLICK_CHAR);
}

void c_keyboard::on_char_clicked(unsigned int ctrl_id)
{//ctrl_id = char ascii code.
	if (m_str_len >= sizeof(m_str))
	{
		return;
	}
	if ((ctrl_id >= '0' && ctrl_id <= '9') || ctrl_id == ' ' || ctrl_id == '.')
	{
		goto InputChar;
	}

	if (ctrl_id >= 'A' && ctrl_id <= 'Z')
	{
		if (STATUS_LOWERCASE == m_cap_status)
		{
			ctrl_id += 0x20;
		}
		goto InputChar;
	}
	ASSERT(false);
InputChar:
	m_str[m_str_len++] = ctrl_id;
	notify_parent(KEYBORAD_CLICK, CLICK_CHAR);
}

void c_keyboard::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	m_surface->fill_rect(rect, GL_RGB(0, 0, 0), m_z_order);
}

void c_keyboard_button::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	switch(m_status)
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
	
	if (m_resource_id == 0x14)
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Caps", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == 0x1B)
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Esc", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == ' ')
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Space", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == '\n')
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Enter", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == '.')
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, ".", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == 0x7F)
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "Back", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	else if (m_resource_id == 0x90)
	{
		return c_word::draw_string_in_rect(m_surface, m_z_order, "?123", rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
	}
	
	char letter[] = { 0, 0 };
	if (m_resource_id >= 'A' && m_resource_id <= 'Z')
	{
		letter[0] = (((c_keyboard*)m_parent)->get_cap_status() == STATUS_UPPERCASE) ? m_resource_id : (m_resource_id + 0x20);
	}
	else if (m_resource_id >= '0' && m_resource_id <= '9')
	{
		letter[0] = m_resource_id;
	}
	c_word::draw_string_in_rect(m_surface, m_z_order, letter, rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_attr);
}
