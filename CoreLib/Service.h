#pragma once

class CService
{
private:
	LPTSTR mServiceName;
	_SERVICE_STATUS mServiceStatus;
	SERVICE_STATUS_HANDLE mServiceStatusHandle;

protected:
	virtual VOID OnStarted(VOID) = 0;
	virtual VOID OnStopped(VOID) = 0;
	VOID RunCallback(DWORD ArgumentCount, LPTSTR *arguments);
	VOID CtrlHandlerCallback(DWORD opCode);
	
public:
	explicit CService();
	virtual ~CService();

	BOOL Install(LPCTSTR serviceName);
	BOOL Uninstall(LPCTSTR serviceName);
	BOOL Begin(LPTSTR serviceName);
	BOOL End(VOID);
};
