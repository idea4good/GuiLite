#ifndef GUILITE_WIDGETS_INCLUDE_BUTTON_H
#define GUILITE_WIDGETS_INCLUDE_BUTTON_H

#define GL_BN_CLICKED							0x1111
#define ON_GL_BN_CLICKED(func)                                       \
{MSG_TYPE_WND, GL_BN_CLICKED, 0, msgCallback(&func)},

typedef struct struct_bitmap_info BITMAP_INFO;
class c_button : public c_wnd
{
protected:
	virtual void on_paint();
	virtual void on_focus();
	virtual void on_kill_focus();
	virtual void pre_create_wnd();

	virtual void on_touch(int x, int y, TOUCH_ACTION action);
	virtual void on_key(KEY_TYPE key);
};

#endif
