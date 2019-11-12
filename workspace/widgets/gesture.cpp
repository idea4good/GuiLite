#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/surface.h"
#include "../core_include/display.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../widgets_include/gesture.h"
#include "../widgets_include/slide_group.h"
#include <stdlib.h>

//#define SWIPE_STEP			300//for arm
#define SWIPE_STEP			10//for PC & ANDROID
#define MOVE_THRESHOLD		10

c_gesture::c_gesture(c_slide_group* group)
{
	m_slide_group = group;
	m_state = TOUCH_IDLE;
	m_down_x = m_down_y = m_move_x = m_move_y = 0;
}

bool c_gesture::handle_swipe(int x, int y, TOUCH_ACTION action)
{
	if(action == TOUCH_DOWN)//MOUSE_LBUTTONDOWN
	{
		if(m_state == TOUCH_IDLE)
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
	else if(action == TOUCH_UP)//MOUSE_LBUTTONUP
	{
		if(m_state == TOUCH_MOVE)
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

bool c_gesture::on_move(int x)
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

bool c_gesture::on_swipe(int x)
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

int c_gesture::swipe_left()
{
	if (m_slide_group == 0)
	{
		return -1;
	}
	int index = m_slide_group->get_active_slide_index();
	if((index + 1) >= MAX_PAGES ||
		m_slide_group->get_slide(index + 1) == 0 ||
		m_slide_group->get_slide(index) == 0)
	{
		return -2;
	}
	c_surface* s1 = m_slide_group->get_slide(index + 1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	if (s1->get_display() != s2->get_display())
	{
		return -3;
	}

	int step = m_down_x - m_move_x;
	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	while(step < rc.Width())
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

int c_gesture::swipe_right()
{
	if (m_slide_group == 0)
	{
		return -1;
	}
	int index = m_slide_group->get_active_slide_index();
	if(index <= 0 ||
			m_slide_group->get_slide(index - 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
	{
		return -2;
	}
	c_surface* s1 = m_slide_group->get_slide(index -1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	if (s1->get_display() != s2->get_display())
	{
		return -3;
	}

	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	int step = rc.Width() - (m_move_x - m_down_x);
	while(step > 0)
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

void c_gesture::move_left()
{
	int index = m_slide_group->get_active_slide_index();
	if((index + 1) >= MAX_PAGES ||
			m_slide_group->get_slide(index + 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
	{
		return;
	}
	c_surface* s1 = m_slide_group->get_slide(index +1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	if(s1->get_display() == s2->get_display())
	{
		s1->get_display()->swipe_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, (m_down_x - m_move_x));
	}
}

void c_gesture::move_right()
{
	int index = m_slide_group->get_active_slide_index();
	if(index <= 0 ||
			m_slide_group->get_slide(index - 1) == 0 ||
			m_slide_group->get_slide(index) == 0)
	{
		return;
	}
	c_surface* s1 = m_slide_group->get_slide(index -1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	if(s1->get_display() == s2->get_display())
	{
		s1->get_display()->swipe_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, (rc.Width() - (m_move_x - m_down_x)));
	}
}
