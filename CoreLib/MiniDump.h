#pragma once

class CMiniDump
{
private:
	static LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter;
public:
	CMiniDump();
	~CMiniDump();
	BOOL Begin(VOID);
	BOOL End(VOID);
};

LPTOP_LEVEL_EXCEPTION_FILTER CMiniDump::PreviousExceptionFilter = nullptr;