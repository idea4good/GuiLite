#ifndef GUILITE_WIDGETS_INCLUDE_EDIT_H
#define GUILITE_WIDGETS_INCLUDE_EDIT_H

#define MAX_EDIT_STRLEN		32

class c_edit : public c_wnd
{
	friend class c_keyboard;
public:
	virtual c_wnd* clone(){return new c_edit();}
	const char* get_text(){return m_str;}
	void set_text(const char* str);
	void set_keyboard_style(KEYBOARD_STYLE kb_sytle) { m_kb_style = kb_sytle; }
	
protected:
	virtual void pre_create_wnd();
	virtual void on_paint();
	virtual void on_focus();
	virtual void on_kill_focus();
	virtual bool on_touch(int x, int y, TOUCH_ACTION action);
	
	void on_key_board_click(unsigned int ctrl_id, long param);
	GL_DECLARE_MESSAGE_MAP()
private:
	void show_keyboard();
	void on_touch_down(int x, int y);
	void on_touch_up(int x, int y);

	KEYBOARD_STYLE m_kb_style;
	char m_str_input[MAX_EDIT_STRLEN];
	char m_str[MAX_EDIT_STRLEN];
};

#endif
