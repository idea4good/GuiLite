#include "../core_include/api.h"
#include "../core_include/cmd_target.h"

GL_MSG_ENTRY c_cmd_target::ms_usr_map_entries[USR_MSG_MAX];
unsigned short c_cmd_target::ms_user_map_size;

GL_BEGIN_MESSAGE_MAP(c_cmd_target)
GL_END_MESSAGE_MAP()

c_cmd_target::c_cmd_target()
{
}

c_cmd_target::~c_cmd_target()
{
}

int c_cmd_target::handle_usr_msg(unsigned int msgId, unsigned int wParam, unsigned int lParam)
{
	int i;
	c_cmd_target* p_wnd = 0;
	MSGFUNCS msg_funcs;
	for (i = 0; i < ms_user_map_size; i++)
	{
		if (msgId == ms_usr_map_entries[i].msgId)
		{
			p_wnd = (c_cmd_target*)ms_usr_map_entries[i].pObject;
			msg_funcs.func = ms_usr_map_entries[i].func;
			(p_wnd->*msg_funcs.func_vwl)(wParam , lParam);
		}
	}
	return 1;
}

void c_cmd_target::load_cmd_msg()
{
	const GL_MSG_ENTRY* p_entry = GetMSgEntries();
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
				&& ms_usr_map_entries[i].pObject == this)
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
			ms_usr_map_entries[ms_user_map_size].pObject = this;
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

const GL_MSG_ENTRY* c_cmd_target::FindMsgEntry(const GL_MSG_ENTRY *pEntry, 
	unsigned int msgType, unsigned short msgId, unsigned short ctrlId)
{
	if ( MSG_TYPE_INVALID == msgType)
	{
		return 0;
	}

	while (MSG_CALLBACK_NULL != pEntry->callbackType)
	{
		if ( (msgType == pEntry->msgType) && (msgId == pEntry->msgId) && (void*)(unsigned long)ctrlId == pEntry->pObject)
		{
			return pEntry;
		}
		pEntry++;
	}
	return 0;
}
