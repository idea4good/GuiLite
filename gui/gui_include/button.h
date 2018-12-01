#ifndef _BUTTON_H_
#define _BUTTON_H_

#define GL_BN_CLICKED							0x1111
#define ON_GL_BN_CLICKED(ctrlId, func)                                       \
{MSG_TYPE_WND, GL_BN_CLICKED, (c_cmd_target*)ctrlId, MSG_CALLBACK_VWV, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int)>(&func))},

typedef struct struct_bitmap_info BITMAP_INFO;
class c_button : public c_wnd
{
public:
	virtual const char* get_class_name() const {return "c_button";}
	virtual c_wnd* clone(){return new c_button();}

	void set_bitmap(const BITMAP_INFO *pBitmap) { m_bitmap_normal = pBitmap; }
	void set_focus_bitmap(const BITMAP_INFO *pBitmap) { m_bitmap_focus = pBitmap; }
	void set_pushed_bitmap(const BITMAP_INFO *pBitmap) { m_bitmap_pushed = pBitmap; }
	void set_disable_bitmap(const BITMAP_INFO *pBitmap) { m_bitmap_disable = pBitmap; }
protected:
	virtual void on_paint();
	virtual void on_focus();
	virtual void on_kill_focus();
	virtual void on_touch_down(int x, int y);
	virtual void on_touch_up(int x, int y);
	virtual void pre_create_wnd();

	const BITMAP_INFO*	m_bitmap_normal;
	const BITMAP_INFO*	m_bitmap_focus;
	const BITMAP_INFO*	m_bitmap_pushed;
	const BITMAP_INFO*	m_bitmap_disable;
};

#endif
