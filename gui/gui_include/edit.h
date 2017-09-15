#ifndef _EDIT_H_
#define _EDIT_H_

#define KEY_BOARD_STYLE    	0x00001000L
#define NUM_BOARD_STYLE    	0x00002000L
#define MAX_EDIT_STRLEN		32

class c_edit : public c_wnd
{
	friend class c_keyboard;
public:
	virtual c_wnd* clone(){return new c_edit();}
	virtual const char* get_class_name(void) const {return "c_edit";}
	const char* get_text(void){return m_str;}
	void set_text(const char* str);
	
protected:
	virtual void pre_create_wnd();
	virtual void on_paint(void);
	virtual void on_focus(unsigned int w_param);
	virtual void on_kill_focus(void);
	virtual void handle_mouse_down_msg(int x, int y);
	virtual void handle_mouse_up_msg(int x, int y);
	
	void on_key_board_click(unsigned int ctrl_id, long param);
	GLT_DECLARE_MESSAGE_MAP()
private:
	void show_keyboard();

	char m_str_input[MAX_EDIT_STRLEN];
	char m_str[MAX_EDIT_STRLEN];
};

#endif
