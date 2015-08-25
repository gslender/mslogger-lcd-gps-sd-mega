#define DEBUG // comment out this line to disable
#ifdef DEBUG
	#include <SoftwareSerial.h>
	extern SoftwareSerial* debugSerial;
	//// the following must be in an impl file  D(SoftwareSerial* debugSerial;)
	#define D(x) x
#else
	#define D(x)
#endif
