#ifndef GUILITE_CORE_INCLUDE_CMD_TARGET_H
#define GUILITE_CORE_INCLUDE_CMD_TARGET_H

class c_cmd_target;

#define MSG_TYPE_INVALID	0xFFFF
#define MSG_TYPE_WND		0x0001
#define MSG_TYPE_USR		0x0002
#define USR_MSG_MAX			32

typedef void (c_cmd_target::*msgCallback)(int, int);

struct GL_MSG_ENTRY
{
	unsigned int		msgType;
	unsigned int		msgId;
	c_cmd_target*		object;
	msgCallback			callBack;
};

#define ON_GL_USER_MSG(msgId, func)                    \
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

#define GL_END_MESSAGE_MAP()                           \
{MSG_TYPE_INVALID, 0, 0, 0}};

class c_cmd_target
{
public:
	static int handle_usr_msg(int msg_id, int resource_id, int param);
protected:
	void load_cmd_msg();
	const GL_MSG_ENTRY* find_msg_entry(const GL_MSG_ENTRY *pEntry, int msgType, int msgId);
private:
	static GL_MSG_ENTRY ms_usr_map_entries[USR_MSG_MAX];
	static unsigned short ms_user_map_size;
	GL_DECLARE_MESSAGE_MAP()
};

#endif
