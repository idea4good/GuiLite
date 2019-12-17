#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/resource.h"
#include "../core_include/bitmap.h"
#include "../core_include/surface.h"
#include "../core_include/wnd.h"

c_wnd::c_wnd(): m_status(STATUS_NORMAL), m_attr(ATTR_VISIBLE), m_parent(0), m_top_child(0), m_prev_sibling(0), m_next_sibling(0),
m_str(0), m_font_color(0), m_bg_color(0), m_id(0),	m_z_order(Z_ORDER_LEVEL_0),	m_focus_child(0), m_surface(0)
{
	m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
}

int c_wnd::connect(c_wnd *parent, unsigned short resource_id, const char* str,
		   short x, short y, short width, short height, WND_TREE* p_child_tree )
{
	if(0 == resource_id)
	{
		ASSERT(false);
		return -1;
	}

	m_id = resource_id;
	set_str(str);
	m_parent  = parent;
	m_status = STATUS_NORMAL;

	if (parent)
	{
		m_z_order = parent->m_z_order;
		m_surface = parent->m_surface;
	}
	if(0 == m_surface)
	{
		ASSERT(false);
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
	if (0 == p_child_tree)
	{
		return 0;
	}
	int sum = 0;

	WND_TREE* p_cur = p_child_tree;
	while(p_cur->p_wnd)
	{
		if (0 != p_cur->p_wnd->m_id)
		{//This wnd has been used! Do not share!
			ASSERT(false);
			return -1;
		}
		else
		{
			p_cur->p_wnd->connect(this, p_cur->resource_id, p_cur->str,
				p_cur->x, p_cur->y, p_cur->width, p_cur->height,p_cur->p_child_tree);
		}
		p_cur++;
		sum++;
	}
	return sum;
}

c_wnd* c_wnd::connect_clone(c_wnd *parent, unsigned short resource_id, const char* str,
		   short x, short y, short width, short height, WND_TREE* p_child_tree )
{
	if(0 == resource_id)
	{
		ASSERT(false);
		return 0;
	}

	c_wnd* wnd = clone();
	wnd->m_id = resource_id;
	wnd->set_str(str);
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
	if(0 == wnd->m_surface)
	{
		ASSERT(false);
		return 0;
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
	if (0 == p_child_tree)
	{
		return 0;
	}
	int sum = 0;

	WND_TREE* p_cur = p_child_tree;
	while(p_cur->p_wnd)
	{
		p_cur->p_wnd->connect_clone(this, p_cur->resource_id, p_cur->str,
									p_cur->x, p_cur->y, p_cur->width, p_cur->height,p_cur->p_child_tree);
		p_cur++;
		sum++;
	}
	return sum;
}

void c_wnd::disconnect()
{
	if (0 == m_id)
	{
		return;
	}

	if (0 != m_top_child)
	{
		c_wnd *child = m_top_child;
		c_wnd *next_child = 0;

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
	m_focus_child = 0;
	m_id = 0;
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

void c_wnd::set_attr(WND_ATTRIBUTION attr)
{
	m_attr = attr;

	if ( ATTR_DISABLED == (attr & ATTR_DISABLED) )
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

bool c_wnd::is_focus_wnd() const
{
	if ( (m_attr & ATTR_VISIBLE)
		&& !(m_attr & ATTR_DISABLED)
		&& (m_attr & ATTR_FOCUS))
	{
		return true;
	}
	else
	{
		return false;
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

	while ( 0 != parent )
	{
		parent->get_wnd_rect(rect);
		x += rect.m_left;
		y += rect.m_top;

		parent = parent->m_parent;
	}
}

void c_wnd::wnd2screen(c_rect &rect) const
{
	int l = rect.m_left;
	int t = rect.m_top;
	wnd2screen(l, t);

	int r = (l + rect.Width() - 1);
	int b = (t + rect.Height() - 1);
	rect.SetRect(l, t, r, b);
}

c_wnd* c_wnd::set_child_focus(c_wnd * focus_child)
{
	ASSERT(0 != focus_child);
	ASSERT(focus_child->m_parent == this);

	c_wnd *old_focus_child = m_focus_child;
	if (focus_child->is_focus_wnd())
	{
		if (focus_child != old_focus_child )
		{
			if (old_focus_child)
			{
				old_focus_child->on_kill_focus();
			}
			m_focus_child = focus_child;

			if (m_parent)
			{
				m_parent->set_child_focus(this);
			}
			m_focus_child->on_focus();
		}
	}
	return m_focus_child;
}

void c_wnd::add_child_2_tail(c_wnd *child)
{
	if( 0 == child )return;
	if(child == get_wnd_ptr(child->m_id))return;

	if ( 0 == m_top_child )
	{
		m_top_child = child;
		child->m_prev_sibling = 0;
		child->m_next_sibling = 0;
	}
	else
	{
		c_wnd *last_child = get_last_child();
		if (0 == last_child)
		{
			ASSERT(false);
		}
		last_child->m_next_sibling = child;
		child->m_prev_sibling = last_child;
		child->m_next_sibling = 0;
	}
}

c_wnd* c_wnd::get_last_child() const
{
	if ( 0 == m_top_child )
	{
		return 0;
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
	if ((0 == child)
		|| (this != child->m_parent))
	{
		return -1;
	}

	if (0 == m_top_child)
	{
		return -2;
	}

	bool find = false;

	c_wnd *tmp_child = m_top_child;
	if (tmp_child == child)
	{
		m_top_child = child->m_next_sibling;
		if (0 != child->m_next_sibling)
		{
			child->m_next_sibling->m_prev_sibling = 0;
		}

		find = true;
	}
	else
	{
		while (tmp_child->m_next_sibling)
		{
			if (child == tmp_child->m_next_sibling)
			{
				tmp_child->m_next_sibling = child->m_next_sibling;
				if (0 != child->m_next_sibling)
				{
					child->m_next_sibling->m_prev_sibling = tmp_child;
				}

				find = true;
				break;
			}

			tmp_child = tmp_child->m_next_sibling;
		}
	}

	if (true == find)
	{
		if (m_focus_child == child)
		{
			m_focus_child = 0;
		}

		child->m_next_sibling = 0;
		child->m_prev_sibling = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

void c_wnd::show_window()
{
	if (ATTR_VISIBLE == (m_attr & ATTR_VISIBLE))
	{
		on_paint();
		c_wnd *child = m_top_child;
		if ( 0 != child )
		{
			while ( child )
			{
				child->show_window();
				child = child->m_next_sibling;
			}
		}
	}
}

bool c_wnd::on_touch(int x, int y, TOUCH_ACTION action)
{
	c_rect rect;
	x -= m_wnd_rect.m_left;
	y -= m_wnd_rect.m_top;
	c_wnd* child = m_top_child;

	c_wnd* target_wnd = 0;
	int target_z_order = Z_ORDER_LEVEL_0;

	while (child)
	{
		if (ATTR_VISIBLE == (child->m_attr & ATTR_VISIBLE))
		{
			child->get_wnd_rect(rect);
			if (true == rect.PtInRect(x, y) || child->m_attr & ATTR_MODAL)
			{
				if (true == child->is_focus_wnd())
				{
					if (child->m_z_order >= target_z_order)
					{
						target_wnd = child;
						target_z_order = child->m_z_order;
					}
				}
			}
		}
		child = child->m_next_sibling;
	}

	if (target_wnd == 0)
	{
		return false;
	}
	return target_wnd->on_touch(x, y, action);
}

bool c_wnd::on_key(KEY_TYPE key)
{
	ASSERT(key == KEY_FORWARD || key == KEY_BACKWARD || key == KEY_ENTER);

	// Find current focus wnd.
	c_wnd* old_focus_wnd = m_focus_child;
	while (m_focus_child && m_focus_child->m_focus_child)
	{
		old_focus_wnd = m_focus_child->m_focus_child;
	}
	if (old_focus_wnd && !old_focus_wnd->on_key(key))
	{
		return true;
	}

	// Default moving focus(Default handle KEY_FOWARD/KEY_BACKWARD)
	if (key == KEY_ENTER)
	{
		return true;
	}
	if (!old_focus_wnd)
	{// No current focus wnd, new one.
		c_wnd *child = m_top_child;
		c_wnd *new_focus_wnd = 0;
		while (child)
		{
			if (ATTR_VISIBLE == (child->m_attr & ATTR_VISIBLE))
			{
				if (true == child->is_focus_wnd())
				{
					new_focus_wnd = child;
					new_focus_wnd->m_parent->set_child_focus(new_focus_wnd);
					child = child->m_top_child;
					continue;
				}
			}
			child = child->m_next_sibling;
		}
		return true;
	}

	// Move focus from old wnd to next wnd
	c_wnd* next_focus_wnd = (key == KEY_FORWARD) ? old_focus_wnd->m_next_sibling : old_focus_wnd->m_prev_sibling;
	while (next_focus_wnd && (!next_focus_wnd->is_focus_wnd()))
	{// Search neighbor of old focus wnd
		next_focus_wnd = (key == KEY_FORWARD) ? next_focus_wnd->m_next_sibling : next_focus_wnd->m_prev_sibling;
	}
	if (!next_focus_wnd)
	{// Search whole brother wnd
		next_focus_wnd = (key == KEY_FORWARD) ? old_focus_wnd->m_parent->m_top_child : old_focus_wnd->m_parent->get_last_child();
		while (next_focus_wnd && (!next_focus_wnd->is_focus_wnd()))
		{
			next_focus_wnd = (key == KEY_FORWARD) ? next_focus_wnd->m_next_sibling : next_focus_wnd->m_prev_sibling;
		}
	}
	if (next_focus_wnd)
	{
		next_focus_wnd->m_parent->set_child_focus(next_focus_wnd);
	}
	return true;
}

void c_wnd::notify_parent(int msg_id, int param)
{
	if (!m_parent)
	{
		return;
	}
	const GL_MSG_ENTRY* entry = m_parent->find_msg_entry(m_parent->get_msg_entries(), MSG_TYPE_WND, msg_id);
	if (0 == entry)
	{
		return;
	}
	(m_parent->*(entry->callBack))(m_id, param);
}
