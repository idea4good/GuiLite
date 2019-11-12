#ifndef GUILITE_WIDGETS_INCLUDE_SPINBOX_H
#define GUILITE_WIDGETS_INCLUDE_SPINBOX_H

#define GL_SPIN_CONFIRM				0x2222
#define	GL_SPIN_CHANGE				0x3333

#define ON_SPIN_CONFIRM(ctrlId, func) \
{MSG_TYPE_WND, GL_SPIN_CONFIRM, (c_cmd_target*)ctrlId, MSG_CALLBACK_VWL, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int, int)>(&func))},

#define ON_SPIN_CHANGE(ctrlId, func) \
{MSG_TYPE_WND, GL_SPIN_CHANGE, (c_cmd_target*)ctrlId, MSG_CALLBACK_VWL, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int, int)>(&func))},

class c_spin_box : public c_wnd
{
public:
	short get_value(){return m_value;}
	void set_value(unsigned short value){m_value = m_cur_value = value;}

	void set_max_min(short max, short min){m_max = max; m_min = min;}
	void set_step(short step){m_step = step;}

	short get_min(){return m_min;}
	short get_max(){return m_max;}
	short get_step(){return m_step;}

	void set_value_digit(short digit){m_digit = digit;}
	short get_value_digit(){return m_digit;}
	
protected:
	virtual c_wnd* clone(){return new c_spin_box();}
	virtual void on_paint();
	virtual void on_focus();
	virtual void on_kill_focus();
	virtual void pre_create_wnd();
	virtual bool on_touch(int x, int y, TOUCH_ACTION action);

	void on_arrow_up_bt_click(unsigned int ctr_id);
	void on_arrow_down_bt_click(unsigned int ctr_id);

	GL_DECLARE_MESSAGE_MAP()

private:
	void show_arrow_button();
	void hide_arrow_button();
	void on_touch_down(int x, int y);
	void on_touch_up(int x, int y);

protected:
	short			m_cur_value;
	short			m_value;
	short			m_step;
	short			m_max;
	short			m_min;
	short			m_digit;

	c_button  		m_bt_up;
	c_button  		m_bt_down;
	c_rect			m_bt_up_rect;
	c_rect			m_bt_down_rect;
};

#endif
