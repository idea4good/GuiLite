#include "../../core_include/api.h"
#include "../../core_include/msg.h"
#include <windows.h>

c_fifo::c_fifo()
{
	m_head = m_tail = 0;
	m_read_sem = CreateSemaphore(NULL,	// default security attributes
								 0,		// initial count
								 1,		// maximum count
								 NULL);	// unnamed semaphore
	m_write_mutex = CreateMutex(NULL, FALSE, NULL);
}

int c_fifo::read(void* buf, int len)
{
	unsigned char* pbuf = (unsigned char*)buf;
	int i = 0;
	while(i < len)
	{
		if (m_tail == m_head)
		{//empty
			WaitForSingleObject(m_read_sem, INFINITE);
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

	WaitForSingleObject(m_write_mutex, INFINITE);
	while(i < len)
	{
		if ((m_tail + 1) % FIFO_BUFFER_LEN == m_head)
		{//full, clear data has been written;
			m_tail = tail;
			log_out("Warning: fifo full\n");
			ReleaseMutex(m_write_mutex);
			return 0;
		}
		m_buf[m_tail] = *pbuf++;
		m_tail = (m_tail + 1) % FIFO_BUFFER_LEN;
		i++;
	}
	ReleaseMutex(m_write_mutex);

	if(i != len)
	{
		ASSERT(FALSE);
	}
	else
	{
		ReleaseSemaphore(m_read_sem, 1, NULL);
	}
	return i;
}
