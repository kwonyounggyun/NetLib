#pragma once
class CRandom
{
public:
	CRandom();
	~CRandom();

private:
	UINT mSeed;

public:
	BOOL Init(UINT seed);

	INT Rand(VOID);
	INT SimpleRandom(VOID);
	INT ComplexRandom(VOID);
};

