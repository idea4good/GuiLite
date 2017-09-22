#ifndef _GESTURE_H_
#define _GESTURE_H_

typedef enum{
	TOUCH_MOVE,
	TOUCH_IDLE
}ACTION;

class c_hid_pipe;
class c_slide_group;
class c_gesture{
public:
	c_gesture(c_wnd* root, c_slide_group* group, c_hid_pipe* hid_pipe);
	void set_page_group(c_slide_group* group){m_slide_group = group;}
protected:
	bool handle_flip(MSG_INFO &msg);
	bool on_move(int x);
	bool on_flip(int x);
private:
	int flip_left();
	int flip_right();
	void move_left();
	void move_right();
	void handle_hid_msg(MSG_INFO &msg);

	int m_down_x;
	int m_down_y;
	int m_move_x;
	int m_move_y;
	ACTION m_action;

	c_slide_group* 	m_slide_group;
	c_wnd*			m_root;
	c_hid_pipe*		m_hid_pipe;

	static c_gesture* ms_gesture[MAX_DISPLAY];
	static void* task(void* param);
};

#endif
