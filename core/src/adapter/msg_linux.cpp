#include "../../core_include/api.h"
#include "../../core_include/rect.h"
#include "../../core_include/surface.h"
#include "../../core_include/display.h"
#include "../../core_include/msg.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

c_hid_pipe* c_hid_pipe::ms_pipes[MAX_HID_PIPES];
int c_hid_pipe::ms_max_fd;

static c_fifo	s_usr_fifo("usr fifo");

int read_usr_msg(MSG_INFO* msg)
{
	return s_usr_fifo.read(msg, sizeof(MSG_INFO));
}

int write_usr_msg(MSG_INFO* msg)
{
	if(msg->dwMsgId & 0xf000000)ASSERT(FALSE);
	return s_usr_fifo.write(msg, sizeof(MSG_INFO));
}

c_fifo::c_fifo(const char* name)
{
	if (strlen(name) >= FIFO_NAME_LEN)
	{
		ASSERT(FALSE);
	}
	memset(m_name, 0, sizeof(m_name));
	strcpy(m_name, name);

	m_head = m_tail = 0;
	m_read_sem = malloc(sizeof(sem_t));
	m_write_mutex = malloc(sizeof(pthread_mutex_t));
	
	sem_init(m_read_sem, 0, 0);
	pthread_mutex_init(m_write_mutex, NULL);
}

int c_fifo::read(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	while(i < len)
	{
		if (m_tail == m_head)
		{//empty
			sem_wait(m_read_sem);
			continue;
		}
		*pbuf++ = m_buf[m_head];
		m_head = (m_head + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	if(i != len)
	{
		ASSERT(FALSE);
	}
	return i;
}

int c_fifo::write(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	int tail = m_tail;

	pthread_mutex_lock(m_write_mutex);
	while(i < len)
	{
		if ((m_tail + 1) % FIFO_BUFFER_LEN == m_head)
		{//full, clear data has been written;
			m_tail = tail;
			log_out("Warning: ");
			log_out(m_name);
			log_out(" full\n");
			pthread_mutex_unlock(m_write_mutex);
			return 0;
		}
		m_buf[m_tail] = *pbuf++;
		m_tail = (m_tail + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	pthread_mutex_unlock(m_write_mutex);

	if(i != len)
	{
		ASSERT(FALSE);
	}
	else
	{
		sem_post(m_read_sem);
	}
	return i;
}

////////////////////////////////////
//				HID				  //
////////////////////////////////////
static c_fifo	s_hid_fifo("hid fifo");
c_hid_pipe::c_hid_pipe(void* id)
{
	for(int i = 0; i < MAX_HID_PIPES; i++)
	{
		if(!ms_pipes[i])
		{
			ms_pipes[i] = this;
			break;
		}
	}
}

int c_hid_pipe::read_hid_msg()
{
	int ret = -1;
	for(int i = 0; i < MAX_HID_PIPES; i++)
	{
		if(ms_pipes[i])
		{
			ms_pipes[i]->m_msg.dwMsgId = XXX;
		}
	}
	MSG_INFO msg;
	ret = s_hid_fifo.read(&msg, sizeof(MSG_INFO));
	if(0 >= ret)
	{
		return ret;
	}
	int display_id = ((msg.dwMsgId & 0xff00000) >> 24);
	msg.dwMsgId &= 0x00ffffff;//recover message.

	if (!ms_pipes[display_id])
	{
		return -2;
	}
	ms_pipes[display_id]->m_msg = msg;
	return ret;
}

int c_hid_pipe::write_hid_msg(MSG_INFO* msg, unsigned int display_id)
{
	if(MAX_DISPLAY <= display_id)
	{
		ASSERT(FALSE);
		return -1;
	}
	msg->dwMsgId |= ((display_id) << 24);//merge display ID in message.
	return s_hid_fifo.write(msg, sizeof(MSG_INFO));
}