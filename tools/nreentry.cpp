#include "nreentry.h"

namespace Common{
	int NEntryTry::_count = 0;

	NEntryTry::NEntryTry()
	{
		if( __sync_fetch_and_add(&NEntryTry::_count, 1) > 0 )
			{
				__sync_fetch_and_sub(&NEntryTry::_count, 1);
				throw EntryException( "failed to entry the codeing", __FILE__, __LINE__, 0);
			}
	}
	
	NEntryTry::~NEntryTry()
	{
		__sync_sub_and_fetch(&NEntryTry::_count, 1);
	}




}
