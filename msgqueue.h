#ifndef __MSG_QUEUE_H
#define __MSG_QUEUE_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
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
			return formatStr.str().c_Str();
};

template <typename DATA>
struct _QueueData{
	long _msgtype;
	DATA &_data;
	_QueueData(int type, DATA& data):_msgtype(type), _data(data){ }
};

class MsgQueue{
	public:
		MsgQueue(const std::string path=".", int id = 2016);
		~MsgQueue();
	public:
		virtual bool deleteMsgid();
		virtual bool sendMsgToQueue(DATA*, int);
		virtual bool rcvMsgFromQueue(DATA*, );
	protected:
		key_t   _key;
		int     _msgid;
};



}
#endif
