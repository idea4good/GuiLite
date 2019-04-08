#include "core_include/api.h"
#include "core_include/rect.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "core_include/msg.h"
#include "core_include/surface.h"
#include "core_include/display.h"
#include "core_include/resource.h"
#include "core_include/bitmap.h"
#include "core_include/word.h"
#include "core_include/theme.h"
#include "../widgets_include/button.h"
#include "../widgets_include/dialog.h"

#include <string.h>

DIALOG_ARRAY c_dialog::ms_the_dialogs[SURFACE_CNT_MAX];
void c_dialog::pre_create_wnd()
{
	m_style = GL_ATTR_FOCUS;
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
	return NULL;
}

int c_dialog::open_dialog(c_dialog* p_dlg)
{
	if (NULL == p_dlg)
	{
		ASSERT(FALSE);
		return 0;
	}
	c_dialog* cur_dlg = get_the_dialog(p_dlg->get_surface());
	if (cur_dlg == p_dlg)
	{
		return 1;
	}

	if(cur_dlg)
	{
		cur_dlg->modify_style(0, GL_ATTR_VISIBLE);
	}

	c_rect rc;
	p_dlg->get_screen_rect(rc);
	p_dlg->get_surface()->set_frame_layer(rc, Z_ORDER_LEVEL_1);

	p_dlg->modify_style(GL_ATTR_VISIBLE, 0);
	p_dlg->show_window();
	p_dlg->set_me_the_dialog();
	return 1;
}

int c_dialog::close_dialog(c_surface* surface)
{
	c_dialog* dlg = get_the_dialog(surface);

	if (NULL == dlg)
	{
		return 0;
	}
	c_rect rc;
	
	dlg->modify_style(0, GL_ATTR_VISIBLE);
	surface->set_frame_layer(rc, dlg->m_z_order);

	//clear the dialog
	for(int i = 0; i < SURFACE_CNT_MAX; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			ms_the_dialogs[i].dialog = NULL;
			return 1;
		}
	}
	ASSERT(FALSE);
	return -1;
}

void c_dialog::on_touch_down(int x, int y)
{
	c_wnd *child = m_top_child;
	c_rect rect;
	get_wnd_rect(rect);

	if ( NULL != child )
	{
		while ( child )
		{
			if (child->m_z_order > m_z_order)
			{
				x -= rect.m_left;
				y -= rect.m_top;
				child->on_touch_down(x, y);
				return;
			}
			child = child->m_next_sibling;
		}
	}
	c_wnd::on_touch_down(x, y);
}

void c_dialog::on_touch_up(int x, int y)
{
	c_wnd *child = m_top_child;
	c_rect rect;
	get_wnd_rect(rect);
	if ( NULL != child )
	{
		while ( child )
		{
			if (child->m_z_order > m_z_order)
			{
				x -= rect.m_left;
				y -= rect.m_top;
				return child->on_touch_up(x, y);
			}
			child = child->m_next_sibling;
		}
	}
	c_wnd::on_touch_up(x, y);
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
		if(ms_the_dialogs[i].surface == NULL)
		{
			ms_the_dialogs[i].dialog = this;
			if(this)
			{
				ms_the_dialogs[i].surface = surface;
			}
			return 1;
		}
	}
	ASSERT(FALSE);
	return -2;
}
