#pragma once
#include "CircularQueue2.h"
#include "NetMessage.h"

class TaskManager
{
private:
	CCircularQueue2<NetMessage*> msg_queue;
	
	TaskManager() {}

public:
	~TaskManager() {}
	static TaskManager& GetInstance()
	{
		static TaskManager taskManager;
		return taskManager;
	}

	BOOL PushMsg(NetMessage* msg)
	{
		msg_queue.Push(msg);
	}

	BOOL PopMsg(NetMessage* msg)
	{
		msg_queue.Pop(msg);
	}
};

