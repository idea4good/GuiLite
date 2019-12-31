#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/surface.h"
#include "../core_include/display.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/theme.h"
#include "../widgets_include/dialog.h"

DIALOG_ARRAY c_dialog::ms_the_dialogs[SURFACE_CNT_MAX];
void c_dialog::pre_create_wnd()
{
	m_attr = WND_ATTRIBUTION(0);// no focus/visible
	m_z_order = Z_ORDER_LEVEL_1;
	m_bg_color = GL_RGB(33, 42, 53);
}

void c_dialog::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	m_surface->fill_rect(rect, m_bg_color, m_z_order);

	if (m_str)
	{
		c_word::draw_string(m_surface, m_z_order, m_str, rect.m_left+35, rect.m_top, c_theme::get_font(FONT_DEFAULT), GL_RGB(255, 255, 255), GL_ARGB(0, 0, 0, 0), ALIGN_LEFT);
	}
}

c_dialog* c_dialog::get_the_dialog(c_surface* surface)
{
	for(int i = 0; i < SURFACE_CNT_MAX; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			return ms_the_dialogs[i].dialog;
		}
	}
	return 0;
}

int c_dialog::open_dialog(c_dialog* p_dlg, bool modal_mode)
{
	if (0 == p_dlg)
	{
		ASSERT(false);
		return 0;
	}
	c_dialog* cur_dlg = get_the_dialog(p_dlg->get_surface());
	if (cur_dlg == p_dlg)
	{
		return 1;
	}

	if(cur_dlg)
	{
		cur_dlg->set_attr(WND_ATTRIBUTION(0));
	}

	c_rect rc;
	p_dlg->get_screen_rect(rc);
	p_dlg->get_surface()->set_frame_layer_visible_rect(rc, Z_ORDER_LEVEL_1);

	p_dlg->set_attr(modal_mode ? (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_PRIORITY) : (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS));
	p_dlg->show_window();
	p_dlg->set_me_the_dialog();
	return 1;
}

int c_dialog::close_dialog(c_surface* surface)
{
	c_dialog* dlg = get_the_dialog(surface);

	if (0 == dlg)
	{
		return 0;
	}
	c_rect rc;
	
	dlg->set_attr(WND_ATTRIBUTION(0));
	surface->set_frame_layer_visible_rect(rc, dlg->m_z_order);

	//clear the dialog
	for(int i = 0; i < SURFACE_CNT_MAX; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			ms_the_dialogs[i].dialog = 0;
			return 1;
		}
	}
	ASSERT(false);
	return -1;
}

int c_dialog::set_me_the_dialog()
{
	c_surface* surface = get_surface();
	for(int i = 0; i < SURFACE_CNT_MAX; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			ms_the_dialogs[i].dialog = this;
			return 0;
		}
	}

	for(int i = 0; i < SURFACE_CNT_MAX; i++)
	{
		if(ms_the_dialogs[i].surface == 0)
		{
			ms_the_dialogs[i].dialog = this;
			ms_the_dialogs[i].surface = surface;
			return 1;
		}
	}
	ASSERT(false);
	return -2;
}
