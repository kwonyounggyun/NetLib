#include "stdafx.h"
#include "Crypt.h"

const INT C1 = 52845;
const INT C2 = 22719;
const	INT KEY = 72957;

BOOL CCrypt::Encrypt(BYTE * source, BYTE * destination, DWORD length)
{
	DWORD i;
	INT key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i = 0; i < length; i++)
	{
		destination[i] = source[i] ^ key >> 8;
		key = (destination[i] + key)*C1 + C2;
	}
	return TRUE;
}

BOOL CCrypt::Decrypt(BYTE * source, BYTE * destination, DWORD length)
{
	DWORD i;
	BYTE PreviousBlock;
	INT key = KEY;

	if (!source || !destination || length <= 0)
		return FALSE;

	for (i = 0; i < length; i++)
	{
		PreviousBlock = source[i];
		destination[i] = source[i] ^ key >> 8;
		key = (PreviousBlock + key)*C1 + C2;
	}

	return TRUE;
}
