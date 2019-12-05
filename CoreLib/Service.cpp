#include "stdafx.h"
#include "Service.h"
#include <functional>

VOID __stdcall RunCallback(DWORD ArgumentCount, LPTSTR* arguments)
{

}

VOID __stdcall CtrlHandlerCallback(DWORD opCode)
{

}

CService::CService():mServiceName(nullptr), mServiceStatusHandle(NULL)
{
	ZeroMemory(&mServiceStatus, sizeof(_SERVICE_STATUS));
}

CService::~CService()
{
}

BOOL CService::Install(LPCTSTR serviceName)
{
	TCHAR ServiceFileName[MAX_PATH] = { 0, };
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (!serviceName)
	{
		printf("service name null ~\n");
		return FALSE;
	}

	ServiceFileName[0] = '"';
	GetModuleFileName(NULL, ServiceFileName + 1, MAX_PATH - 1);
	_tcscat_s(ServiceFileName, _T("\""));

	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!ServiceControlManager)
	{
		printf("service manager open fail\n");
		return FALSE;
	}

	ServiceHandle = CreateService(ServiceControlManager,
		serviceName,
		serviceName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		ServiceFileName,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if (!ServiceHandle)
	{
		printf("service create fail\n");
		CloseServiceHandle(ServiceControlManager);
		return FALSE;
	}

	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

BOOL CService::Uninstall(LPCTSTR serviceName)
{
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	printf("starting uninstall\n");
	if (!serviceName)
	{
		printf("service name null ~\n");
		return FALSE;
	}

	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!ServiceControlManager)
	{
		printf("service manager open fail\n");
		return FALSE;
	}

	ServiceHandle = OpenService(ServiceControlManager, serviceName, DELETE);

	if (!ServiceHandle)
	{
		printf("service open fail\n");
		CloseServiceHandle(ServiceControlManager);
		return FALSE;
	}

	if (!DeleteService(ServiceHandle))
	{
		printf("service delete faile\n");
		CloseServiceHandle(ServiceHandle);
		CloseServiceHandle(ServiceControlManager);
		
		return FALSE;
	}

	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

BOOL CService::Begin(LPTSTR serviceName)
{
	if(!serviceName)
		return FALSE;

	SERVICE_TABLE_ENTRY DispatchTable[] = 
	{
		{serviceName, ::RunCallback}, {NULL, NULL}
	};

	_tcscpy_s(mServiceName, sizeof(serviceName), serviceName);
	_tprintf(_T("test"));
	OnStarted();
	
	if (!StartServiceCtrlDispatcher(DispatchTable))
	{
		_tprintf(_T("## Debuf console mode ##\n"));
		INT tmp = getchar();
	}
	return TRUE;
}

BOOL CService::End(VOID)
{
	OnStopped();

	return TRUE;
}

VOID CService::RunCallback(DWORD ArgumentCount, LPTSTR *arguments)
{
	DWORD Status;
	mServiceStatus.dwServiceType = SERVICE_WIN32;
	mServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	mServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;

	mServiceStatus.dwWin32ExitCode = 0;
	mServiceStatus.dwServiceSpecificExitCode = 0;
	mServiceStatus.dwCheckPoint = 0;
	mServiceStatus.dwWaitHint = 0;

	mServiceStatusHandle = RegisterServiceCtrlHandler(mServiceName, ::CtrlHandlerCallback);

	if (mServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
		return;

	Status = NO_ERROR;

	mServiceStatus.dwCurrentState = SERVICE_RUNNING;
	mServiceStatus.dwCheckPoint = 0;
	mServiceStatus.dwWaitHint = 0;

	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus))
		return;
}



VOID CService::CtrlHandlerCallback(DWORD opCode)
{
	switch (opCode)
	{
	case SERVICE_CONTROL_PAUSE:
		mServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		mServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:
		mServiceStatus.dwWin32ExitCode = 0;
		mServiceStatus.dwCurrentState = SERVICE_STOPPED;
		mServiceStatus.dwCheckPoint = 0;
		mServiceStatus.dwWaitHint = 0;
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	}

	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus))
		return;
}