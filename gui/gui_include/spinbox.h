#ifndef _SPIN_BOX_H_
#define _SPIN_BOX_H_

#define GLT_SPIN_SELECT						0x2222
#define GLT_SPIN_CONFIRM					0x3333
#define	GLT_SPIN_ROTATION					0x4444

#define ON_SPIN_SELECT(ctrlId, func)                                       \
{MSG_TYPE_WND, GLT_SPIN_SELECT, (c_cmd_target*)ctrlId, MSG_CALLBACK_VWV, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int)>(&func))},

#define ON_SPIN_CONFIRM(ctrlId, func)                                      \
{MSG_TYPE_WND, GLT_SPIN_CONFIRM, (c_cmd_target*)ctrlId, MSG_CALLBACK_VWV, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int)>(&func))},

#define ON_SPIN_ROTATION(ctrlId, func)                                     \
{MSG_TYPE_WND, GLT_SPIN_ROTATION, (c_cmd_target*)ctrlId, MSG_CALLBACK_VWL, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int)>(&func))},

class c_spin_box : public c_wnd
{
public:
	short get_value(void){return m_value;}
	void set_value(unsigned short value){m_value = m_cur_value = value;}

	void set_max_min(short max, short min){m_max = max; m_min = min;}
	void set_step(short step){m_step = step;}

	short get_min(void){return m_min;}
	short get_max(void){return m_max;}
	short get_step(void){return m_step;}

	void set_value_digit(short digit){m_digit = digit;}
	short get_value_digit(void){return m_digit;}
	
protected:
	virtual c_wnd* clone(){return new c_spin_box();}
	virtual void on_paint(void);
	virtual void on_focus(unsigned int w_param);
	virtual void on_kill_focus(void);
	
	virtual void handle_mouse_down_msg(int x, int y);
	virtual void handle_mouse_up_msg(int x, int y);
	virtual void pre_create_wnd();

protected:
	void on_arrow_up_bt_click(unsigned int ctr_id);
	void on_arrow_down_bt_click(unsigned int ctr_id);

	GLT_DECLARE_MESSAGE_MAP()

private:
	void show_arrow_button(void);
	void hide_arrow_button(void);

protected:
	short			m_cur_value;
	short			m_value;
	short			m_step;
	short			m_max;
	short			m_min;
	short			m_digit;

	c_button  		m_bt_arrow_up;
	c_button  		m_bt_arrow_down;
	c_rect			m_bt_arrow_up_rect;
	c_rect			m_bt_arrow_down_rect;
};

#endif
