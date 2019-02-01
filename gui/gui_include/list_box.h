#ifndef LIST_BOX_H
#define LIST_BOX_H

#define MAX_ITEM_NUM			12

#define GL_LIST_SELECT			0x1
#define GL_LIST_CONFIRM		0x2
#define GL_LIST_ROTATION		0x3

#define ON_LIST_SELECT(ctrlId, func)                                       \
{MSG_TYPE_WND, GL_LIST_SELECT, (c_cmd_target*)ctrlId, ctrlId, MSG_CALLBACK_VWV, reinterpret_cast<MsgFuncVV>(&func)},

#define ON_LIST_CONFIRM(ctrlId, func)                                      \
{MSG_TYPE_WND, GL_LIST_CONFIRM, (c_cmd_target*)ctrlId, ctrlId, MSG_CALLBACK_VWV, reinterpret_cast<MsgFuncVV>(&func)},

#define ON_LIST_ROTATION(ctrlId, func)                                     \
{MSG_TYPE_WND, GL_LIST_ROTATION, (c_cmd_target*)ctrlId, ctrlId, MSG_CALLBACK_VWL, reinterpret_cast<MsgFuncVV>(&func)},

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
	virtual void on_touch_down(int x, int y);
	virtual void on_touch_up(int x, int y);
	
private:
	void update_list_size();
	void show_list();

	short			m_selected_item;
	short			m_item_total;
	char*			m_item_array[MAX_ITEM_NUM];
	c_rect			m_list_wnd_rect;	//rect relative to parent wnd.
	c_rect			m_list_screen_rect;	//rect relative to physical screen(frame buffer)
};

#endif
