#pragma once
#include "pch.h"

#define MAX_BUF 4096

typedef struct OVERLAPPED_EX
{
	OVERLAPPED overlap;
	INT data;
};

enum MSG_TYPE : BYTE
{
	LOGIN,
	LOGOUT
};