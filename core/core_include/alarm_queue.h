#ifndef ALARM_QUEUE_H_
#define ALARM_QUEUE_H_

#define ALARM_QUEUE_LENGTH	20

typedef struct{
	unsigned short	id;
	unsigned short 	str_id;
	short 			level;
	long			time_in_sec;
}ALARM_NODE;

class c_alarm_queue{
public:
	c_alarm_queue();
	int get_node_by_cycle(unsigned short &str_id, unsigned short &level);
	int insert_node(unsigned short alarm_id, short level, unsigned short str_id);
	int clear_node(unsigned short alarm_id);
private:
	bool find_node(unsigned short alarm_id);
	int read_node();
	int m_list_tail;
	int m_list_head;
	int m_cycle_cursor;
	ALARM_NODE m_node_list[ALARM_QUEUE_LENGTH];
};

#endif
