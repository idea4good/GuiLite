#ifndef GUILITE_WIDGETS_INCLUDE_SLIDE_GROUP_H
#define GUILITE_WIDGETS_INCLUDE_SLIDE_GROUP_H

#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/display.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../widgets_include/dialog.h"
#include <stdlib.h>

#define MAX_PAGES	5
class c_gesture;
class c_slide_group : public c_wnd {
public:
	inline c_slide_group();

	int set_active_slide(int index, bool is_redraw = true)
	{
		if (index >= MAX_PAGES || index < 0)
		{
			return -1;
		}
		if (0 == m_slides[index])
		{
			return -2;
		}
		m_active_slide_index = index;

		for (int i = 0; i < MAX_PAGES; i++)
		{
			if (m_slides[i] == 0)
			{
				continue;
			}
			if (i == index)
			{
				m_slides[i]->get_surface()->set_active(true);
				add_child_2_tail(m_slides[i]);
				if (is_redraw)
				{
					c_rect rc;
					get_screen_rect(rc);
					m_slides[i]->get_surface()->flush_screen(rc.m_left, rc.m_top, rc.m_right, rc.m_bottom);
				}
			}
			else
			{
				m_slides[i]->get_surface()->set_active(false);
			}
		}
		return 0;
	}
	c_wnd* get_slide(int index){return m_slides[index];}
	c_wnd* get_active_slide(){return m_slides[m_active_slide_index];}
	int get_active_slide_index(){return m_active_slide_index;}
	int add_slide(c_wnd* slide, unsigned short resource_id, short x, short y, short width, short height, WND_TREE* p_child_tree = 0, Z_ORDER_LEVEL max_zorder =  Z_ORDER_LEVEL_0)
	{
		if (0 == slide)
		{
			return -1;
		}

		c_surface* old_surface = get_surface();
		c_surface* new_surface = old_surface->get_display()->alloc_surface(max_zorder);
		new_surface->set_active(false);
		set_surface(new_surface);
		slide->connect(this, resource_id, 0, x, y, width, height, p_child_tree);
		set_surface(old_surface);

		int i = 0;
		while (i < MAX_PAGES)
		{
			if (m_slides[i] == slide)
			{//slide has lived
				ASSERT(false);
				return -2;
			}
			i++;
		}

		//new slide
		i = 0;
		while (i < MAX_PAGES)
		{
			if (m_slides[i] == 0)
			{
				m_slides[i] = slide;
				slide->show_window();
				return 0;
			}
			i++;
		}

		//no more slide can be add
		ASSERT(false);
		return -3;
	}
	void disabel_all_slide()
	{
		for (int i = 0; i < MAX_PAGES; i++)
		{
			if (m_slides[i])
			{
				m_slides[i]->get_surface()->set_active(false);
			}
		}
	}
	inline virtual void on_touch(int x, int y, TOUCH_ACTION action);
	virtual void on_navigate(NAVIGATION_KEY key)
	{
		if (m_slides[m_active_slide_index])
		{
			m_slides[m_active_slide_index]->on_navigate(key);
		}
	}
protected:
	c_wnd* m_slides[MAX_PAGES];
	int m_active_slide_index;
	c_gesture* m_gesture;
};

//#define SWIPE_STEP			300//for arm
#define SWIPE_STEP			10//for PC & ANDROID
#define MOVE_THRESHOLD		10

typedef enum {
	TOUCH_MOVE,
	TOUCH_IDLE
}TOUCH_STATE;

class c_slide_group;
class c_gesture {
public:
	c_gesture(c_slide_group* group)
	{
		m_slide_group = group;
		m_state = TOUCH_IDLE;
		m_down_x = m_down_y = m_move_x = m_move_y = 0;
	}
	bool handle_swipe(int x, int y, TOUCH_ACTION action)
	{
		if (action == TOUCH_DOWN)//MOUSE_LBUTTONDOWN
		{
			if (m_state == TOUCH_IDLE)
			{
				m_state = TOUCH_MOVE;
				m_move_x = m_down_x = x;
				return true;
			}
			else//TOUCH_MOVE
			{
				return on_move(x);
			}
		}
		else if (action == TOUCH_UP)//MOUSE_LBUTTONUP
		{
			if (m_state == TOUCH_MOVE)
			{
				m_state = TOUCH_IDLE;
				return on_swipe(x);
			}
			else
			{
				return false;
				//ASSERT(false);
			}
		}
		return true;
	}

private:
	bool on_move(int x)
	{
		if (m_slide_group == 0)
		{
			return true;
		}
		if (abs(x - m_move_x) < MOVE_THRESHOLD)
		{
			return false;
		}

		m_slide_group->disabel_all_slide();
		m_move_x = x;
		if ((m_move_x - m_down_x) > 0)
		{
			move_right();
		}
		else
		{
			move_left();
		}
		return false;
	}
	bool on_swipe(int x)
	{
		if (m_slide_group == 0)
		{
			return true;
		}
		if ((m_down_x == m_move_x) && (abs(x - m_down_x) < MOVE_THRESHOLD))
		{
			return true;
		}

		m_slide_group->disabel_all_slide();
		int page = -1;
		m_move_x = x;
		if ((m_move_x - m_down_x) > 0)
		{
			page = swipe_right();
		}
		else
		{
			page = swipe_left();
		}
		if (page >= 0)
		{
			m_slide_group->set_active_slide(page);
		}
		else
		{
			m_slide_group->set_active_slide(m_slide_group->get_active_slide_index(), false);
		}
		return false;
	}
	int swipe_left()
	{
		if (m_slide_group == 0)
		{
			return -1;
		}
		int index = m_slide_group->get_active_slide_index();
		if ((index + 1) >= MAX_PAGES ||
			m_slide_group->get_slide(index + 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
		{
			return -2;
		}
		c_surface* s1 = m_slide_group->get_slide(index + 1)->get_surface();
		c_surface * s2 = m_slide_group->get_slide(index)->get_surface();
		if (s1->get_display() != s2->get_display())
		{
			return -3;
		}

		int step = m_down_x - m_move_x;
		c_rect rc;
		m_slide_group->get_screen_rect(rc);
		while (step < rc.Width())
		{
			s1->get_display()->swipe_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, step);
			step += SWIPE_STEP;
		}
		if (step != rc.Width())
		{
			s1->get_display()->swipe_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, rc.Width());
		}
		return (index + 1);
	}

	int swipe_right()
	{
		if (m_slide_group == 0)
		{
			return -1;
		}
		int index = m_slide_group->get_active_slide_index();
		if (index <= 0 ||
			m_slide_group->get_slide(index - 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
		{
			return -2;
		}
		c_surface* s1 = m_slide_group->get_slide(index - 1)->get_surface();
		c_surface * s2 = m_slide_group->get_slide(index)->get_surface();
		if (s1->get_display() != s2->get_display())
		{
			return -3;
		}

		c_rect rc;
		m_slide_group->get_screen_rect(rc);
		int step = rc.Width() - (m_move_x - m_down_x);
		while (step > 0)
		{
			s1->get_display()->swipe_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, step);
			step -= SWIPE_STEP;
		}
		if (step != 0)
		{
			s1->get_display()->swipe_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, 0);
		}
		return (index - 1);
	}
	void move_left()
	{
		int index = m_slide_group->get_active_slide_index();
		if ((index + 1) >= MAX_PAGES ||
			m_slide_group->get_slide(index + 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
		{
			return;
		}
		c_surface* s1 = m_slide_group->get_slide(index + 1)->get_surface();
		c_surface * s2 = m_slide_group->get_slide(index)->get_surface();
		c_rect rc;
		m_slide_group->get_screen_rect(rc);
		if (s1->get_display() == s2->get_display())
		{
			s1->get_display()->swipe_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, (m_down_x - m_move_x));
		}
	}
	void move_right()
	{
		int index = m_slide_group->get_active_slide_index();
		if (index <= 0 ||
			m_slide_group->get_slide(index - 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
		{
			return;
		}
		c_surface* s1 = m_slide_group->get_slide(index - 1)->get_surface();
		c_surface * s2 = m_slide_group->get_slide(index)->get_surface();
		c_rect rc;
		m_slide_group->get_screen_rect(rc);
		if (s1->get_display() == s2->get_display())
		{
			s1->get_display()->swipe_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, (rc.Width() - (m_move_x - m_down_x)));
		}
	}

	int m_down_x;
	int m_down_y;
	int m_move_x;
	int m_move_y;
	TOUCH_STATE m_state;
	c_slide_group* m_slide_group;
};

inline c_slide_group::c_slide_group()
{
	m_gesture = new c_gesture(this);
	for (int i = 0; i < MAX_PAGES; i++)
	{
		m_slides[i] = 0;
	}
	m_active_slide_index = 0;
}

inline void c_slide_group::on_touch(int x, int y, TOUCH_ACTION action)
{
	x -= m_wnd_rect.m_left;
	y -= m_wnd_rect.m_top;

	if (m_gesture->handle_swipe(x, y, action))
	{
		if (m_slides[m_active_slide_index])
		{
			m_slides[m_active_slide_index]->on_touch(x, y, action);
		}
	}
}
#endif
