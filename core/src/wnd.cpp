#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/api.h"
#include "../core_include/resource_type.h"
#include "../core_include/bitmap.h"
#include "../core_include/surface.h"
#include "../core_include/wnd.h"

c_wnd::c_wnd(): m_status(STATUS_NORMAL), m_style(GLT_ATTR_VISIBLE), m_parent(NULL), m_top_child(NULL), m_prev_sibling(NULL), m_next_sibling(NULL),
	m_str_id(0),m_bitmap(NULL), m_bitmap_focus(NULL), m_bitmap_pushed(NULL), m_bitmap_disable(NULL),
	m_font_type(NULL), m_font_color(0), m_bg_color(0), m_is_visible_now(false),m_resource_id(0),
	m_z_order(Z_ORDER_LEVEL_0),	m_active_child(NULL), m_surface(NULL)
{
	m_wnd_rect.Empty();
}

c_wnd:: ~c_wnd()
{
}

void c_wnd::pre_create_wnd()
{
	m_style = GLT_ATTR_VISIBLE | GLT_ATTR_FOCUS;
}

int c_wnd::connect(c_wnd *parent, unsigned short resource_id, unsigned short str_id,
		   short x, short y, short width, short height, WND_TREE* p_child_tree )
{
	if(0 == resource_id)
	{
		ASSERT(FALSE);
		return -1;
	}

	m_resource_id = resource_id;
	m_parent  = parent;
	m_status = STATUS_NORMAL;

	if (parent)
	{
		m_z_order = parent->m_z_order;
		m_surface = parent->m_surface;
	}
	if(NULL == m_surface)
	{
		ASSERT(FALSE);
		return -2;
	}

	/* (cs.x = x * 1024 / 768) for 1027*768=>800*600 quickly*/
	m_wnd_rect.m_left	= x;
	m_wnd_rect.m_top    = y;
	m_wnd_rect.m_right	= (x + width - 1);
	m_wnd_rect.m_bottom = (y + height - 1);

	c_rect rect;
	get_screen_rect(rect);
	ASSERT(m_surface->is_valid(rect));

	pre_create_wnd();
	set_str_id(str_id);

	if ( 0 != parent )
	{
		parent->add_child_2_tail(this);
	}

	if (load_child_wnd(p_child_tree) >= 0)
	{
		load_cmd_msg();
		on_init_children();
	}
	return 0;
}

int c_wnd::load_child_wnd(WND_TREE *p_child_tree)
{
	if (NULL == p_child_tree)
	{
		return 0;
	}
	int sum = 0;

	WND_TREE* p_cur = p_child_tree;
	while(p_cur->p_wnd)
	{
		if (0 != p_cur->p_wnd->m_resource_id)
		{//This wnd has been used! Do not share!
			ASSERT(FALSE);
			return -1;
		}
		else
		{
			p_cur->p_wnd->connect(this, p_cur->resource_id, p_cur->caption_id,
				p_cur->x, p_cur->y, p_cur->width, p_cur->height,p_cur->p_child_tree);
		}
		p_cur++;
		sum++;
	}
	return sum;
}

c_wnd* c_wnd::connect_clone(c_wnd *parent, unsigned short resource_id, unsigned short str_id,
		   short x, short y, short width, short height, WND_TREE* p_child_tree )
{
	if(0 == resource_id)
	{
		ASSERT(FALSE);
		return NULL;
	}

	c_wnd* wnd = clone();
	wnd->m_resource_id = resource_id;
	wnd->m_parent  = parent;
	wnd->m_status = STATUS_NORMAL;

	if (parent)
	{
		wnd->m_z_order =  parent->m_z_order;
		wnd->m_surface = parent->m_surface;
	}
	else
	{
		wnd->m_surface = m_surface;
	}
	if(NULL == wnd->m_surface)
	{
		ASSERT(FALSE);
		return NULL;
	}

	/* (cs.x = x * 1024 / 768) for 1027*768=>800*600 quickly*/
	wnd->m_wnd_rect.m_left   = x;
	wnd->m_wnd_rect.m_top    = y;
	wnd->m_wnd_rect.m_right  = (x + width - 1);
	wnd->m_wnd_rect.m_bottom = (y + height - 1);

	c_rect rect;
	wnd->get_screen_rect(rect);
	ASSERT(wnd->m_surface->is_valid(rect));

	wnd->pre_create_wnd();
	wnd->set_str_id(str_id);

	if ( 0 != parent )
	{
		parent->add_child_2_tail(wnd);
	}

	if (wnd->load_clone_child_wnd(p_child_tree) >= 0)
	{
		wnd->load_cmd_msg();
		wnd->on_init_children();
	}
	return wnd;
}

int c_wnd::load_clone_child_wnd(WND_TREE *p_child_tree)
{
	if (NULL == p_child_tree)
	{
		return 0;
	}
	int sum = 0;

	WND_TREE* p_cur = p_child_tree;
	while(p_cur->p_wnd)
	{
		if (0 != p_cur->p_wnd->m_resource_id)
		{//This wnd has been used! Do not share!
			ASSERT(FALSE);
			return -1;
		}
		else
		{
			p_cur->p_wnd->connect_clone(this, p_cur->resource_id, p_cur->caption_id,
				p_cur->x, p_cur->y, p_cur->width, p_cur->height,p_cur->p_child_tree);
		}
		p_cur++;
		sum++;
	}
	return sum;
}

void c_wnd::disconnect()
{
	if (0 == m_resource_id)
	{
		return;
	}

	if (NULL != m_top_child)
	{
		c_wnd *child = m_top_child;
		c_wnd *next_child = NULL;

		while (child)
		{
			next_child = child->m_next_sibling;
			child->disconnect();
			child = next_child;
		}
	}

	if (0 != m_parent)
	{
		m_parent->unlink_child(this);
	}
	m_active_child = 0;
	m_is_visible_now = false;
	m_resource_id = 0;
}

c_wnd* c_wnd::get_wnd_ptr(unsigned short id) const
{
	c_wnd *child = m_top_child;

	while ( child )
	{
		if ( child->get_id() == id )
		{
			break;
		}

		child = child->m_next_sibling;
	}

	return child;
}

void c_wnd::modify_style(unsigned int add_style, unsigned int remove_style)
{
	m_style &= ~remove_style;
	m_style |= add_style;

	if ( GLT_ATTR_DISABLED == (m_style & GLT_ATTR_DISABLED) )
	{
		m_status = STATUS_DISABLED;
	}
	else
	{
		if (m_status == STATUS_DISABLED)
		{
			m_status = STATUS_NORMAL;
		}
	}
}

bool c_wnd::is_foreground()
{
	return (m_surface->is_active() && m_is_visible_now);
}

void c_wnd::set_visible(bool visible)
{
	m_is_visible_now = visible;

	c_wnd *child = m_top_child;
	while (child)
	{
		child->set_visible(visible);
		child = child->m_next_sibling;
	}
}

void c_wnd::enable_wnd(int enable)
{
	int old_enable = is_wnd_enable();        

	if ( enable != old_enable )
	{
		if ( enable )
		{
			m_style &= ~GLT_ATTR_DISABLED;
			m_status = STATUS_NORMAL;
		}
		else
		{
			m_style |= GLT_ATTR_DISABLED;
			m_status = STATUS_DISABLED;
		}

		if ( 0 != m_top_child )
		{
			c_wnd *child = m_top_child;
			c_wnd *next_child = 0;

			while ( child )
			{
				next_child = child->m_next_sibling;

				child->enable_wnd(enable);
				child = next_child;
			}
		}

		if ( true == m_is_visible_now )
		{
			on_paint();
		}
	}
}

int c_wnd::is_wnd_enable() const
{
	if ( GLT_ATTR_DISABLED == (m_style & GLT_ATTR_DISABLED) )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void c_wnd::enable_focus(int enable)
{
	if (enable)
	{
		m_style |= GLT_ATTR_FOCUS;
	}
	else
	{
		m_style &= ~GLT_ATTR_FOCUS;
	}
	return;
}

int c_wnd::is_focus_wnd() const
{
	if ( (m_style & GLT_ATTR_VISIBLE)
		&& !(m_style & GLT_ATTR_DISABLED)
		&& (m_style & GLT_ATTR_FOCUS)
		&& (true == m_is_visible_now))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

const int c_wnd::is_active_wnd() const
{
	if(!m_parent)
	{
		return TRUE;
	}
	else
	{
		if( this == m_parent->get_active_child() )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

void c_wnd::set_wnd_pos(short x, short y, short width, short height)
{
	m_wnd_rect.m_left   = x;
	m_wnd_rect.m_top    = y;
	m_wnd_rect.m_right  = x + width - 1;
	m_wnd_rect.m_bottom = y + height - 1;
}

void c_wnd::get_wnd_rect(c_rect &rect) const
{
	rect = m_wnd_rect;
}

void c_wnd::get_screen_rect(c_rect &rect) const
{
	rect.SetRect(0, 0, (m_wnd_rect.Width() - 1), (m_wnd_rect.Height() - 1));
	wnd2screen(rect);
}

void c_wnd::wnd2screen(int &x, int &y) const
{
	c_wnd *parent = m_parent;
	c_rect rect;

	x += m_wnd_rect.m_left;
	y += m_wnd_rect.m_top;

	while ( NULL != parent )
	{
		parent->get_wnd_rect(rect);
		x += rect.m_left;
		y += rect.m_top;

		parent = parent->get_parent();
	}
}

void c_wnd::wnd2screen(c_rect &rect) const
{
	int l, t, r, b;

	l = rect.m_left;
	t = rect.m_top;
	wnd2screen(l, t);

	r = (l + rect.Width() - 1);
	b = (t + rect.Height() - 1);

	rect.SetRect(l, t, r, b);
}

void c_wnd::screen2wnd(short &x, short &y) const
{
	c_wnd *parent = m_parent;
	c_rect rect;

	x -= m_wnd_rect.m_left;
	y -= m_wnd_rect.m_top;

	while ( NULL != parent )
	{
		parent->get_wnd_rect(rect);
		x -= rect.m_left;
		y -= rect.m_top;
		parent = parent->get_parent();
	}
}

void c_wnd::screen2wnd(c_rect &rect) const
{
	short l, t, r, b;

	l = rect.m_left;
	t = rect.m_top;
	screen2wnd(l, t);

	r = l + rect.Width() - 1;
	b = t + rect.Height() - 1;

	rect.SetRect(l, t, r, b);
}

void c_wnd::on_focus()
{
	if (m_active_child)
	{
		m_active_child->on_focus();
	}
}

void c_wnd::on_kill_focus()
{
	if (m_active_child)
	{
		m_active_child->on_kill_focus();
		m_active_child = 0;
	}
}

c_wnd* c_wnd::set_focus(c_wnd * new_active_child)
{
	ASSERT(NULL != new_active_child);
	ASSERT(new_active_child->get_parent() == this);

	c_wnd *old_focus_child = m_active_child;
	if (new_active_child->is_focus_wnd())
	{
		if ( new_active_child != old_focus_child )
		{
			if (0 != old_focus_child)
			{
				old_focus_child->on_kill_focus();
			}
			m_active_child = new_active_child;

			if (get_parent())
			{
				get_parent()->set_focus(this);
			}
			m_active_child->on_focus();
		}
	}
	return m_active_child;
}

int c_wnd::on_notify(unsigned short notify_code, unsigned short ctrl_id, long l_param)
{
	const GLT_MSG_ENTRY *entry = FindMsgEntry(GetMSgEntries(), MSG_TYPE_WND, notify_code, ctrl_id);
	if ( NULL != entry )
	{
		MSGFUNCS msg_funcs;
		msg_funcs.func = entry->func;

		switch ( entry->callbackType)
		{
		case MSG_CALLBACK_VV:
			(this->*msg_funcs.func)();
			break;
		case MSG_CALLBACK_VVL:
			(this->*msg_funcs.func_vvl)(l_param);
			break;
		case MSG_CALLBACK_VWV:
			(this->*msg_funcs.func_vwv)(ctrl_id);
			break;
		case MSG_CALLBACK_VWL:
			(this->*msg_funcs.func_vwl)(ctrl_id, l_param);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

void c_wnd::add_child_2_head(c_wnd *child)
{
	if( NULL == child )return;
	if(child == get_wnd_ptr(child->m_resource_id))return;

	if ( NULL == m_top_child )
	{
		m_top_child = child;
		child->m_prev_sibling = NULL;
		child->m_next_sibling = NULL;
	}
	else
	{
		child->m_next_sibling = m_top_child;
		child->m_prev_sibling = NULL;
		m_top_child->m_prev_sibling = child;
		m_top_child = child;
	}
}

void c_wnd::add_child_2_tail(c_wnd *child)
{
	if( NULL == child )return;
	if(child == get_wnd_ptr(child->m_resource_id))return;

	if ( NULL == m_top_child )
	{
		m_top_child = child;
		child->m_prev_sibling = NULL;
		child->m_next_sibling = NULL;
	}
	else
	{
		c_wnd *last_child = get_last_child();
		if (NULL == last_child)
		{
			ASSERT(FALSE);
		}
		last_child->m_next_sibling = child;
		child->m_prev_sibling = last_child;
		child->m_next_sibling = NULL;
	}
}

c_wnd* c_wnd::get_last_child() const
{
	if ( NULL == m_top_child )
	{
		return NULL;
	}

	c_wnd *child = m_top_child;

	while ( child->m_next_sibling)
	{
		child = child->m_next_sibling;
	}

	return child;
}

int	c_wnd::unlink_child(c_wnd *child)
{
	if ((NULL == child)
		|| (this != child->get_parent()))
	{
		return -1;
	}

	if (NULL == m_top_child)
	{
		return -2;
	}

	int find = FALSE;

	c_wnd *tmp_child = m_top_child;
	if (tmp_child == child)
	{
		m_top_child = child->m_next_sibling;
		if (NULL != child->m_next_sibling)
		{
			child->m_next_sibling->m_prev_sibling = NULL;
		}

		find = TRUE;
	}
	else
	{
		while (tmp_child->m_next_sibling)
		{
			if (child == tmp_child->m_next_sibling)
			{
				tmp_child->m_next_sibling = child->m_next_sibling;
				if (NULL != child->m_next_sibling)
				{
					child->m_next_sibling->m_prev_sibling = tmp_child;
				}

				find = TRUE;
				break;
			}

			tmp_child = tmp_child->m_next_sibling;
		}
	}

	if (TRUE == find)
	{
		if (m_active_child == child)
		{
			m_active_child = NULL;
		}

		child->m_next_sibling = NULL;
		child->m_prev_sibling = NULL;
		return 1;
	}
	else
	{
		return 0;
	}
}

void c_wnd::display_window()
{
	if (GLT_ATTR_VISIBLE == (m_style & GLT_ATTR_VISIBLE))
	{
		m_is_visible_now = true;
		on_paint();

		c_wnd *child = m_top_child;
		if ( NULL != child )
		{
			while ( child )
			{
				child->show_window(GLT_WIN_SHOW);
				child = child->m_next_sibling;
			}
		}
	}
}

void c_wnd::hide_widow()
{
	c_wnd *child = m_top_child;

	if ( true == m_is_visible_now )
	{
		if ( NULL != child )
		{
			while ( child )
			{
				child->show_window(GLT_WIN_HIDE);
				child = child->m_next_sibling;
			}
		}
		m_is_visible_now = false;
		if (m_parent && (m_parent->get_active_child() == this))
		{
			m_parent->on_kill_focus();
		}
	}
}

void c_wnd::show_window(int show_type)
{    
	if ( GLT_WIN_SHOW == show_type )
	{
		display_window();
	}
	else if (GLT_WIN_HIDE == show_type)
	{
		hide_widow();
	}
}

void c_wnd::on_touch_down(int x, int y)
{
	c_rect rect;
	x -= m_wnd_rect.m_left;
	y -= m_wnd_rect.m_top;
	c_wnd *pChild = m_top_child;

	if ( pChild != NULL )
	{
		while ( pChild )
		{
			if (pChild->is_visible())
			{
				pChild->get_wnd_rect(rect);
				if ( TRUE == rect.PtInRect(x, y) )
				{
					if ( TRUE == pChild->is_focus_wnd() )
					{
						pChild->on_touch_down(x, y);
					}
				}
			}
			pChild = pChild->get_next_sibling();
		}
	}
}

void c_wnd::on_touch_up(int x, int y)
{
	c_rect rect;
	x -= m_wnd_rect.m_left;
	y -= m_wnd_rect.m_top;
	c_wnd *pChild = m_top_child;

	if ( pChild != NULL )
	{
		while ( pChild )
		{
			if (pChild->is_visible())
			{
				pChild->get_wnd_rect(rect);
				if ( TRUE == rect.PtInRect(x, y) )
				{
					if ( TRUE == pChild->is_focus_wnd() )
					{
						pChild->on_touch_up(x, y);
					}
				}
			}
			pChild = pChild->get_next_sibling();
		}
	}
}

void c_wnd::notify_parent(unsigned short msg_id, unsigned int w_param, long l_param)
{
	if (m_parent)
	{
		m_parent->on_notify(msg_id, w_param, l_param);
	}
}

void c_wnd::set_pixel(int x, int y, unsigned int rgb)
{
	m_surface->set_pixel(x, y, rgb, m_z_order);
}

void c_wnd::draw_hline(int x0, int x1, int y, unsigned int rgb)
{
	m_surface->draw_hline(x0, x1, y, rgb, m_z_order);
}

void c_wnd::draw_vline(int x, int y0, int y1, unsigned int rgb)
{
	m_surface->draw_vline(x, y0, y1, rgb, m_z_order);
}

void c_wnd::draw_line(int x0, int y0, int x1, int y1, unsigned int rgb)
{
	m_surface->draw_line(x0, y0, x1, y1, rgb, m_z_order);
}

void c_wnd::draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb)
{
	m_surface->draw_rect(x0, y0, x1, y1, rgb, m_z_order);
}

void c_wnd::draw_rect(c_rect rect, unsigned int rgb)
{
	m_surface->draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, m_z_order);
}

void c_wnd::fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb)
{
	m_surface->fill_rect(x0, y0, x1, y1, rgb, m_z_order);
}

void c_wnd::fill_rect(c_rect rect, unsigned int rgb)
{
	m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, m_z_order);
}

void c_wnd::draw_custom_shape(int l, int t, int r, int b, unsigned int color, const CUSTOM_SHAPE pRgn[])
{
	m_surface->draw_custom_shape(l, t, r, b, color, pRgn, m_z_order);
}
