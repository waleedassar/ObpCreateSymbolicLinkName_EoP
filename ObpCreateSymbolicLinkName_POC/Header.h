#pragma once
#include "windows.h"


#define ulong unsigned long
#define ulonglong unsigned long long
#define longlong long long
#define ULONG unsigned long
#define ULONGLONG unsigned long long
#define ushort unsigned short
#define USHORT unsigned short




#define OBJ_PROTECT_CLOSE 		0x00000001
#define OBJ_INHERIT             0x00000002
#define OBJ_AUDIT_OBJECT_CLOSE  0x00000004
#define OBJ_PERMANENT           0x00000010
#define OBJ_EXCLUSIVE           0x00000020
#define OBJ_CASE_INSENSITIVE    0x00000040
#define OBJ_OPENIF              0x00000080
#define OBJ_OPENLINK            0x00000100
#define OBJ_KERNEL_HANDLE       0x00000200
#define OBJ_FORCE_ACCESS_CHECK  0x00000400
#define OBJ_VALID_ATTRIBUTES    0x000007F2
#define OBJ_CREATOR_INFO_MINE   0x00010000


#define SYMBOLIC_LINK_QUERY 0x0001
#define SYMBOLIC_LINK_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0x1)



typedef struct _IO_STATUS_BLOCK {
  union {
    int Status;
    void*    Pointer;
  } DUMMYUNIONNAME;
  ulonglong Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

struct _UNICODE_STRING
{
	unsigned short Length;
	unsigned short MaxLength;
	unsigned long Pad;
	wchar_t* Buffer;
};

struct _OBJECT_ATTRIBUTES
{
  ULONGLONG           Length;
  HANDLE          RootDirectory;
  _UNICODE_STRING* ObjectName;
  ULONGLONG           Attributes;
  PVOID           SecurityDescriptor;
  PVOID           SecurityQualityOfService;
};



#define ProcessDeviceMap 0x17

extern "C"
{
	int ZwOpenDirectoryObject(HANDLE*,int DesiredAccess,_OBJECT_ATTRIBUTES*);
	int ZwQueryDirectoryObject(HANDLE DirectoryHandle,  void* Buffer, ulonglong Length, bool ReturnSingleEntry, bool RestartScan,  ulonglong* Context,  ulonglong* ReturnLength);
	int ZwCreateDirectoryObjectEx(HANDLE* DirectoryHandle, ACCESS_MASK DesiredAccess, _OBJECT_ATTRIBUTES* ObjectAttributes, HANDLE ShadowDirectoryHandle, ulonglong Flags);

	int ZwClose(HANDLE Handle);
	int ZwCreateSymbolicLinkObject(HANDLE* LinkHandle, ACCESS_MASK DesiredAccess, _OBJECT_ATTRIBUTES* ObjectAttributes, _UNICODE_STRING* LinkTarget);
	int ZwOpenSymbolicLinkObject(HANDLE* LinkHandle, ACCESS_MASK DesiredAccess, _OBJECT_ATTRIBUTES* ObjectAttributes);
	int ZwQuerySymbolicLinkObject(HANDLE LinkHandle,  _UNICODE_STRING* LinkTarget,  ulonglong* ReturnedLength);


	int ZwMakePermanentObject(HANDLE Handle); 
	int ZwMakeTemporaryObject(HANDLE Handle); 


	int ZwSetInformationProcess(HANDLE ProcessHandle, 
						ulonglong ProcessInformationClass,  
						void* ProcessInformation, 
						ulonglong ProcessInformationLength);
}
