#include "error.h"

#define TRY(times, statement) do { 		\
	int c = (times);			\
	while(c > 0) {				\
		usleep(300000);			\
		if((statement)) break;		\
		c--;				\
	}					\
	if(!c) {				\
		ERROR_COUNT("Tries exceeded", (times));	\
	}					\
   else if(c < times -1) \
   { \
      INFO_COUNT("Tries", times - c);	\
   } \
} while(0)
