#include "msgqueue.h"


namespace Common{

MsgQueue::MsgQueue(std::string path=".", int id = 2016):_key(-1),_msgid(0)
{
	_key = ftok(path.c_str(), id);
	if( -1 == _key)
		throw QueueException("init msgQueue failed, create key failed.", __FILE__, __LINE__, errno);
	/*
	_msgid = msgget(_key, IPC_CREAT);
	if( _msgid == -1 )
		throw QueueException("init msgQueue failed, msgget failed.", __FILE__, __LINE__, errno);
	*/
}

MsgQueue::~MsgQueue()
{

}

bool MsgQueue::deleteMsgid()
{
	if(_msgid > 0 )
		if(msgctl(_msgid, IPC_RMID, 0) == 0)
			return true;
	return false
}

bool MsgQueue::sendMsgToQueue(DATA *data, int dataSize)
{

}

}
