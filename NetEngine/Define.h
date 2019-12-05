#pragma once
#include "pch.h"

#define MAX_BUF 4096

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED overlap;
	INT data;
}OVERLAPPED_EX;

enum MSG_TYPE : BYTE
{
	LOGIN,
	LOGOUT
};