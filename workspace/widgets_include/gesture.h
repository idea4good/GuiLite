#ifndef GUILITE_WIDGETS_INCLUDE_GESTURE_H
#define GUILITE_WIDGETS_INCLUDE_GESTURE_H

typedef enum{
	TOUCH_MOVE,
	TOUCH_IDLE
}TOUCH_STATE;

class c_slide_group;
class c_gesture{
public:
	c_gesture(c_slide_group* group);
	bool handle_swipe(int x, int y, TOUCH_ACTION action);
private:
	bool on_move(int x);
	bool on_swipe(int x);
	int swipe_left();
	int swipe_right();
	void move_left();
	void move_right();

	int m_down_x;
	int m_down_y;
	int m_move_x;
	int m_move_y;
	TOUCH_STATE m_state;
	c_slide_group* 	m_slide_group;
};

#endif
