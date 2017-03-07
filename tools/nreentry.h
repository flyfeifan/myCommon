#ifndef NO_REENTRY_H
#define NO_REENTRY_H
#include <iostream>
#include<stdexcept> 
#include<sstream>
//#include <stdlib.h>

namespace Common{
class EntryException : public std::runtime_error{
	public:
		explicit EntryException(std::string s, std::string file, int line, int err) : _line(line), _filename(file), std::runtime_error(s),_errno(err){ }
		explicit EntryException(std::string s):std::runtime_error(s){ }
		virtual ~EntryException() throw() { }
		int _line;
		std::string _filename;
		int _errno;
		virtual const char* what() const throw(){
			std::ostringstream  formatStr;
			formatStr << "file: " << _filename << ":"<<_line<< " errno: "<<_errno<<"message: "<<std::runtime_error::what();
			return formatStr.str().c_str();
		}

};

class NEntryFunc{
	public:
		NEntryFunc() : _count(0) { }
		~NEntryFunc(){ }
		bool tryEntry()
		{
			if( __sync_fetch_and_add(&_count, 1) > 0 )
			{
				__sync_fetch_and_sub(&_count, 1);
				return false;
			}
			return true;
		}

		void out()
		{
			__sync_sub_and_fetch(&_count, 1);
		}

		void addEntry()
		{
			__sync_fetch_and_add(&_count, 1);
		}

	private:
		int _count;

};

class NEntryTry{
public:
	NEntryTry();
	~NEntryTry();
private:
	static  int _count;
};

}
#endif

