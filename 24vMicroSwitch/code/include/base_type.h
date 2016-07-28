#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__
#include <stdio.h>
#include "platform.h"

typedef unsigned long H_U32;
typedef unsigned short H_U16;
typedef unsigned char H_U8;
typedef long H_S32;
typedef short H_S16;
typedef char H_S8;
typedef int H_BOOL;

typedef unsigned char BYTE;
typedef unsigned int WORD;


#define H_SUCCESS			(0)
#define H_FAILURE			(~0)
#define H_TRUE				(1)
#define H_FAUSE				(0)
#define H_NULL				(0)

#endif