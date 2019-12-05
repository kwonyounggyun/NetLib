#include "stdafx.h"
#include "Stream.h"


CStream::CStream()
{
	mBufferPointer = NULL;
	mLength = 0;
}


CStream::~CStream()
{
}

BOOL CStream::SetBuffer(BYTE * buffer)
{
	if (!buffer)
		return FALSE;

	mBufferPointer = buffer;
	mLength = 0;

	return TRUE;
}

BOOL CStream::ReadInt32(INT * data)
{
	CopyMemory(data, mBufferPointer+mLength, sizeof(INT));

	mLength += sizeof(INT);

	return TRUE;
}

BOOL CStream::ReadDWORD(DWORD * data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD));

	mLength += sizeof(DWORD);

	return TRUE;
}

BOOL CStream::ReadDWORD_PTR(DWORD_PTR * data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD_PTR));

	mLength += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL CStream::ReadByte(BYTE * data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(BYTE));

	mLength += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::ReadBytes(BYTE * data, DWORD length)
{
	CopyMemory(data, mBufferPointer + mLength, length);

	mLength += length;

	return TRUE;
}

BOOL CStream::ReadFloat(FLOAT * data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(FLOAT));

	mLength += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::ReadInt64(INT64 * data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT64));

	mLength += sizeof(INT64);

	return TRUE;
}

BOOL CStream::ReadUSHORT(USHORT * data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(USHORT));

	mLength += sizeof(USHORT);

	return TRUE;
}

BOOL CStream::ReadSHORT(SHORT * data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(SHORT));

	mLength += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::ReadBOOL(BOOL * data)
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(BOOL));

	mLength += sizeof(BOOL);

	return TRUE;
}

BOOL CStream::WriteInt32(INT data)
{
	CopyMemory(mBufferPointer+mLength, &data, sizeof(INT));

	mLength += sizeof(INT);

	return TRUE;
}

BOOL CStream::WriteDWORD(DWORD data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(DWORD));

	mLength += sizeof(DWORD);

	return TRUE;
}

BOOL CStream::WriteDWORD_PTR(DWORD_PTR data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(DWORD_PTR));

	mLength += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL CStream::WriteByte(BYTE data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(BYTE));

	mLength += sizeof(BYTE);

	return TRUE;
}

BOOL CStream::WriteBytes(BYTE * data, DWORD length)
{
	CopyMemory(mBufferPointer + mLength, &data, length);

	mLength += length;

	return TRUE;
}

BOOL CStream::WriteFloat(FLOAT data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(FLOAT));

	mLength += sizeof(FLOAT);

	return TRUE;
}

BOOL CStream::WriteInt64(INT64 data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(INT64));

	mLength += sizeof(INT64);

	return TRUE;
}

BOOL CStream::WriteUSHORT(USHORT data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(USHORT));

	mLength += sizeof(USHORT);

	return TRUE;
}

BOOL CStream::WriteSHORT(SHORT data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(SHORT));

	mLength += sizeof(SHORT);

	return TRUE;
}

BOOL CStream::WriteBOOL(BOOL data)
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(BOOL));

	mLength += sizeof(BOOL);

	return TRUE;
}

DWORD CStream::GetLength(VOID)
{
	return mLength;
}
