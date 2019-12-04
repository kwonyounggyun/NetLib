#pragma once
#include "MemoryPool.h"
#include <Windows.h>

class CustomSocketPool : MemoryPool<SOCKET, 1000>
{
public:
	CustomSocketPool();
	~CustomSocketPool();
};

