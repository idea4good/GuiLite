#ifndef GUILITE_WIDGETS_INCLUDE_SPINBOX_H
#define GUILITE_WIDGETS_INCLUDE_SPINBOX_H

#define	GL_SPIN_CHANGE				0x3333

#define ON_SPIN_CHANGE(func) \
{MSG_TYPE_WND, GL_SPIN_CHANGE, 0, msgCallback(&func)},

class c_spin_box;
class c_spin_button : public c_button
{
	friend class c_spin_box;
	virtual void on_touch(int x, int y, TOUCH_ACTION action);
	c_spin_box* m_spin_box;
};

class c_spin_box : public c_wnd
{
	friend class c_spin_button;
public:
	short get_value() { return m_value; }
	void set_value(unsigned short value) { m_value = m_cur_value = value; }
	void set_max_min(short max, short min) { m_max = max; m_min = min; }
	void set_step(short step) { m_step = step; }
	short get_min() { return m_min; }
	short get_max() { return m_max; }
	short get_step() { return m_step; }
	void set_value_digit(short digit) { m_digit = digit; }
	short get_value_digit() { return m_digit; }

protected:
	virtual void on_paint();
	virtual void pre_create_wnd();
	void on_arrow_up_bt_click();
	void on_arrow_down_bt_click();

	short			m_cur_value;
	short			m_value;
	short			m_step;
	short			m_max;
	short			m_min;
	short			m_digit;
	c_spin_button  	m_bt_up;
	c_spin_button  	m_bt_down;
};

#endif
