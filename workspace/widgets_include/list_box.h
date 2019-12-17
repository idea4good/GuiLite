#ifndef GUILITE_WIDGETS_INCLUDE_LIST_BOX_H
#define GUILITE_WIDGETS_INCLUDE_LIST_BOX_H

#define MAX_ITEM_NUM			4
#define GL_LIST_CONFIRM			0x1

#define ON_LIST_CONFIRM(func) \
{MSG_TYPE_WND, GL_LIST_CONFIRM, 0, msgCallback(&func)},

class c_list_box : public c_wnd
{
public:
	int add_item(char* str);
	void clear_item();
	short get_item_count() { return m_item_total; }
	void  select_item(short index);
	
protected:
	virtual c_wnd* clone(){return new c_list_box();}
	virtual void pre_create_wnd();
	virtual void on_paint();
	virtual void on_focus();
	virtual void on_kill_focus();
	virtual bool on_touch(int x, int y, TOUCH_ACTION action);
	
private:
	void update_list_size();
	void show_list();
	void on_touch_down(int x, int y);
	void on_touch_up(int x, int y);

	short			m_selected_item;
	short			m_item_total;
	char*			m_item_array[MAX_ITEM_NUM];
	c_rect			m_list_wnd_rect;	//rect relative to parent wnd.
	c_rect			m_list_screen_rect;	//rect relative to physical screen(frame buffer)
};

#endif
