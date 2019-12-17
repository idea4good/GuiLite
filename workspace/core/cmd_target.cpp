#include "../core_include/api.h"
#include "../core_include/cmd_target.h"

GL_MSG_ENTRY c_cmd_target::ms_usr_map_entries[USR_MSG_MAX];
unsigned short c_cmd_target::ms_user_map_size;

GL_BEGIN_MESSAGE_MAP(c_cmd_target)
GL_END_MESSAGE_MAP()

int c_cmd_target::handle_usr_msg(int msgId, int resource_id, int param)
{
	int i;
	c_cmd_target* p_wnd = 0;
	for (i = 0; i < ms_user_map_size; i++)
	{
		if (msgId == ms_usr_map_entries[i].msgId)
		{
			p_wnd = (c_cmd_target*)ms_usr_map_entries[i].object;
			(p_wnd->*ms_usr_map_entries[i].callBack)(resource_id, param);
		}
	}
	return 1;
}

void c_cmd_target::load_cmd_msg()
{
	const GL_MSG_ENTRY* p_entry = get_msg_entries();
	if (0 == p_entry)
	{
		return;
	}

	bool bExist = false;
	while(MSG_TYPE_INVALID != p_entry->msgType)
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
			ms_usr_map_entries[ms_user_map_size] = *p_entry;
			ms_usr_map_entries[ms_user_map_size].object = this;
			ms_user_map_size++;
			if (USR_MSG_MAX == ms_user_map_size)
			{
				ASSERT(false);
			}
		}
		else
		{
			ASSERT(false);
			break;
		}
		p_entry++;
	}
}

const GL_MSG_ENTRY* c_cmd_target::find_msg_entry(const GL_MSG_ENTRY *pEntry, int msgType, int msgId)
{
	if ( MSG_TYPE_INVALID == msgType)
	{
		return 0;
	}

	while (MSG_TYPE_INVALID != pEntry->msgType)
	{
		if ( (msgType == pEntry->msgType) && (msgId == pEntry->msgId))
		{
			return pEntry;
		}
		pEntry++;
	}
	return 0;
}
