#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define KEYBORAD_CLICK			0x5014
#define ON_KEYBORAD_UPDATE(ctrlId, func)  \
{MSG_TYPE_WND, KEYBORAD_CLICK, (c_cmd_target*)ctrlId, MSG_CALLBACK_VWL, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int, long)>(&func))},

typedef enum
{
	STATUS_UPPERCASE,
	STATUS_LOWERCASE
}KEYBOARD_STATUS;

typedef enum
{
	STYLE_ALL_BOARD,
	STYLE_NUM_BOARD
}KEYBOARD_STYLE;

typedef enum
{
	CLICK_CHAR,
	CLICK_ENTER,
	CLICK_ESC
}CLICK_STATUS;

class c_keyboard: public c_wnd
{
public:
	virtual int create(c_wnd *parent, unsigned short resource_id, unsigned short str_id,
		short x, short y, short width, short height, WND_TREE* p_child_tree = NULL);
	KEYBOARD_STATUS get_cap_status(){return m_cap_status;}
	void set_style(KEYBOARD_STYLE style) { m_style = style; }
	char* get_str() { return m_str; }
protected:
	virtual void pre_create_wnd();
	virtual c_wnd* clone(){return new c_keyboard();}

	void on_char_clicked(unsigned int ctrl_id);
	void on_del_clicked(unsigned int ctrl_id);
	void on_caps_clicked(unsigned int ctrl_id);
	void on_enter_clicked(unsigned int ctrl_id);
	void on_esc_clicked(unsigned int ctrl_id);

	GLT_DECLARE_MESSAGE_MAP()
private:
	char m_str[32];
	int	 m_str_len;
	KEYBOARD_STATUS m_cap_status;
//	KEYBOARD_STYLE	m_stlyle; // not used
};

class c_keyboard_button : public c_button
{
protected:
	virtual c_wnd* clone(){return new c_keyboard_button();}
	virtual void on_paint();
};

#endif /* KEYBOARD_H_ */
