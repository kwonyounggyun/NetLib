#pragma once
#include "pch.h"
#include "Define.h"
#include "MemoryPool.h"

//�̰͵� ����ڰ� �ڱ� �Ը��� �°� �ۼ��ϵ��� �������̽��� �վ�����
class NetMessage : public CMemoryPool<NetMessage, 1000>
{
	friend class Session;

public:
	const static DWORD HEADER_SIZE = sizeof(MSG_TYPE) + sizeof(DWORD);
	const static DWORD MSG_BUF_SIZE = MAX_BUF - HEADER_SIZE;

private:
	BYTE header_[HEADER_SIZE];
	BYTE data_[MSG_BUF_SIZE];
	
public:
	//���ۺ��� Ŭ��� �߸��� �����ͷ� �Ǵ�����
	BOOL operator ()(BYTE* data, DWORD size)
	{
		if (size > (MSG_BUF_SIZE - size_))
			return FALSE;

		CopyMemory(buf+size_, data, size);
		size_ += size;

		return TRUE;
	}

	explicit NetMessage() :size_(0)
	{
		ZeroMemory(buf, MSG_BUF_SIZE);
	}
	
	NetMessage(MSG_TYPE type):type_(type), size_(0)
	{
		ZeroMemory(buf, MSG_BUF_SIZE);
	}

	BOOL BufferClear()
	{
		size_ = 0;
		ZeroMemory(buf, MSG_BUF_SIZE);
	}
};