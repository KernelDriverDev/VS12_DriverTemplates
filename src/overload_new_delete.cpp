#include<ntddk.h>

#pragma warning (disable: 4996)

void* __cdecl operator new(size_t size)
{
	return ExAllocatePool(PagedPool, size);
}

void* __cdecl operator new[](size_t size)
{
	return ExAllocatePool(PagedPool, size);
}

void __cdecl operator delete(void* p)
{
	if (p != NULL)
	{
		ExFreePool(p);
	}
}

void __cdecl operator delete[](void* p)
{
	if (p != NULL)
	{
		ExFreePool(p);
	}
}

class Myclass
{
public:
	Myclass()
	{
		KdPrint(("构造函数\n"));
	}
	~Myclass()
	{
		KdPrint(("析构函数\n"));
	}
	void* __cdecl operator new(size_t size, POOL_TYPE PoolType = PagedPool)
	{
		KdPrint(("调用new\n"));
		return ExAllocatePool(PoolType, size);
	}

	void __cdecl operator delete(void* p)
	{
		KdPrint(("调用delete\n"));
		if (p != NULL)
		{
			ExFreePool(p);
		}
	}
protected:
private:
};

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	KdPrint(("驱动卸载成功\n"));
}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	PCHAR pstr = new char[100];
	delete[] pstr;

	PCHAR pstr1 = new char;
	delete pstr1;

	Myclass* pmyclass = new Myclass;
	delete pmyclass;

	KdPrint(("驱动加载成功\n"));
	DriverObject->DriverUnload = Unload;

	return STATUS_SUCCESS;
}