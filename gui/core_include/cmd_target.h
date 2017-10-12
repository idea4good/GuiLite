#ifndef CMD_TARGET_H
#define CMD_TARGET_H

class c_cmd_target;

#define MSG_TYPE_INVALID	0xFFFF
#define MSG_TYPE_WND		0x0001
#define MSG_TYPE_USR		0x0002
#define USR_MSG_MAX			1024

typedef void (c_cmd_target::*MsgFuncVV)();

enum MSG_CALLBACK_TYPE
{
	MSG_CALLBACK_NULL = 0,
	MSG_CALLBACK_VV,
	MSG_CALLBACK_IWL,
	MSG_CALLBACK_IWV,
	MSG_CALLBACK_VWV,
	MSG_CALLBACK_VVL,
	MSG_CALLBACK_VWL,
	MSG_CALLBACK_IVV
};

struct GLT_MSG_ENTRY
{
	unsigned int		msgType;
	unsigned int		msgId;
	c_cmd_target*		pObject;
	MSG_CALLBACK_TYPE	callbackType;
	MsgFuncVV			func;
};

typedef struct GLT_MSG_MAP
{
	const GLT_MSG_MAP *baseMsgMap;
	const GLT_MSG_ENTRY *msgMapEntry;
}GLT_MSG_MAP;

#define ON_GLT_USER_MSG(msgId, func)                        \
{MSG_TYPE_USR, msgId, 0, MSG_CALLBACK_VWL, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int, unsigned int)>(&func))},

#define GLT_DECLARE_MESSAGE_MAP()                           \
protected:													\
static const GLT_MSG_MAP mMsgMap;                           \
virtual const GLT_MSG_MAP* GetMessageMap() const; 		\
private:                                                    \
static const GLT_MSG_ENTRY mMsgMapEntries[];

#define GLT_BEGIN_MESSAGE_MAP(theClass, baseClass)			\
const GLT_MSG_MAP* theClass::GetMessageMap() const			\
{															\
	return &(theClass::mMsgMap);							\
}															\
const GLT_MSG_MAP theClass::mMsgMap =                       \
{                                                           \
	&(baseClass::mMsgMap), &(theClass::mMsgMapEntries[0])   \
};                                                          \
const GLT_MSG_ENTRY theClass::mMsgMapEntries[] =     		\
{

#define GLT_END_MESSAGE_MAP()                               \
{MSG_TYPE_INVALID, 0, (c_cmd_target*)0, MSG_CALLBACK_NULL, (MsgFuncVV)0}};

class c_cmd_target
{
public:
	c_cmd_target();
	virtual ~c_cmd_target();
	static int handle_usr_msg(unsigned int msgId, unsigned int wParam, unsigned int lParam);
protected:
	void load_cmd_msg();
	const GLT_MSG_ENTRY* FindMessageMapEntry(const GLT_MSG_ENTRY *pEntry, 
		unsigned int msgType, unsigned short msgId, unsigned short ctrlId);
private:
	static GLT_MSG_ENTRY ms_usr_map_entries[USR_MSG_MAX];
	static unsigned short ms_user_map_size;
	GLT_DECLARE_MESSAGE_MAP()
};

typedef union
{
	void (c_cmd_target::*func)();
	void (c_cmd_target::*func_vwv)(unsigned int w_param);
	int (c_cmd_target::*func_iwl)(unsigned int w_param, long l_param);
	int (c_cmd_target::*func_iwv)(unsigned int w_param);
	void (c_cmd_target::*func_vvl)(long l_param);
	void (c_cmd_target::*func_vwl)(unsigned int w_param, long l_param);
	int (c_cmd_target::*func_ivv)();
}MSGFUNCS; 

#endif
