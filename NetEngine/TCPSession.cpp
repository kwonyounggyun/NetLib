#include "pch.h"
#include "TCPSession.h"

BOOL TCPSession::Read(DWORD data_length)
{
	if (data_length <= 0)
		return FALSE;

	DWORD recv_byte = 0;
	DWORD recv_flag = 0;
	DWORD recv_offset = 0;

	//여기서 패킷 분리 굳이 이렇게 할게 아니라 그냥 read버퍼를 사용하는게 복사 비용도안들고 좋긴하다.
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
FALSE가 반환되면 잘못된 패킷 들어온건데 잘못된 패킷이 들어온 순간 READ쪽에서 세션을 종료해버린다.
*/
BOOL TCPSession::GetPacket(BYTE* buf, DWORD data_length, USHORT& remain_pos)
{
	USHORT length = 0;
	BYTE* buf_read_pos = buf;
	USHORT remain_length = data_length;
	USHORT length_size = sizeof(length);
	//1바이트만 들어오는 경우도 잇지않을까? 네크워크가 떡된경우

	while (remain_length > 0)
	{
		if (remain_msg != nullptr)
		{
			/*
			remain이 남는 경우는 GetPacket이 종료되었을때만 남는다. 다음번 데이터가 들어오면 무조건 remain이 완성된다.
			왜냐면 iocp는 내가 지정한 버퍼만큼 읽으며 tcp통신이 완료되었을때 신호를 완료신호를 보낸다. 나는 버퍼 크기를 지정하고 해당 버퍼보다 큰 데이터를 보내지 않을 것이다.
			그래서 다음번 이벤트가 발생하면 무조건 패킷이 완성된다.
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
			//데이터 앞으로 당기기
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