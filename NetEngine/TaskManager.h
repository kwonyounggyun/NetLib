#pragma once
#include "CircularQueue.h"
#include "NetMessage.h"

class TaskManager
{
private:
	ConcurrentCircularQueue<NetMessage*> msg_queue;
	
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
		return msg_queue.Push(msg);
	}

	BOOL PopMsg(NetMessage* msg)
	{
		return msg_queue.Pop(msg);
	}
};

