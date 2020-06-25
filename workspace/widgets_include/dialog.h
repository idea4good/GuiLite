#ifndef GUILITE_WIDGETS_INCLUDE_DIALOG_H
#define GUILITE_WIDGETS_INCLUDE_DIALOG_H

#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/display.h"
#include "../core_include/resource.h"
#include "../core_include/word.h"
#include "../core_include/theme.h"

class c_surface;
class c_dialog;

typedef struct
{
	c_dialog* 	dialog;
	c_surface*	surface;
} DIALOG_ARRAY;

class c_dialog : public c_wnd
{
public:
	static int open_dialog(c_dialog* p_dlg, bool modal_mode = true)
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

		if (cur_dlg)
		{
			cur_dlg->set_attr(WND_ATTRIBUTION(0));
		}

		p_dlg->set_attr(modal_mode ? (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_PRIORITY) : (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS));
		p_dlg->show_window();
		p_dlg->set_me_the_dialog();
		return 1;
	}

	static int close_dialog(c_surface* surface)
	{
		c_dialog* dlg = get_the_dialog(surface);

		if (0 == dlg)
		{
			return 0;
		}
		c_rect rc;
		dlg->get_screen_rect(rc);
		dlg->set_attr(WND_ATTRIBUTION(0));
		surface->show_layer(rc, dlg->m_z_order -  1);

		//clear the dialog
		for (int i = 0; i < SURFACE_CNT_MAX; i++)
		{
			if (ms_the_dialogs[i].surface == surface)
			{
				ms_the_dialogs[i].dialog = 0;
				return 1;
			}
		}
		ASSERT(false);
		return -1;
	}

	static c_dialog* get_the_dialog(c_surface* surface)
	{
		for (int i = 0; i < SURFACE_CNT_MAX; i++)
		{
			if (ms_the_dialogs[i].surface == surface)
			{
				return ms_the_dialogs[i].dialog;
			}
		}
		return 0;
	}
protected:
	virtual void pre_create_wnd()
	{
		m_attr = WND_ATTRIBUTION(0);// no focus/visible
		m_z_order = Z_ORDER_LEVEL_1;
		m_bg_color = GL_RGB(33, 42, 53);
	}
	virtual void on_paint()
	{
		c_rect rect;
		get_screen_rect(rect);
		m_surface->fill_rect(rect, m_bg_color, m_z_order);

		if (m_str)
		{
			c_word::draw_string(m_surface, m_z_order, m_str, rect.m_left + 35, rect.m_top, c_theme::get_font(FONT_DEFAULT), GL_RGB(255, 255, 255), GL_ARGB(0, 0, 0, 0), ALIGN_LEFT);
		}
	}
private:
	int set_me_the_dialog()
	{
		c_surface* surface = get_surface();
		for (int i = 0; i < SURFACE_CNT_MAX; i++)
		{
			if (ms_the_dialogs[i].surface == surface)
			{
				ms_the_dialogs[i].dialog = this;
				return 0;
			}
		}

		for (int i = 0; i < SURFACE_CNT_MAX; i++)
		{
			if (ms_the_dialogs[i].surface == 0)
			{
				ms_the_dialogs[i].dialog = this;
				ms_the_dialogs[i].surface = surface;
				return 1;
			}
		}
		ASSERT(false);
		return -2;
	}
	static DIALOG_ARRAY ms_the_dialogs[SURFACE_CNT_MAX];
};

#endif
