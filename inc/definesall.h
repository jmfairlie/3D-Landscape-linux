#ifndef __DEFINES_ALL_H__
#define __DEFINES_ALL_H__

#include <errors.h>

#ifdef __WIN32__
#define u_int unsigned int
#define u_short unsigned short
#endif

//selection buffer size
#define SELBUFSIZE 512

#define INFINITYM 9999999.0F

#ifdef __WIN32__
#define bzero(x, y) memset(x, 0, y)
#endif

#endif //__DEFINES_ALL_H__
