#ifndef DIALOG_H
#define DIALOG_H

class c_surface;
class c_dialog;

typedef struct
{
	c_dialog* 	dialog;
	c_surface*	surface;
} DIALOG_ARRAY;

class c_dialog : public c_wnd
{
public:
	static int open_dialog(c_dialog* p_dlg);
	static int close_dialog(c_surface* surface);
	static c_dialog* get_the_dialog(c_surface* surface);
	void set_divider_lines(unsigned char lines){m_divider_lines = lines;}
	virtual void on_touch_down(int x, int y);
	virtual void on_touch_up(int x, int y);
protected:
	virtual const char* get_class_name(void) const {return "c_dialog";}
	virtual void pre_create_wnd();
	virtual void on_paint();
	static DIALOG_ARRAY ms_the_dialogs[SURFACE_CNT_MAX];
private:
	int set_me_the_dialog();
	unsigned char m_divider_lines;
};

#endif
