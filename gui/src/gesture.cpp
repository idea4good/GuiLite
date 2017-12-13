#include "core_include/api.h"
#include "core_include/msg.h"
#include "core_include/rect.h"
#include "core_include/surface.h"
#include "core_include/display.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "../gui_include/dialog.h"
#include "../gui_include/gesture.h"
#include "../gui_include/slide_group.h"
#include <stdlib.h>

//#define FLIP_STEP			300//for arm
#define FLIP_STEP			10//for PC & ANDROID
#define MOVE_THRESHOLD		10

c_gesture* c_gesture::ms_gesture[MAX_DISPLAY];

void* c_gesture::task(void* param)
{
	MSG_INFO msg;
	while(1)
	{
		if(c_hid_pipe::read_hid_msg() < 0)
		{
			continue;
		}

		for(int i = 0; i < MAX_DISPLAY; i++)
		{
			if(!ms_gesture[i])
			{
				continue;
			}

			msg = ms_gesture[i]->m_hid_pipe->m_msg;
			if(XXX == msg.dwMsgId)
			{
				continue;
			}

			if(ms_gesture[i]->handle_flip(msg))
			{
				ms_gesture[i]->handle_hid_msg(msg);
			}
		}
	}
	return 0;
}

static unsigned long s_pid;
c_gesture::c_gesture(c_wnd* root, c_slide_group* group,  c_hid_pipe* hid_pipe)
{
	m_root = root;
	m_slide_group = group;
	m_hid_pipe = hid_pipe;
	m_action = TOUCH_IDLE;
	m_down_x = m_down_y = m_move_x = m_move_y = 0;

	for(int i = 0; i < MAX_DISPLAY; i++)
	{
		if(!ms_gesture[i])
		{
			ms_gesture[i] = this;
			break;
		}
	}
	if(!s_pid)
	{
		create_thread(&s_pid, NULL, task, NULL);
	}
}

bool c_gesture::handle_flip(MSG_INFO &msg)
{
	int x = msg.dwParam1;

	if(msg.dwMsgId == 0x4700)//MOUSE_LBUTTONDOWN
	{
		if(m_action == TOUCH_IDLE)
		{
			m_action = TOUCH_MOVE;
			m_move_x = m_down_x = x;
			return true;
		}
		else//TOUCH_MOVE
		{
			return on_move(x);
		}
	}
	else if(msg.dwMsgId == 0x4600)//MOUSE_LBUTTONUP
	{
		if(m_action == TOUCH_MOVE)
		{
			m_action = TOUCH_IDLE;
			return on_flip(x);
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
	if (m_slide_group == NULL)
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

bool c_gesture::on_flip(int x)
{
	if (m_slide_group == NULL)
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
		page = flip_right();
	}
	else
	{
		page = flip_left();
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

int c_gesture::flip_left()
{
	if (m_slide_group == NULL)
	{
		return -1;
	}
	int index = m_slide_group->get_active_slide_index();
	if((index + 1) >= MAX_PAGES ||
		m_slide_group->get_slide(index + 1) == NULL ||
		m_slide_group->get_slide(index) == NULL)
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
	while(step <= rc.Width())
	{
		s1->get_display()->merge_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, step);
		step += FLIP_STEP;
	}
	if (step != rc.Width())
	{
		s1->get_display()->merge_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, rc.Width());
	}
	return (index + 1);
}

int c_gesture::flip_right()
{
	if (m_slide_group == NULL)
	{
		return -1;
	}
	int index = m_slide_group->get_active_slide_index();
	if(index <= 0 ||
			m_slide_group->get_slide(index - 1) == NULL ||
			m_slide_group->get_slide(index) == NULL)
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
	while(step >= 0)
	{
		s1->get_display()->merge_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, step);
		step -= FLIP_STEP;
	}
	if (step != 0)
	{
		s1->get_display()->merge_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, 0);
	}
	return (index - 1);
}

void c_gesture::move_left()
{
	int index = m_slide_group->get_active_slide_index();
	if((index + 1) >= MAX_PAGES ||
			m_slide_group->get_slide(index + 1) == NULL ||
			m_slide_group->get_slide(index) == NULL)
	{
		return;
	}
	c_surface* s1 = m_slide_group->get_slide(index +1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	if(s1->get_display() == s2->get_display())
	{
		s1->get_display()->merge_surface(s2, s1, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, (m_down_x - m_move_x));
	}
}

void c_gesture::move_right()
{
	int index = m_slide_group->get_active_slide_index();
	if(index <= 0 ||
			m_slide_group->get_slide(index - 1) == NULL ||
			m_slide_group->get_slide(index) == NULL)
	{
		return;
	}
	c_surface* s1 = m_slide_group->get_slide(index -1)->get_surface();
	c_surface* s2 = m_slide_group->get_slide(index)->get_surface();
	c_rect rc;
	m_slide_group->get_screen_rect(rc);
	if(s1->get_display() == s2->get_display())
	{
		s1->get_display()->merge_surface(s1, s2, rc.m_left, rc.m_right, rc.m_top, rc.m_bottom, (rc.Width() - (m_move_x - m_down_x)));
	}
}

void c_gesture::handle_hid_msg(MSG_INFO &msg)
{
	switch(msg.dwMsgId)
	{
	case 0x4700://MOUSE_LBUTTONDOWN
		m_root->on_touch_down(msg.dwParam1, msg.dwParam2);
		break;
	case 0x4600://MOUSE_LBUTTONUP
		m_root->on_touch_up(msg.dwParam1, msg.dwParam2);
		break;
	}
}
