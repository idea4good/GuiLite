#include "core_include/api.h"
#include "core_include/rect.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "core_include/surface.h"
#include "core_include/resource.h"
#include "core_include/bitmap.h"
#include "core_include/theme.h"
#include "core_include/word.h"
#include "../widgets_include/label.h"

void c_label::pre_create_wnd()
{
	m_attr = ATTR_VISIBLE;
	m_font_color = c_theme::get_color(COLOR_WND_FONT);
	m_font_type = c_theme::get_font(FONT_DEFAULT);
}

void c_label::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);

	if (m_str)
	{
		m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, m_parent->get_bg_color(), m_z_order);
		c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, m_parent->get_bg_color(), ALIGN_LEFT | ALIGN_VCENTER);
	}
}
