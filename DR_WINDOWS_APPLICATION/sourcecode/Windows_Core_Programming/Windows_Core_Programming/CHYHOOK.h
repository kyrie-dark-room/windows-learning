#pragma once
#include <Windows.h>
#include <commctrl.h>

/*
* ֪ʶ�㣺
* 1��Ӧ�ó�����Ҫ��Խ���̱߽���������һ�����̵ĵ�ַ�ռ��������£�
*	��������Ҫ����һ�����̴����Ĵ����������ര�ڡ�
*	��������ҪһЩ�ֶ�����������--���磬������Ҫȷ����һ����������ʹ����ЩDLL��
*	��������Ҫ����һ�����̰�װ�ҹ���
* 2�����ǿ���ʹ�ö��ֻ�������һ��DLLע�뵽��һ�����̵ĵ�ַ�ռ��У�һ��DLL���������һ����ַ�ռ䣬��ô���ǾͿ������Ǹ�����������������������Ϊ�ˡ���Ӧ�ù����˵ģ�
	�������Ĵ�����ô��֮ǰ����������ؿ��ǡ�
* 3��ʹ��ע�����ע��DLL
*	a.ע����HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows�µļ�ΪAppInit_DLLs��ֵ���ܻ����һ��DLL���ļ�����һ��DLL���ļ�����ͨ���ո�򶺺ŷָ��������ڿո���     �����ָ��ļ����ģ�������Ǳ���������ļ����а����ո񡣵�һ��DLL���ļ������԰���·����������DLL������·���򱻺��ԡ�
*	b.Ϊ������ϵͳʹ�����ע�������ǻ����봴��һ����ΪLoadAppInit_Dlls������ΪDWORD��ע������������ֵ��Ϊ1��
*	c.��User32.dll��ӳ�䵽һ���µĽ���ʱ�����յ�DLL_PROCESS_ATTACH֪ͨ����User32.dll�������д����ʱ�򣬻�ȡ������ע������ֵ��������LoadLibrary����������ַ�����ָ����ÿ��DLL��
*	d.������ע��DLL�����з����У���������һ�֡�������Ҫ����ֻ��������ע��������������ֵ�������ַ���Ҳ��һЩȱ�㣬����������ʾ��
*		�� ���ǵ�DLLֻ�ᱻӳ�䵽��Щʹ����User32.dll�Ľ����С����л���GUI��Ӧ�ó���ʹ����User32.dll�������������CUI��Ӧ�ó��򶼲���ʹ��������ˣ������Ҫ��DLLע�뵽����������������
*		   �����ַ����Ͳ����С�
*		�� ���ǵ�DLL�ᱻӳ�䵽ÿ������GUI��Ӧ�ó����У������ǿ���ֻ���DLLע�뵽һ������������Ӧ�ó����С�
*		�� ���ǵ�DLL�ᱻӳ�䵽ÿ������GUI��Ӧ�ó����У���Ӧ�ó�����ֹ֮ǰ������һֱ�����ڽ��̵ĵ�ַ�ռ��С�
* 4��ʹ��Windows�ҹ���ע��DLL
*		��װ�ҹ���
*			HHOOK hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, hInstDll, 0);
*			��������������һ���������ᷢ��ʲô��
*			a. ����B�е�һ���߳�׼����һ����������һ����Ϣ��
*			b. ϵͳ�����߳��Ƿ��Ѿ���װ��WH_GETMESSAGE�ҹ���
*			c. ϵͳ���GetMsgProc���ڵ�DLL�Ƿ��Ѿ���ӳ�䵽����B�ĵ�ַ�ռ��С�
*			d. ���DLL��δ��ӳ�䣬��ôϵͳ��ǿ�ƽ�DLLӳ�䵽����B�ĵ�ַ�ռ��У���������B�и�DLL������������lock count��������
*			e. ����DLL��hInstDll���ڽ���B��ӳ��ģ����ϵͳ��������м�飬�������DLL�ڽ���A�е�λ���Ƿ���ͬ����ͬ�����ת����
*			f. ϵͳ�ڽ���B�е�����DLL������������
*			g. ϵͳ�ڽ���B�ĵ�ַ�ռ��е���GetMsgProc������
*			h. ��GetMsgProc���ص�ʱ��ϵͳ�ݼ���DLL�ڽ����е�����������
*		ж�عҹ���
*			BOOL UnhookWindowsHookEx(HHOOK hHook);
*			��һ���̵߳���UnhookWindowsHookEx��ʱ��ϵͳ������Լ��ڲ���һ���Ѿ�ע�����DLL�Ľ����б�������DLL�����������ݼ�����������������0��ʱ��ϵͳ���Զ��ӽ��̵ĵ�ַ�ռ�
*			�г����Ը�DLL��ӳ�䡣
*       ��Ŀʵ����Desktop Item Position Saver(DIPS)����
* 5��ʹ��Զ���߳���ע��DLL
		ע��DLL�ĵ����ַ�����ʹ��Զ���̣߳�remote thread�������ṩ����ߵ�����ԡ�
		�����Windows����ֻ����һ�����̶����Լ����в��������ǣ�WindowsҲ�ṩ��һЩ��������һ�����̶���һ�����̽��в�������Ȼ��Щ�����еĴ����һ��ʼ����Ϊ�˵�����������������Ƶģ�
	  �����κ�Ӧ�ó��򶼿��Ե�����Щ������
		�Ӹ�����˵��DLLע�뼼��Ҫ��Ŀ������е�һ���̵߳���LoadLibrary������������Ҫ��DLL���������ǲ������׵ؿ��Ʊ��˽����е��̣߳�������ַ���Ҫ��������Ŀ������д���һ���µ��̡߳�
	  ��������߳��������Լ������ģ�������ǿ��Զ���ִ�еĴ�����Կ��ơ�
	    ע�벽��
		a.��VirtualAllocEx������Զ�̽��̵ĵ�ַ�ռ��з���һ���ڴ档
		b.��WriteProcessMemory������DLL��·�������Ƶ�a�з�����ڴ��С�
		c.��GetProcAddress�������õ�LoadLibraryW��LoadLibraryA��������Kernel32.dll�У���ʵ�ʵ�ַ��
		d.��CreateRemoteThread������Զ���߳��д���һ���̣߳������̵߳�����ȷLoadLibrary�������ڲ����д���a������ڴ��ַ����ʱ��DLL�Ѿ���ע�뵽Զ�̽��̵ĵ�ַ�ռ��У�DLL��DllMain����
		  ���յ�DLL_PROCESS_ATTACH֪ͨ���ҿ���ִ��������Ҫִ�еĴ��롣��DllMain���ص�ʱ��Զ���̻߳��LoadLibraryW/A���÷��ص�BaseThreadStart������BaseThreadStartȻ�����ExitThread��
		  ʹԶ���߳���ֹ��
		������
		e.��VirtualFreeEx���ͷŵ�a��������ڴ档
		f.��GetProcAddress���õ�FreeLibrary��������Kernel32.dll�У���ʵ�ʵ�ַ��
		g.��CreateRemoteThread������Զ�̽����д���һ���̣߳��ø��̵߳���FreeLibrary�������ڲ����д���Զ��DLL��HMODULE��
		ʾ����
		    a.PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
		    b.HANDLE hThread = CreateRemoteThread(hProcessRemote, NULL, 0, pfnThreadRtn, L"C:\\MyLib.dll", 0, NULL);
* 6��һЩ�������ñ���ϵͳ�˺����еģ���WinLogon��SvcHost��Csrss����¼���û����޷�����Щ���̽����޸ĵġ���������Ѿ���õ��԰�ȫ��Ȩ��debug security privalege������Ȩ������Ҳ������
*	���԰�ȫ��Ȩ����ô����Ҳ���ܹ�����Щ���̵ľ����
* 7��ʹ��ľ��DLL��ע��DLL
*		a.������֪���Ľ��̱�Ȼ�������һ��DLL�滻�����������Լ���DLL���ڲ������ǵ���ԭ��DLL���������з��š�
*		b.�޸�Ӧ�ó���.exeģ��ĵ���Ρ����ַ��������ǰ�������Ǳ����൱��Ϥ.exe��DLL���ļ���ʽ��
* 8����DLL��Ϊ��������ע��
*	 �����������ڱ����Խ�����ִ���������Ĳ�����ϵͳ����һ�������Գ���debuggee����ʱ�򣬻��ڱ����Գ���ĵ�ַ�ռ�׼�����֮�󣬵������Գ�������߳���δ��ʼִ���κδ���֮ǰ���Զ�
*	֪ͨ����������ʱ������������ǿ�ƽ�һЩ����ע�뵽�����Գ���ĵ�ַ�ռ��У�����ʹ��WriteProcessMemory����Ȼ���ñ����Գ�������߳�ȥִ����Щ���롣
*	 ���ַ���Ҫ�����ǶԱ������̵߳�CONTEXT�ṹ���̲�������Ҳ��ζ�����Ǳ����д��CPU��صĴ��롣Ϊ�������ַ������ڲ�ͬ��CPUƽ̨���������������Ǳ����Դ��������޸ġ����⣬���ǿ���
*	�������ֹ���дһЩ�������ñ����Գ���ִ�еĻ�������ָ�
*	 ��Ĭ������£������������ֹ����ôWindows���Զ���ֹ�����Գ��򡣵��ǣ�����������ͨ������DebugSetProcessKillOnExit������FALSE���ı�Ĭ�ϵ���Ϊ���ڲ���ֹһ�����̵�ǰ����ֹͣ����
*	�ý���Ҳ���п��ܵģ���Ҫ�鹦��DebugActiveProcessStop������
* 9��ʹ��CreateProcess��ע�����
*	 �������ý��̶������ӽ��̵����߳�ִ�еĴ�����п��Ƶ�һ�ַ�����
*		a.�ý�������һ����������ӽ��̡�
*		b.��.exeģ����ļ�ͷ��ȡ�����̵߳���ʼ�ڴ��ַ��
*		c.��λ�ڸ��ڴ��ַ���Ļ���ָ���������
*		d.ǿ�ƽ�һЩ�ֶ���д�Ļ���ָ��д�뵽���ڴ��ַ������Щָ��Ӧ�õ���LoadLibrary������һ��DLL��
*		e.���ӽ��̵����ָ̻߳����У��Ӷ�����Щָ��õ�ִ�С�
*		f.�ѱ���������ԭʼָ��ָ�����ʼ��ַ����
*		g.�ý��̴���ʼ��ַ����ִ�У��ͺ���ʲô��û�з�����һ����
*	 ȱ�㣺ֻ�е����ǵĴ����ڸ������е�ʱ�����ǲ��������ַ�����ע��DLL����Ȼ�����ַ�������CPU��أ����Ǳ���Ϊ��ͬ��CPUƽ̨����Ӧ���޸ġ�
* 10������API
*	 ����һ�����Ǵ���
*		a.���ڴ��ж�Ҫ���صĺ������ж�λ���Ӷ��õ������ڴ��ַ��
*		b.�����������ʼ�ļ����ֽڱ��浽�����Լ����ڴ��С�
*		c.��CPU��һ��JUMPָ�����������������ʼ�ļ����ֽڣ�����JUMPָ��������ת�����ǵ�����������ڴ��ַ��
*		d.���ڣ����̵߳��ñ����غ�����ʱ����תָ��ʵ���ϻ���ת�����ǵ������������ʱ�����ǾͿ���ִ���Լ���Ҫִ�е��κδ��롣
*		e.Ϊ�˳����Ժ��������أ����Ǳ����b�б�����ֽڷŻ����غ�����ʼ�ļ����ֽ��С�
*		f.���ǵ��ñ����غ����������Ѿ����ٶ������������ˣ����øú���ִ��������������
*		g.��ԭ���ĺ�������ʱ�������ٴ�ִ��b��c���������ǵ���������������ᱻ���õ���
*		���ַ�������һЩ���صĲ��㣬��ǿ�ҽ��鲻Ҫʹ��������CPU�������ԣ���Σ�����ռʽ�����̻߳����¸������ܹ�����
*	 ��������ͨ���޸�ģ��ĵ����������API
*		Ϊ������һ���ض��ĺ�������������Ҫ���ľ����޸�����ģ��ĵ�����еĵ�ַ������ô�򵥣���ȫ�����ڶ�CPU�������ԡ����ң��������ǲ�û���޸ĺ����Ĵ��룬���Ҳ�Ͳ������߳�ͬ�������⡣
*/

namespace HY_HOOK_Manager
{
	class CHYHOOK
	{
	public:
		CHYHOOK(void);
		~CHYHOOK(void);

		// ����ָ�����ڵ����ԣ�ͬʱ����������һ�������������ര��
		LONG_PTR SetWindowLongPtr(
			HWND     hWnd,
			int      nIndex,
			LONG_PTR dwNewLong
		);

		// Installs or updates a window subclass callback
		BOOL SetWindowSubclass(
			HWND         hWnd,
			SUBCLASSPROC pfnSubclass,
			UINT_PTR     uIdSubclass,
			DWORD_PTR    dwRefData
		);
		// ����ָ����������ص�����������
		BOOL GetWindowSubclass(
			HWND         hWnd,
			SUBCLASSPROC pfnSubclass,
			UINT_PTR     uIdSubclass,
			DWORD_PTR* pdwRefData
		);
		// �Ӵ�����ɾ������ص�
		BOOL RemoveWindowSubclass(
			HWND         hWnd,
			SUBCLASSPROC pfnSubclass,
			UINT_PTR     uIdSubclass
		);
		// Calls the next handler in a window's subclass chain. The last handler in the subclass chain calls the original window procedure for the window
		LRESULT DefSubclassProc(
			HWND   hWnd,
			UINT   uMsg,
			WPARAM wParam,
			LPARAM lParam
		);

		// ��Ӧ�ó�����Ĺҹ����̰�װ���ҹ����С� �㽫��װ�ҹ�����������ϵͳ��ĳЩ���͵��¼��� ��Щ�¼����ض��̻߳�����߳�λ��ͬһ�����е������߳��������
		HHOOK SetWindowsHookEx(
			int       idHook,
			HOOKPROC  lpfn,
			HINSTANCE hmod,
			DWORD     dwThreadId
		);
		// ɾ�� SetWindowsHookEx �����ڹҹ����а�װ�Ĺҹ����̡�
		BOOL UnhookWindowsHookEx(HHOOK hhk);

		// ��ָ�����̵������ַ�ռ��б������ύ������ڴ������״̬�� �ú�����ʼ�������������ڴ�
		LPVOID VirtualAllocEx(
			HANDLE hProcess,
			LPVOID lpAddress,
			SIZE_T dwSize,
			DWORD  flAllocationType,
			DWORD  flProtect
		);

		// һ�����̶���һ�����̵ĵ�ַ�ռ���ж�
		BOOL ReadProcessMemory(
			HANDLE  hProcess,
			LPCVOID lpBaseAddress,
			LPVOID  lpBuffer,
			SIZE_T  nSize,
			SIZE_T* lpNumberOfBytesRead
		);

		// Writes data to an area of memory in a specified process. The entire area to be written to must be accessible or the operation fails
		BOOL WriteProcessMemory(
			HANDLE  hProcess,
			LPVOID  lpBaseAddress,
			LPCVOID lpBuffer,
			SIZE_T  nSize,
			SIZE_T* lpNumberOfBytesWritten
		);

		// ��������һ�����̵������ַ�ռ������е��߳�
		HANDLE CreateRemoteThread(
			HANDLE                 hProcess,
			LPSECURITY_ATTRIBUTES  lpThreadAttributes,
			SIZE_T                 dwStackSize,
			LPTHREAD_START_ROUTINE lpStartAddress,
			LPVOID                 lpParameter,
			DWORD                  dwCreationFlags,
			LPDWORD                lpThreadId
		);

		// �ͷš�ȡ���ύ���ͷŲ��ͷ�ָ�����̵������ַ�ռ��е��ڴ�����
		BOOL VirtualFreeEx(
			HANDLE hProcess,
			LPVOID lpAddress,
			SIZE_T dwSize,
			DWORD  dwFreeType
		);

		// ��һ��ģ��ĵ�����в��Ҷ�һ�����ŵ����ã�����������������ã�������޸ĸ÷��ŵĵ�ַ��
		void ReplaceIATEntryInOneMod(PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller);
	private:
		CHYHOOK(const CHYHOOK&) = delete;
		CHYHOOK& operator=(const CHYHOOK&) = delete;
		CHYHOOK(CHYHOOK&&) = delete;
		CHYHOOK& operator=(CHYHOOK&&) = delete;
	};
}

