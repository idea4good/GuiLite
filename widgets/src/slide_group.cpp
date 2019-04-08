#include "core_include/api.h"
#include "core_include/rect.h"
#include "core_include/surface.h"
#include "core_include/display.h"
#include "core_include/cmd_target.h"
#include "core_include/wnd.h"
#include "core_include/surface.h"
#include "../widgets_include/dialog.h"
#include "../widgets_include/slide_group.h"

c_slide_group::c_slide_group()
{
	for(int i = 0; i < MAX_PAGES; i++)
	{
		m_slides[i] = NULL;
	}
	m_active_slide_index = 0;
}

int c_slide_group::set_active_slide(int index, bool is_redraw)
{
	if(index >= MAX_PAGES || index < 0)
	{
		return -1;
	}
	if(0 == m_slides[index])
	{
		return -2;
	}
	m_active_slide_index = index;

	for(int i = 0; i < MAX_PAGES; i++)
	{
		if(m_slides[i] == NULL)
		{
			continue;
		}
		if(i == index)
		{
			m_slides[i]->get_surface()->set_active(true);
			add_child_2_tail(m_slides[i]);
			if(is_redraw)
			{
				c_rect rc;
				get_screen_rect(rc);
				m_slides[i]->get_surface()->flush_scrren(rc.m_left, rc.m_top, rc.m_right, rc.m_bottom);
			}
		}
		else
		{
			m_slides[i]->get_surface()->set_active(false);
		}
	}
	return 0;
}

int c_slide_group::add_slide(c_wnd* slide, unsigned short resource_id, short x, short y,
			short width, short height, WND_TREE* p_child_tree, Z_ORDER_LEVEL max_zorder)
{
	if(NULL == slide)
	{
		return -1;
	}

	c_surface* old_surface = get_surface();
	c_surface* new_surface = old_surface->get_display()->alloc_surface(slide,max_zorder);
	new_surface->set_active(false);
	set_surface(new_surface);
	slide->connect(this, resource_id ,0 , x, y, width, height, p_child_tree);
	set_surface(old_surface);

	int i = 0;
	while(i < MAX_PAGES)
	{
		if(m_slides[i] == slide)
		{//slide has lived
			ASSERT(FALSE);
			return -2;
		}
		i++;
	}

	//new slide
	i = 0;
	while(i < MAX_PAGES)
	{
		if(m_slides[i] == NULL)
		{
			m_slides[i] = slide;
			slide->show_window();
			return 0;
		}
		i++;
	}

	//no more slide can be add
	ASSERT(FALSE);
	return -3;
}

int c_slide_group::add_clone_silde(c_wnd* slide, unsigned short resource_id, short x, short y,
			short width, short height, WND_TREE* p_child_tree, Z_ORDER_LEVEL max_zorder)
{
	if(NULL == slide)
	{
		return -1;
	}

	c_surface* old_surface = get_surface();
	c_surface* new_surface = old_surface->get_display()->alloc_surface(slide,max_zorder);
	new_surface->set_active(false);
	set_surface(new_surface);
	c_wnd* page_tmp = slide->connect_clone(this,resource_id,0,x,y,width,height,p_child_tree);
	set_surface(old_surface);

	int i = 0;
	while(i < MAX_PAGES)
	{
		if(m_slides[i] == page_tmp)
		{//slide has lived
			ASSERT(FALSE);
			return -2;
		}
		i++;
	}

	//new slide
	i = 0;
	while(i < MAX_PAGES)
	{
		if(m_slides[i] == NULL)
		{
			m_slides[i] = page_tmp;
			page_tmp->show_window();
			return 0;
		}
		i++;
	}

	//no more slide can be add
	ASSERT(FALSE);
	return -3;
}

void c_slide_group::disabel_all_slide()
{
	for(int i = 0; i < MAX_PAGES; i++)
	{
		if(m_slides[i])
		{
			m_slides[i]->get_surface()->set_active(false);
		}
	}
	m_active_child = NULL;
}
