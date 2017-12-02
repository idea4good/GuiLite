#ifndef PAGE_GROUP_H_
#define PAGE_GROUP_H_

#define MAX_PAGES	5

#include "core_include/surface.h"

class c_slide_group : public c_wnd {
public:
	c_slide_group();
	int set_active_slide(int index, bool is_redraw = true);
	c_wnd* get_slide(int index){return m_slides[index];}
	c_wnd* get_active_slide(){return m_slides[m_active_slide_index];}
	int get_active_slide_index(){return m_active_slide_index;}
	int add_slide(c_wnd* slide, unsigned short resource_id, short x, short y,
			short width, short height, WND_TREE* p_child_tree = NULL,
			Z_ORDER_LEVEL max_zorder =  Z_ORDER_LEVEL_0);
	int add_clone_silde(c_wnd* slide, unsigned short resource_id, short x, short y,
				short width, short height, WND_TREE* p_child_tree = NULL,
				Z_ORDER_LEVEL max_zorder =  Z_ORDER_LEVEL_0);
	void disabel_all_slide();
protected:
	virtual void display_window();
	virtual c_wnd* clone(){return new c_slide_group();}
	c_wnd* m_slides[MAX_PAGES];
	int m_active_slide_index;
};

class c_slide_root : public c_wnd
{
	virtual c_wnd* clone() { return new c_slide_root(); }
};

#endif
