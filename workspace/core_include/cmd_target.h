#ifndef GUILITE_CORE_INCLUDE_CMD_TARGET_H
#define GUILITE_CORE_INCLUDE_CMD_TARGET_H

#include "../core_include/api.h"

#define MSG_TYPE_INVALID	0xFFFF
#define MSG_TYPE_WND		0x0001
#define MSG_TYPE_USR		0x0002
#define USR_MSG_MAX			32

class c_cmd_target;
typedef void (c_cmd_target::*msgCallback)(int, int);

struct GL_MSG_ENTRY
{
	unsigned int		msgType;
	unsigned int		msgId;
	c_cmd_target*		object;
	msgCallback			callBack;
};

#define ON_GL_USER_MSG(msgId, func)                    	\
{MSG_TYPE_USR, msgId, 0, msgCallback(&func)},

#define GL_DECLARE_MESSAGE_MAP()						\
protected:												\
	virtual const GL_MSG_ENTRY* get_msg_entries() const;\
private:                                                \
	static const GL_MSG_ENTRY m_msg_entries[];

#define GL_BEGIN_MESSAGE_MAP(theClass)					\
const GL_MSG_ENTRY* theClass::get_msg_entries() const	\
{														\
	return theClass::m_msg_entries;						\
}														\
const GL_MSG_ENTRY theClass::m_msg_entries[] =     		\
{

#define GL_END_MESSAGE_MAP()                           	\
{MSG_TYPE_INVALID, 0, 0, 0}};

class c_cmd_target
{
public:
	static int handle_usr_msg(int msg_id, int resource_id, int param)
	{
		int i;
		c_cmd_target* p_wnd = 0;
		for (i = 0; i < ms_user_map_size; i++)
		{
			if (msg_id == ms_usr_map_entries[i].msgId)
			{
				p_wnd = (c_cmd_target*)ms_usr_map_entries[i].object;
				(p_wnd->*ms_usr_map_entries[i].callBack)(resource_id, param);
			}
		}
		return 1;
	}
protected:
	void load_cmd_msg()
	{
		const GL_MSG_ENTRY* p_entry = get_msg_entries();
		if (0 == p_entry)
		{
			return;
		}

		bool bExist = false;
		while (MSG_TYPE_INVALID != p_entry->msgType)
		{
			if (MSG_TYPE_WND == p_entry->msgType)
			{
				p_entry++;
				continue;
			}

			bExist = false;
			for (int i = 0; i < ms_user_map_size; i++)
			{
				//repeat register, return.
				if (p_entry->msgId == ms_usr_map_entries[i].msgId
					&& ms_usr_map_entries[i].object == this)
				{
					bExist = true;
					break;
				}
			}
			if (true == bExist)
			{
				p_entry++;
				continue;
			}

			if (MSG_TYPE_USR == p_entry->msgType)
			{
				if (USR_MSG_MAX == ms_user_map_size)
				{
					ASSERT(false);
				}
				ms_usr_map_entries[ms_user_map_size] = *p_entry;
				ms_usr_map_entries[ms_user_map_size].object = this;
				ms_user_map_size++;
			}
			else
			{
				ASSERT(false);
				break;
			}
			p_entry++;
		}
	}

	const GL_MSG_ENTRY* find_msg_entry(const GL_MSG_ENTRY *pEntry, int msgType, int msgId)
	{
		if (MSG_TYPE_INVALID == msgType)
		{
			return 0;
		}

		while (MSG_TYPE_INVALID != pEntry->msgType)
		{
			if ((msgType == pEntry->msgType) && (msgId == pEntry->msgId))
			{
				return pEntry;
			}
			pEntry++;
		}
		return 0;
	}

private:
	static GL_MSG_ENTRY ms_usr_map_entries[USR_MSG_MAX];
	static unsigned short ms_user_map_size;
	GL_DECLARE_MESSAGE_MAP()
};

#endif
