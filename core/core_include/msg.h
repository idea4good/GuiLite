#ifndef _MSG_H
#define _MSG_H

typedef struct
{
	unsigned int dwMsgId;
	unsigned int dwParam1;
	unsigned int dwParam2;
}MSG_INFO;

int read_usr_msg(MSG_INFO* msg);
int write_usr_msg(MSG_INFO* msg);

#define FIFO_BUFFER_LEN		1024
#define FIFO_NAME_LEN		16
class c_fifo
{
public:
	c_fifo(const char* name);
	int read(void* buf, int len);
	int write(void* buf, int len);
	
private:
	unsigned char 	m_buf[FIFO_BUFFER_LEN];
	char	m_name[FIFO_NAME_LEN];
	int		m_head;
	int		m_tail;
	void* 	m_read_sem;
	void*	m_write_mutex;
};

#define MAX_HID_PIPES	9
class c_hid_pipe
{
public:
	c_hid_pipe(void* id);

	static int read_hid_msg();
	static int write_hid_msg(MSG_INFO* msg, unsigned int display_id = 0);
	MSG_INFO 	m_msg;
	int 		m_fd;
private:
	static c_hid_pipe* ms_pipes[MAX_HID_PIPES];
	static int ms_max_fd;
};
#endif
