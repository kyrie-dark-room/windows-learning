#pragma once
#include <Windows.h>
#include <imagehlp.h>
/*
* ֪ʶ�㣺
* 1��FreeLibraryAndExitThread������Kernel32.dll�е�ʵ�����£�
*		VOID FreeLibraryAndExitThread(HMODULE hLibModule, DWORD dwExitCode){
*			FreeLibrary(hLibModule);
*			ExitThread(dwExitCode);
*		}
*	 ע�⣺�����DLL���̷ֱ߳�FreeLibrary��ExitThread����ô�����һ�����ص����⡣����������FreeLibrary�������ӽ��̵ĵ�ַ�ռ��г�����DLL��ӳ�䡣��FreeLibrary���÷��ص�ʱ��
*	����ExitThread�Ĵ����Ѿ����������ˣ��߳���ͼִ�е��ǲ����ڵĴ��롣�⽫��������Υ�棬�������������̱���ֹ��
*	 ���ǣ�����̵߳���FreeLibraryAndExitThread����ô������������FreeLibrary����ʹ��DLL��ӳ���������������Ҫִ����һ��ָ������Kernel32.dll�У����������Ѿ�������ӳ���DLL�С�
*	����ζ���߳̿��Լ���ִ�в�����ExitThread��ExitThread��ʹ�߳���ֹ���Ҳ��ٷ��ء�
* 2������LoadLibrary��LoadLibraryEx���ܻᵼ�½�ͬһ��DLLӳ�䵽ͬһ����ַ�ռ��еĲ�ͬλ�á���������LOAD_LIBRARY_AS_DATAFILE��LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE
	��LOAD_LIBRARY_AS_IMAGE_RESOURCE��־����LoadLibraryEx��ʱ�򣬲���ϵͳ���ȼ���DLL�Ƿ��Ѿ���LoadLibrary��LoadLibraryEx����û��ʹ����Щ��־�������������Ѿ������������ô
	�����᷵�ص�ַ�ռ���DLLԭ���Ѿ���ӳ�䵽�ĵ�ַ�����ǣ����DLL��δ�����룬��ôWindows�Ὣ��DLL���뵽��ַ�ռ���һ�����õĵ�ַ��������Ϊ����һ����ȫ�����DLL����ʱ��������
	ģ����������GetModuleFileName����ô�õ��ķ���ֵ��Ϊ0������һ�ַǳ��õķ���������������֪����һ��DLL���Ӧ��ģ��������������̬����������޷�ͨ��GetProcAddress���õ�����
	��ַ���Ժ������е��á�
* 3��Ϊ����DLL������ʹ�ã�Microsoft Visual C++�ṩ��һ��ܰ������ԣ����ӳ�����DLL��һ���ӳ������DLL����ʽ���ӵģ�ϵͳһ��ʼ���Ὣ��DLL���룬ֻ�е����ǵĴ�����ͼȥ����DLL�а�����
*	һ������ʱ��ϵͳ�Ż�ʵ�������DLL���ӳ�����DLL����������·ǳ����á�
*		�����Ӧ�ó���ʹ���˶��DLL����ô���ĳ�ʼ�����ܻ�Ƚ�������Ϊ���س������б����DLLӳ�䵽���̵ĵ�ַ�ռ��С�������������һ�ַ���ʱ��DLL����������ӳٵ����̵�ִ�й����С�
*		����������ڴ����е���һ���µĺ�����Ȼ������ͼ��һ�����ṩ�ú������ϰ汾�Ĳ���ϵͳ�����и����ó�����ô���س���ᱨ��һ�������Ҳ�����Ӧ�ó������С�
*	 �ӳ�����ľ����ԣ�
*		��һ���������ֶΣ������ݣ��༴ȫ�ֱ�������DLL���޷��ӳ�����ġ�
*		��Kernel32.dllģ�����޷��ӳ�����ģ�������Ϊ���������ģ����ܵ���LoadLibrary��GetProcAddress��
*		����Ӧ����DLLMain��ڵ㺯���е���һ���ӳ�����ĺ�������Ϊ�������ܻᵼ�³��������
*	 ���ӿ�ִ���ļ���ʱ�����Ǳ����޸�һЩ���������أ�/DelayLoad:MyDll.dll�����ظ����������������
		��Ҫ��ָ���ĺ���__delayLoadHelper2Ƕ�뵽���ǵĿ�ִ���ļ��С�
		����MyDll.dll�ӿ�ִ��ģ��ĵ������ȥ�������������̳�ʼ����ʱ�򣬲���ϵͳ�ļ��س���Ͳ�����ʽ�������DLL��
		���ڿ�ִ��ģ����Ƕ��һ���µ��ӳ�����Σ���Delay Import section����Ϊ.didata������ʾҪ��MyDll.dll�е�����Щ������
		��ͨ���ö��ӳ����뺯���ĵ�����ת��__delayLoadHelper2����������ɶ��ӳ����뺯���Ľ�����
	 Ӧ�ó������е�ʱ�򣬶��ӳ����뺯���ĵ���ʵ���ϻ����__delayLoadHelper2��������������������Ǹ�������ӳ�����Σ������Ⱥ����LoadLibrary��GetProcAddress��һ���õ��˶�Ӧ��
	�ӳ����뺯���ĵ�ַ��__delayLoadHelper2���޸��Ըú����ĵ��ã��������ĵ��ý�ֱ�ӵ��ø��ӳ����뺯����
	 * Microsoftʵ�ֵ��ӳ�����DLL�ṩ�˸�������ԡ�
		��Ӧ�ó�����Խ�һ���ӳ������DLLж�ء�Ϊ��ж��һ���ӳ������DLL�����Ǳ������������顣���ȣ������ڹ�����ִ���ļ���ʱ��ָ��һ�����������������(/Delay:unload)����Σ�����
		  �޸�Դ���룬��������Ҫж��DLL�ĵط�����__FUnloadDelayLoadedDLL2������
			BOOL __FUnloadDelayLoadedDLL2(PCSTR szDll);
		  ����������/Delay:unload�������������ļ���Ƕ����һ���Ρ�����ΰ�����Ҫ����Ϣ�������������Ѿ����ù��ĺ��������������������Щ������ʱ�򣬻��ٴε���__delayLoadHelper2������
		  ����__FUnloadDelayLoadedDLL2��ʱ����Ҫ����������Ҫж�ص��ӳ������DLL�����֡��ú����������ļ���ж�ضΣ�����DLL���еĺ�����ַ���á�Ȼ��__FUnloadDelayLoadedDLL2�����
		  FreeLibrary��ж�ظ�DLL��
		���ӳ�����DLL����һ�������ǣ���Ĭ�ϵ�����£����ǵ��õĺ����ᱻ�󶨵����̵�ַ�ռ��е�һ���ڴ��ַ�ϣ������ַ��ϵͳ��Ϊ�ú���Ӧ���ڵ�λ�á����ڴ����ɰ󶨵��ӳ�����DLL��
		  �����ӿ�ִ���ļ��Ĵ�С�����������Ҳ֧��һ��/Delay:nobind���ء���Ϊͨ������°󶨶�����Ҫ�ģ���˴����Ӧ�ó���Ӧ��ʹ��������������ء�
		���ӳ�����DLL�����һ��������Ϊ�߼��û���Ƶģ���Ҳ������ʾ��Microsoft��ϸ�ڵĹ�ע����__delayLoadHelper2ִ�е�ʱ�򣬿��Ե���һЩ�����ṩ�Ĺҹ�����(hook function)����Щ
		  �������Խ���__delayLoadHelper2�Ľ���֪ͨ�ʹ���֪ͨ����ˣ���Щ���������Ըı�����DLL�Լ��õ����������ַ�ķ�ʽ��
		  Ϊ�˵õ�֪ͨ�򸲸�Ĭ�ϵ���Ϊ�����Ǳ�����Դ���������������顣
		      �����ȣ������дһ���ҹ��������ҹ�������ԭ��Ϊ��
				typedef FARPROC (WINAPI *pfnDliHook) (unsigned dliNotify, pDelayLoadInfo pdli);
			  ����DelayImp.lib��̬���ӿ���ڲ�������������ȫ�ֱ�����__pfnDliNotifyHook2��__pfnDliFailureHook2�����������������Ͷ���pfnDliHook��
			    DelayImp.lib���ڲ���������������ʼ��ΪNULL�������__delayLoadHelper2��Ҫ�����κιҹ�������Ϊ����ϵͳ�������ǵĹҹ����������Ǳ��뽫����������֮һ��Ϊ�ҹ������ĵ�ַ��

*/

namespace HY_DLL_Manager
{
	class CHYLibraryLoader
	{
	public:
		CHYLibraryLoader();
		~CHYLibraryLoader();

		// HMODULE��ʾ�ļ�ӳ��ӳ�䵽�������ڴ��ַ�����͵ȼ���HINSTANCE�����߿��Ի���
		HMODULE LoadLibrary(LPCWSTR lpLibFileName);
		HMODULE LoadLibraryEx(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);

		// ��Ŀ¼��ӵ����ڲ���Ӧ�ó���� DLL ������·������ӵ�Ĭ������·��˳�򣺽��̵ĵ�ǰĿ¼��ͨ��SetDLLDirectory�����õ��ļ��С�Windows��ϵͳĿ¼��......
		BOOL SetDllDirectory(LPCWSTR lpPathName);
		// ��Ŀ¼��ӵ����ڲ���Ӧ�ó���� DLL ������·��(ע�⣺֧�ֵ�ϵͳ�汾)
		DLL_DIRECTORY_COOKIE AddDllDirectory(PCWSTR NewDirectory);
		// ʹ�� AddDllDirectory ɾ������ӵ����� DLL ����·����Ŀ¼
		BOOL RemoveDllDirectory(DLL_DIRECTORY_COOKIE Cookie);

		// �ͷż��صĶ�̬���ӿ� (DLL) ģ�飬���ڱ�Ҫʱ�ݼ������ü����� �����ü����ﵽ��ʱ�����ӵ��ý��̵ĵ�ַ�ռ�ж��ģ�飬���Ҿ��������Ч
		BOOL FreeLibrary(HMODULE hInstDll);
		// �ݼ����صĶ�̬���ӿ�����ü��� (DLL) ��Ȼ����� ExitThread ��ֹ�����߳�
		void FreeLibraryAndExitThread(HMODULE hLibModule, DWORD dwExitCode);

		// �߳̿���ͨ���������溯�������һ��DLL�Ƿ��Ѿ���ӳ�䵽�˽��̵ĵ�ַ�ռ��У��������ΪNULL���򷵻�Ӧ�ó���Ŀ�ִ���ļ��ľ����
		HMODULE GetModuleHandle(PCTSTR pszModuleName);

		// ��������ָ��ģ����ļ�����ȫ�޶�·����ģ������ɵ�ǰ���̼��أ������NULL��hInstModule��������ô�᷵���������е�Ӧ�ó���Ŀ�ִ���ļ����ļ���
		DWORD GetModuleFileName(HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
		// ��Ҫ�����������̼��ص�ģ����ļ�
		DWORD HYGetModuleFileNameEx(HANDLE hProcess, HMODULE hModule, LPWSTR lpFilename, DWORD nSize);

		// ��ָ���Ķ�̬���ӿ� (DLL) ������������ (Ҳ��Ϊ����) �����
		FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);

		// ����ָ����̬���ӿ� (DLL) ��DLL_THREAD_ATTACH��DLL_THREAD_DETACH֪ͨ�� ����Լ���ĳЩӦ�ó���Ĺ������Ĵ�С
		BOOL DisableThreadLibraryCalls(HMODULE hLibModule);

		// Changes the load address for the specified image, which reduces the required load time for a DLL
		BOOL ReBaseImage64(
			PCSTR   CurrentImageName,
			PCSTR   SymbolPath,
			BOOL    fReBase,
			BOOL    fRebaseSysfileOk,
			BOOL    fGoingDown,
			ULONG   CheckImageSize,
			ULONG* OldImageSize,
			ULONG64* OldImageBase,
			ULONG* NewImageSize,
			ULONG64* NewImageBase,
			ULONG   TimeStamp
		);

		// ���㵼���ÿ�������������ַ
		BOOL BindImageEx(
			DWORD                    Flags,
			PCSTR                    ImageName,
			PCSTR                    DllPath,
			PCSTR                    SymbolPath,
			PIMAGEHLP_STATUS_ROUTINE StatusRoutine
		);

		// Ҫʹ�ö�̬TLS�����Ǳ����ȵ���TlsAlloc�����������ϵͳ�Խ����е�λ��־���м������ҵ�һ��FREE��־��Ȼ��ϵͳ�Ὣ�ñ�־��FREE��ΪINUSE����TlsAlloc���ظñ�־��λ�����е�������
		DWORD TlsAlloc();
		// ��ϵͳ����һ���̵߳�ʱ�򣬻����TLS_MINIMUM_AVAILABLE��PVOIDֵ�������Ƕ���ʼ��Ϊ0�������̹߳���������ÿ���̶߳����Լ���PVOID���飬�����е�ÿ��PVOID���Ա�������ֵ��
		// Ϊ�˰�һ��ֵ�ŵ��̵߳������У�����Ӧ�õ���
		BOOL TlsSetValue(DWORD dwTlsIndex, PVOID pvTlsValue);
		// Ϊ�˴��̵߳�������ȡ��һ��ֵ������Ӧ�õ���
		PVOID TlsGetValue(DWORD dwTlsIndex);
		// �����ǲ�����Ҫһ���Ѿ�Ԥ����TLSԪ��ʱ��Ӧ�õ���TlsFree
		BOOL TlsFree(DWORD dwTlsIndex);

	private:
		CHYLibraryLoader(const CHYLibraryLoader&) = delete;
		CHYLibraryLoader& operator=(const CHYLibraryLoader&) = delete;
		CHYLibraryLoader(CHYLibraryLoader&&) = delete;
		CHYLibraryLoader& operator=(CHYLibraryLoader&&) = delete;
	};
}

