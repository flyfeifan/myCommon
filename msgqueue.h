#ifndef __MSG_QUEUE_H
#define __MSG_QUEUE_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string>
#include <sstream>
#include <stdexcept>

namespace Common{

class QueueException : public std::runtime_error{
	public:
		explicit QueueException(std::string s, std::string file, int line, int err):_line(line),_filename(file), std::runtime_error(s), _errno(err){ }
		explicit QueueException(std::string& s):std::runtime_error(s){ }
		virtual ~QueueException() throw() { }

		int _line;
		std::string _filename;
		int _errno;
	
		virtual const char* what() const throw(){
			std::ostringstream formatStr;
			formatStr << "file: " << _filename<<":"<<_line<<" errno:"<<_errno<<" message:"<<std::runtime_error::what();
			return formatStr.str().c_str();
		}
};
/*
template <typename DATA>
struct _QueueData{
	long _msgtype;
	DATA &_data;
	_QueueData(int type, DATA& data):_msgtype(type), _data(data){ }
};
*/
template <typename DATA>
class MsgQueue{
	public:
	struct _QueueData{
		long _msgtype;
		DATA& _data;
		_QueueData(int type, DATA& data):_msgtype(type), _data(data){ }
	};

	public:
		MsgQueue(const std::string path="/home/ketao/workplace/test/tmp.txt", int id = 10)
		{
			_key = ftok(path.c_str(), id);
			if( -1 == _key)
				throw QueueException("init msgQueue failed, create key failed.", __FILE__, __LINE__, errno);

			_msgid = msgget(_key, IPC_CREAT);
			if( _msgid == -1 )
				throw QueueException("init msgQueue failed, msgget failed.", __FILE__, __LINE__, errno);
		}
		~MsgQueue()
		{

		}
	public:
		virtual bool deleteMsgid()
		{
			if(_msgid > 0 )
				if(msgctl(_msgid, IPC_RMID, 0) == 0)
					return true;
			return false;
		}

		virtual bool sendMsgToQueue(DATA& data, long msgSize,long msgtype)
		{
			if( _msgid < 0 )
				_msgid = msgget(_key, IPC_CREAT);
			if( _msgid == -1 )
				//throw QueueException("init msgQueue failed, msgget failed.", __FILE__, __LINE__, errno);
				return false;

			_QueueData queueData(msgtype, data);

			if( -1 == msgsnd(_msgid, (const void *)&queueData, msgSize, IPC_NOWAIT) )
			{
				int a = errno;
				if(EIDRM == errno)
					_msgid = -1;
				return false;
			}
			return true;
		}
		virtual bool rcvMsgFromQueue(DATA& data, long msgSize, long msgtype)
		{
			if( _msgid < 0 )
				_msgid = msgget(_key, IPC_CREAT);
			if( _msgid == -1 )
				return false;

			_QueueData queueData(msgtype, data);

			if( -1 == msgrcv(_msgid, (void *)(&queueData), msgSize, 0, IPC_NOWAIT) )
			{
				if(EIDRM == errno)
					_msgid = -1;
				return false;
			}
			return true;
		}
	protected:
		key_t   _key;
		int     _msgid;
};

}
#endif
