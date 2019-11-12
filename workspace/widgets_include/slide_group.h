#ifndef GUILITE_WIDGETS_INCLUDE_SLIDE_GROUP_H
#define GUILITE_WIDGETS_INCLUDE_SLIDE_GROUP_H

#define MAX_PAGES	5
class c_gesture;
class c_slide_group : public c_wnd {
public:
	c_slide_group();
	int set_active_slide(int index, bool is_redraw = true);
	c_wnd* get_slide(int index){return m_slides[index];}
	c_wnd* get_active_slide(){return m_slides[m_active_slide_index];}
	int get_active_slide_index(){return m_active_slide_index;}
	int add_slide(c_wnd* slide, unsigned short resource_id, short x, short y,
			short width, short height, WND_TREE* p_child_tree = 0,
			Z_ORDER_LEVEL max_zorder =  Z_ORDER_LEVEL_0);
	int add_clone_silde(c_wnd* slide, unsigned short resource_id, short x, short y,
				short width, short height, WND_TREE* p_child_tree = 0,
				Z_ORDER_LEVEL max_zorder =  Z_ORDER_LEVEL_0);
	void disabel_all_slide();
	virtual bool on_touch(int x, int y, TOUCH_ACTION action);
	virtual bool on_key(KEY_TYPE key);
protected:
	virtual c_wnd* clone(){return new c_slide_group();}
	c_wnd* m_slides[MAX_PAGES];
	int m_active_slide_index;
	c_gesture* m_gesture;
};

#endif
