#ifndef _DIALOG_H_
#define _DIALOG_H_

class c_surface;
class c_dialog;

typedef struct
{
	c_dialog* 	dialog;
	c_surface*	surface;
} DIALOG_ARRAY;

#define	MAX_DIALOG		9

class c_dialog : public c_wnd
{
public:
	static c_dialog* get_the_dialog(c_surface* surface);
	static int open_dialog(c_dialog* p_dlg);
	c_dialog(){m_divider_lines = 0;}
	int close_dialog();
	void set_divider_lines(unsigned char lines){m_divider_lines = lines;}
	virtual void handle_mouse_down_msg(int x, int y);
	virtual void handle_mouse_up_msg(int x, int y);
protected:
	virtual const char* get_class_name(void) const {return "c_dialog";}
	virtual void pre_create_wnd();
	virtual void on_paint(void);
	virtual void bn_exit(unsigned int ctr_id);
	void write_user_msg(short msg_id,short value);
	static DIALOG_ARRAY ms_the_dialogs[MAX_DIALOG];
private:
	int set_me_the_dialog();
	unsigned char m_divider_lines;
};

#endif
