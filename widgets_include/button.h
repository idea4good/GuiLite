#ifndef GUILITE_WIDGETS_INCLUDE_BUTTON_H
#define GUILITE_WIDGETS_INCLUDE_BUTTON_H

#define GL_BN_CLICKED							0x1111
#define ON_GL_BN_CLICKED(ctrlId, func)                                       \
{MSG_TYPE_WND, GL_BN_CLICKED, (c_cmd_target*)ctrlId, MSG_CALLBACK_VWV, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int)>(&func))},

typedef struct struct_bitmap_info BITMAP_INFO;
class c_button : public c_wnd
{
public:
	virtual c_wnd* clone(){return new c_button();}
protected:
	virtual void on_paint();
	virtual void on_focus();
	virtual void on_kill_focus();
	virtual void pre_create_wnd();

	virtual bool on_touch(int x, int y, TOUCH_ACTION action);
	virtual bool on_key(KEY_TYPE key);
};

#endif
