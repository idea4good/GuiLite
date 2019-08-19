#ifndef GUILITE_CORE_INCLUDE_MSG_H
#define GUILITE_CORE_INCLUDE_MSG_H

typedef struct
{
	unsigned int dwMsgId;
	unsigned int dwParam1;
	unsigned int dwParam2;
}MSG_INFO;

#define FIFO_BUFFER_LEN		1024
class c_fifo
{
public:
	c_fifo();
	int read(void* buf, int len);
	int write(void* buf, int len);
	
private:
	unsigned char 	m_buf[FIFO_BUFFER_LEN];
	int		m_head;
	int		m_tail;
	void* 	m_read_sem;
	void*	m_write_mutex;
};

#endif
