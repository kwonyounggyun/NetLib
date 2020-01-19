#include "pch.h"
#include "TCPSession.h"

BOOL TCPSession::Read(DWORD data_length)
{
	if (data_length <= 0)
		return FALSE;

	DWORD recv_byte = 0;
	DWORD recv_flag = 0;
	DWORD recv_offset = 0;

	//���⼭ ��Ŷ �и� ���� �̷��� �Ұ� �ƴ϶� �׳� read���۸� ����ϴ°� ���� ��뵵�ȵ�� �����ϴ�.
	while (ReadForIOCP(m_tcp_buffer + m_tcp_buf_write_pos, MAX_BUF - m_tcp_buf_write_pos, recv_offset, data_length))
	{
		if (GetPacket(m_tcp_buffer, data_length, m_tcp_buf_write_pos) == FALSE)
		{
			End();

			return FALSE;
		}
	}

	return Session::ReadTCP();
}

/*
FALSE�� ��ȯ�Ǹ� �߸��� ��Ŷ ���°ǵ� �߸��� ��Ŷ�� ���� ���� READ�ʿ��� ������ �����ع�����.
*/
BOOL TCPSession::GetPacket(BYTE* buf, DWORD data_length, USHORT& remain_pos)
{
	USHORT length = 0;
	BYTE* buf_read_pos = buf;
	USHORT remain_length = data_length;
	USHORT length_size = sizeof(length);
	//1����Ʈ�� ������ ��쵵 ����������? ��ũ��ũ�� ���Ȱ��

	while (remain_length > 0)
	{
		if (remain_msg != nullptr)
		{
			/*
			remain�� ���� ���� GetPacket�� ����Ǿ������� ���´�. ������ �����Ͱ� ������ ������ remain�� �ϼ��ȴ�.
			�ֳĸ� iocp�� ���� ������ ���۸�ŭ ������ tcp����� �Ϸ�Ǿ����� ��ȣ�� �Ϸ��ȣ�� ������. ���� ���� ũ�⸦ �����ϰ� �ش� ���ۺ��� ū �����͸� ������ ���� ���̴�.
			�׷��� ������ �̺�Ʈ�� �߻��ϸ� ������ ��Ŷ�� �ϼ��ȴ�.
			*/
			remain_msg->WriteByte(buf_read_pos, remain_size);
			TaskManager::GetInstance().PushMsg(remain_msg);
			remain_msg = nullptr;
			remain_length -= remain_size;
			remain_size = 0;
			continue;
		}

		if (remain_length < length_size)
		{
			//������ ������ ����
			CopyMemory(buf, buf_read_pos, remain_length);
			remain_pos = remain_length;
			break;
		}
		remain_pos = 0;

		CopyMemory(&length, buf_read_pos, length_size);
		remain_length -= length_size;
		buf_read_pos += length_size;

		if (length <= 0 || length > MAX_BUF)
			return FALSE;

		NetMessage* msg = NetMessage::NewMsg();
		if (remain_length < length)
		{
			msg->WriteByte(buf_read_pos, remain_length);
			remain_msg = msg;
			remain_size = length - remain_length;
			break;
		}
		else
		{
			msg->WriteByte(buf_read_pos, length);
			TaskManager::GetInstance().PushMsg(msg);
		}

		buf_read_pos += length;
		remain_length -= length;
	}

	return TRUE;
}

BOOL TCPSession::Write(NetMessage* msg)
{	
	WriteTCP(msg, msg->GetSize());

	m_write_msg_queue.Push(msg);

	return TRUE;
}

BOOL TCPSession::WirteComplete()
{
	NetMessage* msg;

	if (m_write_msg_queue.Pop(msg))
		return FALSE;

	delete msg;

	return TRUE;
}