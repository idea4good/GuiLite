#include "core_include/api.h"
#include "core_include/rect.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "core_include/surface.h"
#include "core_include/resource.h"
#include "core_include/bitmap.h"
#include "core_include/word.h"
#include "../gui_include/my_resource.h"
#include "../gui_include/label.h"

void c_label::pre_create_wnd()
{
	m_style = GL_ATTR_VISIBLE | ALIGN_LEFT | ALIGN_VCENTER;
	m_font_color = GL_RGB(255,255,255);

	m_font_type = c_my_resource::get_font(FONT_ENG_SMB);
	m_bg_color = get_parent()->get_bg_color();
}

void c_label::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);

	if (m_str)
	{
		fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom,m_bg_color);
		c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, GL_ARGB(0, 0, 0, 0), m_style);
	}
}
