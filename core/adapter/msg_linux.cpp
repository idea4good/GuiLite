#include "../../core_include/api.h"
#include "../../core_include/msg.h"
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

c_fifo::c_fifo()
{
	m_head = m_tail = 0;
	m_read_sem = malloc(sizeof(sem_t));
	m_write_mutex = malloc(sizeof(pthread_mutex_t));
	
	sem_init((sem_t*)m_read_sem, 0, 0);
	pthread_mutex_init((pthread_mutex_t*)m_write_mutex, 0);
}

int c_fifo::read(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	while(i < len)
	{
		if (m_tail == m_head)
		{//empty
			sem_wait((sem_t*)m_read_sem);
			continue;
		}
		*pbuf++ = m_buf[m_head];
		m_head = (m_head + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	if(i != len)
	{
		ASSERT(false);
	}
	return i;
}

int c_fifo::write(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	int tail = m_tail;

	pthread_mutex_lock((pthread_mutex_t*)m_write_mutex);
	while(i < len)
	{
		if ((m_tail + 1) % FIFO_BUFFER_LEN == m_head)
		{//full, clear data has been written;
			m_tail = tail;
			log_out("Warning: fifo full\n");
			pthread_mutex_unlock((pthread_mutex_t*)m_write_mutex);
			return 0;
		}
		m_buf[m_tail] = *pbuf++;
		m_tail = (m_tail + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	pthread_mutex_unlock((pthread_mutex_t*)m_write_mutex);

	if(i != len)
	{
		ASSERT(false);
	}
	else
	{
		sem_post((sem_t*)m_read_sem);
	}
	return i;
}
