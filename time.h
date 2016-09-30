#include <sys/time.h>
#include <unistd.h>

typedef  long long    int64_t;
typedef  unsigned long long   uint64_t;

namespace Common {

	class Time {
		static  int64_t  now() { return (int64_t)time(0); }

	};



}
