#pragma once

/*
* ֪ʶ�㣺
* 1���ṹ���쳣����structured exception handling��SEH���������ĺô���������д����ʱ�������ȼ��о����������������������̡���������е�ʱ�������ʲô���⣬ϵͳ�Ჶ��������⣬����
*	֪ͨ���ǡ�
* 2����ʹMicrosoft��SEH����Windowsϵͳ������֮һ�������Լ򻯲���ϵͳ����Ŀ�������������ϵͳ�Ŀ�����Աʹ��SEH����ϵͳ���ӽ�׳��������Ҳ����ʹ��SEH����Ӧ�ó�����ӽ�׳��
* 3��Ϊ����SEH�����������������Ĺ�����Ҫ���ڲ���ϵͳ���ڽ�����뿪�쳣��������ʱ����������������һЩ����Ĵ��룬�Լ�����һЩ����֧��SEH�����ݽṹ���������ṩ�ص�����������ϵͳ
*	���ã��Ա�ϵͳ�����쳣����顣������������׼�����̵�ջ��ܣ�stack frame��������һЩ�ڲ���Ϣ����Щ��Ϣ���ǲ���ϵͳ��Ҫʹ�û������õġ��ñ�����֧��SEH����һ���򵥵����񣬾��㲻ͬ
*	�ı����������Բ�ͬ�ķ�ʽ��ʵ������Ҳ����ʲô��ֵ����顣���˵��ǣ����ǿ���ֱ�����ñ�������SEH��֧�֣�������Ҫ�����������֧������ϸ�ڡ�
* 4����Ҫ�����ṹ���쳣������C++�쳣����C++�쳣��������ʽ�ϱ���Ϊʹ�ùؼ���catch��throw����ͽṹ���쳣�������ʽ��ͬ��Microsoft Visual C++֧���쳣���������ڲ�ʵ������ʵ��������
*	�˱�������Windows����ϵͳ�Ľṹ���쳣�����ܡ�
* 5��SEHʵ���ϰ���������Ĺ��ܣ���ֹ����termination handling�����쳣����exception handling����
* 6����ֹ�������ȷ������һ������飨���������루the guarded body����������˳��ģ���һ������飨��ֹ����������ܱ����ú�ִ�С���ֹ������﷨����ʹ��Microsoft Visual C++������ʱ������
*	��ʾ��
*		__try{
*			// Guarded body
*			...
*		}
*		__finally{
*			// Termination handler
*			...
*		}
*	 __try��__finally�ؼ��ֱ������ֹ���������������֣��������������ֹ������򣩡���ǰ����δ����У�����ϵͳ�ͱ�������Эͬ������֤�˲��ܱ��������벿��������˳���--���������ڱ�����
*	������ʹ����return������goto���ֻ���longjump��䣨���ǵ���ExitProcess��ExitThread��TerminateProcess��TerminateThread����ֹ���̻��̣߳�--��ֹ������򶼻ᱻ���ã���__finally����鶼
	��ִ�С�
* 7�����߿�����֪������������α�֤finally�������try������˳�ǰ��ִ�С�ԭ�������������������ʱ���ᷢ����try���������һ��return��䡣���ǣ��������ͻ�����һЩ�����Ƚ�����ֵ������
*	һ��������������ʱ�����Ȼ����ִ��finally����飬������̱���֮Ϊ�ֲ�չ����local unwind������ȷ�е�˵����ϵͳ��Ϊtry������еĴ�����ǰ�˳���ִ��finally�����ʱ���ͻᷢ���ֲ�չ����
*	һ��finally�����ִ����ϣ�����������������ʱ������ֵ�ͻ᷵�ظ������ĵ����ߡ�
* 8�����쳣����ʱ��finally��Ҳ���ܱ�֤�����ܵõ�ִ�С�
*		����windows XPϵͳ����һ����ջ�ľ��쳣����stack exhaustion exception��������try������finally��ͺ��п��ܵò������л��ᣬ��Ϊ�����ڳ���������WER���붼����û���㹻��
*		  ջ�ռ�ȥ���������������������£����������ǲ����κ���ʾ�ر���ֹ��
*		������쳣����SEH�����жϣ���ֹ�������Ҳ����õ�ִ�С�
*		���������쳣�������쳣���˳������ֹ�������Ҳ���ᱻִ�С�
*	 һ�����鷨���Ǿ���������catch����finally���д��������Ĺ�����������̺��п��ܻ���finally��ִ��ǰͻȻ��ֹ����Ҳ����Ϊʲô��Windows Vistaϵͳ�ϣ����󱨸���̣�WER����������һ������
*	�Ľ����
* 9��Ϊ�˰������Ǿ����ܱ���д����try����ǰ�˳��Ĵ��룬MicrosoftΪ����C/C++������������һ���ؼ��֣�__leave���ؼ���__leave�ᵼ�´���ִ�п�������ת��try��Ľ�β--Ҳ������Ϊ����ת���˱�
*	�����Ŵ�����Ϊ����������£�����ִ�н������ش�try�����finally�飬���Բ�������κζ��⿪����
* 10����ĿǰΪֹ�������Ѿ���ȷ���ֲ��������������ֻ�����finally��ִ�е����Σ�ͬʱǿ�������֡�
*		����try�鵽finally�����������������
*		���ֲ�չ������try���е���ǰ�˳�����goto��longjump��continue��break��return��������𣩽����������ǿ��ת��finally�顣
*		������������--ȫ��չ��--Windows Vista��ǰ��ϵͳ�ϣ�try���еĴ����׳��˷����ڴ�Υ��������쳣������Windows Vista��˵��ȫ��չ����Ĭ��������ǲ��ᱻ�����ģ�����finally���ᱻִ�С�
*	 Ҫȷ������������е���һ�֣����ԣ�ֻ�ܣ���finally�������BOOL AbormalTermination()������ڣ��ɱ�����ʶ�𲢴�������⺯������������Ϊ������������������룬���������ɴ������������
	��������������������һ������ֵ������һ���뵱ǰfinally����ص�try���Ƿ��Ѿ���ǰ�˳���

*****************************************************************************************************************************************************************************************
* 11��CPU���𲶻����ƷǷ��ڴ���ʺ���0��Ϊ�������������⣬һ����⵽��Щ������Ϊ�������׳���Ӧ���쳣����CPU�׳����쳣����Ӳ���쳣������ϵͳ��Ӧ�ó���Ҳ�����׳��쳣����Щ�쳣ͨ������Ϊ
*	����쳣����һ��Ӳ����������쳣���׳�ʱ������ϵͳ������ǵ�Ӧ�ó���һ���鿴�쳣���͵Ļ��ᣬ������Ӧ�ó����Լ���������쳣������Ĵ�����ʾ���쳣���������﷨�ṹ��
*			__try {
*				// Guarded body
*				...
*			}
*			__except (exception filter){
*				// exception handler
*				...
*			}
*	  ��ע��__except�ؼ��֣��κ�ʱ�򴴽�һ��try�飬��������һ��finally������except����顣����try�����ͬʱ��finally���except�飬Ҳ����ͬʱ�ж��finally��except�顣����ȴ���Խ�
*	try-finally��Ƕ����try-except���У�������Ҳ���ԡ�
* 12������ֹ�������ͬ���쳣���˳���exception filter�����쳣�������exception handler����Ҫ�ɲ���ϵͳ������ִ��--���쳣���˳�����ʽ������쳣�������ִ�з��棬�����������Ĺ���
*	ʮ�����ޡ����쳣�׳�ʱ��ϵͳ��λ��except��Ŀ�ʼ���������쳣���˳���ı��ʽ��ֵ��������ʽ��ֵ�ض�Ϊ����������ʶ��֮һ����Щ��ʶ����Microsoft Windows��Excpt.h�ļ��ж��塣
*																	�쳣���˳���ķ���ֵ
*				��������������������������������������������������������������������������������������������������������������������������������������
*													��    ʶ									|						ֵ
*				��������������������������������������������������������������������������������������������������������������������������������������
*					EXCEPTION_EXECUTE_HANDLER													|						1
*				��������������������������������������������������������������������������������������������������������������������������������������
*					EXCEPTION_CONTINUE_SEARCH													|						0
*				��������������������������������������������������������������������������������������������������������������������������������������
*					EXCEPTION_CONTINUE_EXECUTION												|						-1
*				��������������������������������������������������������������������������������������������������������������������������������������
* 13��EXCEPTION_EXECUTE_HANDLER�����ֵ�����ϵ��ڸ���ϵͳ������֪������쳣����Ԥ������쳣��ĳ������»ᷢ����ͬʱ�Ѿ�д��һЩ������������������Щ�������ھ�ִ�аɡ������ǣ�ϵͳ
*	ִ��ȫ��չ������global unwind�����ó���ִ����ת��except����鴦�����쳣���������룩����except��������н�����ϵͳ�ͻ���Ϊ�쳣�Ѿ��õ��˴�����������Ӧ�ó������ִ�С����
*	��������WindowsӦ�ó��򲶻���󣬴������ǣ�������ִ�У��û�������֪���������������󡣵�except���еĴ���ִ����Ϻ󣬴���ִ�д�except���ĵ�һ��������ִ�С�
* 14��ȫ��չ��
*	     void funcOStimpy1(){
*			// 1.Do any processing here.
*			...
*			__try {
*				// 2.Call another function
*				FuncORen1();
*				
*				// Code here nener executes.
*			}
*			__except (6.Evaluate filter.EXCEPTION_EXECUTE_HANDLER){
				// 8.After the unwind, the exception handler executes.
				MessageBox(...);
*			}
*			// 9.Exception handled--continue execution.
			...
*		 }

		 void FuncORen1() {
			DWORD dwTemp = 0;

			// 3.Do any processing here.
			...
			__try{
				// 4.Request permissiong to access protected data.
				WaitForSingleObject(g_hSem, INFINITE);

				// 5.Modify the data.
				// An exception is generated here.
				g_dwProtectedData = 5 / dwTemp;
			}
			__finally{
				// 7.Global unwind occurs because filter evaluated
				// to EXCEPTION_EXECUTE_HANDLER.

				// Allow others to use protected data.
				ReleaseSemaphore(g_hSem, 1, NULL);
			}

			// Continue processing--never executes.
			...
		 }
*		  ע��ǰ������ִ�����ִ�е�˳������ǽṹ���쳣�Ĺ������ơ����Ǵ����ִ����ϵͳ�Ŀ����кܴ������ʹ��SEH������⡣����ִ�п����������Ǵ�ͷ��β��ϵͳ�����Լ�����ͼ����
*		����ÿһ���ֵ�ִ��˳��ϵͳ��ִ��˳����Ȼ���ӣ�����Ȼ�ǿ��ƶϵġ�
*		  Ϊ�˸��õ����ִ�е�˳�������Ǵ���һ����΢��ͬ�ĽǶ�������������ʲô�����쳣���˳��򷵻�EXCEPTION_EXECUTE_HANDLERʱ�������ڸ��߲���ϵͳ����ǰ�߳�ָ��ָ��Ӧ��ָ��except
*		���е�ָ�Ȼ����ʵ���ϵ�ǰָ��ָ��ָ�����try���е�ָ�����֪����һ���߳��뿪try-finally�ṹ��try�鲿�֣�ϵͳ��Ҫȷ��finally���еĴ���õ�ִ�С����쳣����ʱ��ϵͳ����
*		ȷ��������������Ļ��ƾ���ȫ��չ����
*		  ���ǿ���ͨ����return�������finally��������ֹϵͳ���ȫ��չ����
*		  ע�⣺��Windows Vista��ʼ�����һ���쳣������try/finally���У��������ϲ���û��try/except�飨ͬʱ���˳��򷵻�EXCEPTION_EXECUTE_HANDLER�������̾ͻ�������ֹ����ȫ��չ��������
*		������finally��Ҳ����ִ�С����������ڰ汾��Windows�У�ȫ��չ�����ڽ�����ֹǰ�������Ӷ�finally��Ҳ�л���õ�ִ�С�
* 15��EXCEPTION_CONTINUE_EXECUTION��ϵͳ�ڿ������˳��򷵻�ֵΪEXCEPTION_CONTINUE_EXECUTION�󣬽����̿�������ת�������쳣������ָ�����������ִ������ָ�
*		ʾ����
*			TCHAR g_szBuffer[100];
*			void FunclinRoosevelt1(){
*				int x = 0;
*				TCHAR *pchBuffer = NULL;
*				__try {
*					*pchBuffer = TEXT('J');
*					x = 5 / x;
*				}
*				__except (OilFilter1(&pchBuffer)){
*					MessageBox(NULL, TEXT("An exception occurred"), NULL, MB_OK);
*				}
*				MessageBox(NULL, TEXT("Function completed"), NULL, MB_OK);
*			}
* 
*			LONG OilFilter1(TCHAR **ppchBuffer) {
*				if (*ppchBuffer == NULL) {
*					*ppchBuffer = g_szBuffer;
*					return(EXCEPTION_CONTINUE_EXECUTION);
*				}
*				return(EXCEPTION_EXECUTE_HANDLER);
*			}
*	    ������ʾ��ʵ���ϣ��������쳣���˳������һ������������������������ʶ���е���һ�������ǿ������쳣���˳���������Ӻܶ���롣��Ȼ�����˳�����뷵�������쳣��ʶ���е�����һ����
*	  ������Ҳ����ִ������������Ҫ��ִ�е����񡣲������������ס��һ���쳣���׳��󣬽��̺ܿ����Ѿ���ò��ȶ������ԣ���û������쳣���˳����еĴ�����Լ򵥡����磬�ڶѱ��ƻ�������£�
*	  �ڹ��˳��������кܶ�Ĵ�����ܵ��½��̱�������߲����κ���ʾ����ֹ��
*		����ʹ��EXCEPTION_CONTINUE_EXECUTION�����Ծ��������쳣�����Ĵ�����ʱ����Գɹ�����������ÿ�ζ���ô���� -- ���ɹ����ȡ���ڳ����Ŀ��CPU�������ǵı��������ΪC/C++����������
*	  ����ָ��ͱ���ѡ���һ�����������EXCEPTION_CONTINUE_EXECUTION����֤����ִ��ʼ�ն��ܳɹ���������Ϊ��Ԥ�����ڴ�����ϡ��ص����洢��ʱ��
* 16��EXCEPTION_CONTINUE_SEARCH�������ʶ����ϵͳ�ڵ���ջ�����ϲ���ǰһ����except���try����飬���������try���Ӧ���쳣���˳���
* 17���������쳣���˳����з���ʲô��ʶ��ǰ�����Ǳ�������������������쳣���˳���Ӧ���ݾ����������غ��ʵ�ֵ��
*		ʾ����
*			__try {
*				x = 0;
*				y = 4 / x; 
*			}
*			__except ((GetExceptionCode() == EXCEPTION_INT_DIVIDE_BY_ZERO) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH){
*			}
*	  GetExceptionCode�����ں��������ķ���ֵ�����ոշ������쳣�����͡�
*	  �����г���һЩԤ������쳣��ʶ�����쳣������ѭ��WinError.h�ļ��ж�����йش������Ĺ��򣩺����Ǿ���ĺ��壬����������Platform SDK���ĵ������ǿ���WinBase.h�ļ����ҵ����ǡ��������£�
*		���ڴ���ص��쳣���£�
*			�� EXCEPTION_ACCESS_VIOLATION �߳���ͼ��дһ�������ڴ��ַ�����������ַ�������߱���Ӧ��Ȩ�ޡ�����������쳣��
*			�� EXCEPTION_DATATYPE_MISALIGNMENT
*			�� EXCEPTION_ARRAY_BOUNDS_EXCEEDED
*			�� EXCEPTION_IN_PAGE_ERROR
*			�� EXCEPTION_GUARD_PAGE
*			�� EXCEPTION_STACK_OVERFLOW
* 			�� EXCEPTION_ILLEGAL_INSTRUCTION
* 			�� EXCEPTION_PRIV_INSTRUCTION
*		���쳣������ص��쳣���£�
* 			�� EXCEPTION_INVALID_DISPOSITION
* 			�� EXCEPTION_NONCONTINUABLE_EXCEPTION
*		�������ص��쳣���£�
*  			�� EXCEPTION_BREAKPOINT
*   		�� EXCEPTION_SINGLE_STEP
*   		�� EXCEPTION_INVALID_HANDLE
*		��������ص��쳣���£�
*   		�� EXCEPTION_INT_DIVIDE_BY_ZERO
*   		�� EXCEPTION_INT_OVERFLOW
*		�븡������ص��쳣���£�
*   		�� EXCEPTION_FLT_DENORMAL_OPERAND
*   		�� EXCEPTION_FLT_DIVIDE_BY_ZERO
*   		�� EXCEPTION_FLT_INEXACT_RESULT
*   		�� EXCEPTION_FLT_INVALID_OPERATION
*   		�� EXCEPTION_FLT_OVERFLOW
*   		�� EXCEPTION_FLT_STACK_CHECK
*   		�� EXCEPTION_FLT_UNDERFLOW
*		���ں���GetExceptionCodeֻ�����쳣���˳������__except֮�������������쳣�������Ĵ�������á��������ǲ������쳣���˳���ĺ��������GetExceptionCode��
* 18����һ���쳣����ʱ������ϵͳ�������쳣���̵߳�ջ��ѹ�������ṹ��EXCEPTION_RECORD��CONTEXT��EXCEPTION_POINTERS��
*			�� EXCEPTION_RECORD���ṹ���������׳��쳣����Ϣ����Щ��Ϣ������������CPUû�й�ϵ��
*			�� CONTEXT���ṹ����������쳣����CPUҲ�йص���Ϣ��
*			�� EXCEPTION_POINTERS���ṹ�������������ݳ�Ա�����Ƿֱ�Ϊָ��ѹ��ջ�е�EXCEPTION_RECORD�ṹ��ָ���ָ��CONTEXT�ṹ��ָ�롣
*	  �����Ҫ�õ���Щ��Ϣ������Ӧ�ó�����ʹ�����ǣ��������µ��ú���GetExceptionInformation: 
*			PEXCEPTION_POINTERS GetExceptionInformation();
*	  ��Ҫ��ʾ���������ֻ�����쳣���˳����е��ã�������Ϊ�������ṹֻ����ϵͳ�����쳣���˳���ʱ������Ч�ġ�һ�����������ת�Ƶ��쳣���������߱�ĵط�����Щջ�ϵ����ݽṹ
*	�ͱ������ˡ�
* 19����ʵ������Ҳ������Ӧ�ó��������ǿ���׳�һ���쳣�����Ǻ���������ʧ��֪ͨ������ߵ���һ�ַ�ʽ��
*	  �׳�һ������쳣�����ײ����ˣ�ֻ��Ҫ����RaiseException������
***********************************************************************************************************************************************************************************
* 20���쳣���˳��򷵻�EXCEPTION_CONTINUE_SEARCHʱ�������������ϵͳ�����ڵ������е��ϲ�Ѱ���쳣���˳��򡣿������ÿ���쳣���˳��򶼷���EXCEPTION_CONTINUE_SEARCH���ֻᷢ��ʲô�أ�
*	����������£����ǻ�������ν��δ�����쳣��
*	  Microsoft Windows�ṩ�˺���SetUnhandledExceptionFilter�����������Ǵ����쳣�������ᣬ����Windows�ͻ���ʽ��Ϊ����쳣û�еõ�����
*	  ����ͨ��Ӧ�ڽ��̳�ʼ���׶ε������������һ�������������������������������߳��׳�δ�����쳣���ᵼ������ָ����ͨ��SetUnhandledExceptionFilter�Ĳ����������ϲ��쳣���˺���ִ�С�
*	����쳣���˺���ԭ�ͱ������������ƣ�
*			LONG WINAPI TopLevelUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
*     ���쳣���˳����ֻҪ��������EXCEPTION_*��ʶ��֮һ���Ϳ������κ�������Ҫ���Ĵ���ע�⣬���̿�����Ϊջ�����δ�ͷ�ͬ���źţ�����û���ͷ��ڶ��Ϸ�����ڴ棬���Ѿ����ڱ��ƻ���
*	״̬�����ԣ�����Ӧ�þ��������쳣���˺����Ĵ����������Ҳ�Ҫ�����붯̬�ڴ棬��Ϊ�ѿ����Ѿ����ƻ���
* 21�������������µ�δ�����쳣���˳���ʱ��SetUnhandledExceptionFilter�����ϴΰ�װ���쳣���˳���ĵ�ַ��ע�⣬�������ʹ�õ���C/C++���п⣬�ڽ�����ڵ㺯����ʼִ��ǰ��C/C++���к���
*	�ͻᰲװһ��Ĭ�ϵ�ȫ���쳣���˳��򣬼�__CxxUnhandledExceptionFilter��������������Ĺ����ܼ򵥣����ȼ���쳣�ǲ�������C++�쳣������ǣ����ڽ���ʱִ��abort���������߽�����Kernel32.dll
*   �е�UnhandledExceptionFilter�����������ڰ汾��C/C++���п��У����̵��˾���ֹ�ˡ�_set_abort_behavior������������������abort����ִ�еĴ��󱨸档���__CxxUnhandledExceptionFilter��Ϊ
*	��ǰ�쳣����һ��C++�쳣���ͻ᷵��EXCEPTION_CONTINUE_SEARCH������Windows���������δ�����쳣��
*	  ���ǿ�����NULL��Ϊ��������SetUnhandledExceptionFilter����ȫ��δ�����쳣���˳�����ΪUnhandledExceptionFilter��
* 22������֪��ÿ���̵߳�ִ�ж��Ǵ�NTDLL.dll�еĺ���BaseThreadStart��ʼ�ģ�
*			VOID BaseThreadStart(PTHREAD_START_ROUTINE pfnStartAddr, PVOID pvParam){
*				__try {
*					ExitThread((pfnStartAddr)(pvParam));
*				}
*				__except(UnhandledExceptionFilter(GetExceptionInformation())) {
*					ExitProcess(GetExceptionCode());
*				}
*				// NOTE: We never get here
*			}
*	  �����������һ���ṹ���쳣�����structured exception handling frame, SEH����򣩣������Ƚ���try����飬Ȼ����try���е����߳�/�������ڵ㺯�������ԣ�������ǵ��߳��׳�һ���쳣��
*	�������а�װ���쳣���˳��򶼷���EXCEPTION_CONTINUE_SEARCH��ϵͳ�ṩ��һ�����⺯�������Զ����ã���UnhandledExceptionFilter��
*			LONG UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
*	  ����ͨ�쳣���˳���һ�������������������EXCEPTION_*�쳣��ʶ���е�һ����
*	  ˵�����ڷ���Ƕ���쳣�������쳣���˳��������׳�һ���쳣��������£�UnhandledExceptionFilter������EXCEPTION_NESTED_CALL������Windows Vista֮ǰ��ƽ̨��UnhandledExceptionFilter����
*	���أ����̲����κ���ʾ�ؽ�����
* 23��UnhandledExceptionFilter�ڷ�����Щ�쳣��ʶ��֮ǰ������ִ�д����Ĵ��롣UnhandledExceptionFilter�����ڴ����쳣ʱ�ᰴ˳��ִ��5�����衣���ǽ���������ÿ�����衣��ִ������Щ�����
*	UnhandledExceptionFilter��ִ�п��ƽ���Windows���󱨸棨Windows Error Reporting��������ΪWER����
*		������1���������Դ����д�����������ִ��
*		������2����δ�����쳣�����������
*		������3��֪ͨ�������õ�ȫ���쳣���˺���
*		������4����δ�����쳣��������������ٴΣ�
*		������5����ֹ����
*	  ����Щ�����UnhandledExceptionFilter�����ں�̨����ͼ��������쳣�����⣬֪ͨ������������У��������ڱ�Ҫ����¼򵥵���ֹ�������С�������ܴ����쳣���ͻ᷵��
	EXCEPTION_CONTINUE_SEARCH������ϵͳ�ں˵õ�������ƣ����������д���֪ͨ�û���
* 24����Windows Vista��ʼ��UnhandledExceptionFilter�������ٷ��ʹ��󱨸浽Microsoft�ķ���������Ϊ���������EXCEPTION_CONTINUE_SEARCH�����ǣ�ϵͳ�ں�֪��һ���쳣���û�̬�߳�û�еõ�����
*	Ȼ�󣬹����쳣��֪ͨ�����͸�WerSvcר�÷������֪ͨ����һ���������߼����ع��̵��ã�Advanced Local Procedure Call�������ΪALPC�����Ļ��Ʒ����ģ�ALPC���Ʋ�û�ж��⹫����ALPC����
*	�����̣߳�ֱ��WerSvc������Ĵ�������ͨ������CreateProcess������WerFault.exe�����ȴ��½��̵Ľ���������Ĵ����ͷ�����WerFault.exe��ɡ���WerFault�����У��ᵯ��һ���Ի����������û�
*	ѡ��ر�Ӧ�ó�����߸���һ��������������û�ѡ��رճ���WerFault.exe�����TerminateProcess��������ʾ����ȫ����Ӧ�ó����ɴ˿ɼ�������������ڷ�������Ľ���֮������֤���󱨸���Ϊ
*	���ȶ���
*	  ���ǿ���ͨ��ע�����������������г��ָ��û��Ľ��棬��HKEY_CURRENT_USER\Software\Microsoft\Windows\Windows Error Reporting����֮�µ�DontShowUIֵ����Ϊ1ʱ���Ͳ������κζԻ��򵯳���
*	������ں�̨���ɲ����͸�Microsoft�ķ�����������������ⷢ��ʱ���û�ѡ���ͻ��߲����ʹ��󱨸��Microsoft�����ǿ��Ըı�DefaultConsent��ֵ��ֵ����ΪDWORD��λ��Consent�����¡����ǣ�
*	��ֵ���Ƽ�����������Control Panel��������壩�е�Problem Reports And Solution�����ⱨ����������������WER����̨��������������������Щѡ�
*	  ��������������һ�µ�δ�����쳣����ʱ��WER�ṩ�����һ�����ܣ���ʱ���ԣ����ó���Ա�������Ϊ��ʵ������û�ѡ����Գ���Ľ��̣�WerFault.exe����ͻᴴ��һ���¼������ֶ���������ΪTrue��
*	ͬʱ��Ϊδ����״̬����������bInheritHandles����ΪTrue���Ӷ�����WerFault.exe���ӽ��̣�����������������Լ̳�����¼������Ȼ��WER�ҵ�������Ĭ�ϵ��������������ŵ�����Ľ����ϡ�
* 25����ʱ����
*	  ���Զ�̬���ŵ�������������Windows��õĹ���֮һ��
*	  ����ԭ��
*		ע���������������Ŀ��
			HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebug
		���������һ����ΪDebugger��ֵ����Visual Studio�İ�װ���̱�����Ϊ����:
			"C:\Windows\system32\vsjitdebugger.exe" -p %ld -e %ld
		vsjitdebugger.exe��������һ������������ֻ������һ���Ի�������������ѡ���������Ӧ�ó���
		WerFault.exe�������������������������һ����Ҫ���ԵĽ���ID���ڶ����Ǽ̳й������¼��������ָ����WerSvc���񴴽��Ĵ���δ�������ֶ������¼���ע�⣬�������Ҳ�ڵȴ�WerSvc���ص�ALPC
	  ֪ͨ������������ʵ�ֵĵ�����Ҫ�ܹ�ʶ��-p ��-e�������أ��������ID���¼������
	    WerFault.exe�Ƚ�����ID���¼�����ϲ���һ���ַ����Ȼ��ͨ��CreateProcess����������������bInheritHandles��������ΪTRUE������������Ľ��̿��Լ̳��¼�����ľ������ʱ������������
	  �����鿴�����в��������-p���ش��ڣ��������õ�����ID��ͨ��DebugActiveProcess���Լ����ŵ���Ӧ�����ϣ�
		BOOL DebugActiveProcess(DWORD dwProcessID);
	    ����ϵͳ�Ե�������ɸ����Ժ󣬻Ὣ�����Գ����״̬����������������磬����ϵͳ����ߵ����������Գ����ж����߳��Լ������Գ����������DLL����������Ҫ��ʱ�䴦����Щ������׼������
	  ������Щ׼����������ʱ����������Ľ���ֻ�ܵȴ���
		��⵽δ�����쳣���Ƕδ��루����4��Ҳ�ڵȴ���WerSvc���ص�ALPC֪ͨ������5����ALPC������Ϊ��WerFault.exe���̾��Ϊ��������WaitForSingleObjectEx��������ֱ��WER������Ĺ�����ע��
	  ������WaitForSingleObjectEx���߳̿����ڿ�֪ͨ״̬�µȴ�����������첽���̵��ã�asynchronous procedure call�� APC�����ܵõ�����
	    ��������ʼ���Ժ󣬿�ʼ���-e���ء�������� �������õ��¼����������SetEvent��
		�����¼�������11����֪ͨWerFault.exe�������Ѿ����ŵ���������̣����Խ��ܲ������쳣�ˡ�����WerFault.exe���̽������˳�����WerSvc����Ҳ�������ӽ����˳��ˣ�������ALPC���أ�����12��
	  ����͵����˱����Գ�����̱߳����ѡ�����֪ͨ�ں�һ���������Ѿ����ŵ���ǰ�̣߳���׼���ý���δ�����쳣��֪ͨ����13������ͺ�25.1�Ĳ���3Ч����ͬ�������ţ����������յ�����δ�����쳣
	  ��֪ͨ������������ȷ��Դ���ļ�������λ���׳��쳣������ָ���ܿᣬ������
	    ������ʾ��
		  HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AeDebugע����������һ����ΪAuto����ֵ������ΪREG_SZ�����ֵ��ʾ�Ƿ���WERѯ���û����Ի��ǹر�������̡����
	  AutoֵΪ1��WER��ѯ���û���ֱ����������������Ӧ����������Ҫ��Ĭ����Ϊ����������WER����������Ϣ��--��Ϊ��δ�����쳣����ʱ��������������ϣ������������̡�
		  ��ʱ�����ǲ���ϣ������һЩ�������̣�����svchost.exe��ĳ���������������������£�������AeDebug�����������һ������AutoExclusionList�����������洴��һ��DWORD��ֵ������������Ϊ
	  ���ǲ�����Եĳ���ֵ��Ϊ1.
		  �����Ҫ��һ��ϸ���ĸ�������Ҫ�Զ���ʱ���ԣ����ǿ�������AutoΪ1��������HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\Windows Error Reporting֮�����һ������DebugApplications��
	  ����������£�����һ��DWORD��ֵ������������Ϊ������Ҫ��δ�����쳣����ʱ�Զ����Եĳ���ֵ��Ϊ1��
* 26���������쳣�ͼ����������(https://blog.csdn.net/sesiria/article/details/78895499)
*	  ǰ����ܵ�SEH������һ�ֻ��ڴ����frame-based�����ƣ���ÿ�ε��߳̽���һ��try�飨������ʱ��ϵͳ�������������һ���ڵ㡣��������쳣��ϵͳ���η��������е�ÿ�������--���������
    ��try�鿪ʼһֱ����������try��--��Ѱ��ÿ��try���catch�������һ���ҵ�һ��catch�������ϵͳ�ٴη�������ִ��finally�顣��չ�������Ժ󣨻���try������û�з����쳣�������˳�����
	ϵͳ���������Ƴ����д����
	  WindowsҲ�ṩ���������쳣����vectored exception handling�� VEH�����ƣ���ͬSEHЭͬ�������������ע��һ��������������Ҫ�����������йصĹؼ��֣�ÿ���쳣��������һ��δ�����쳣����
	��׼SEH�Ŀ���ʱ����������ͻᱻ���á�
	  AddVectoredExceptionHandler���������쳣��������ע�ᣬע����쳣�������ᱻ��ӵ������б���������������̷߳����쳣ʱ����Щ�������ᱻ���á�
		PVOID AddVectoredExceptionHandler(
			ULONG bFirstInTheList,
			PVECTORED_EXCEPTION_HANDLER pfnHandler
		);
	  Handler��ָ���������쳣��������ָ�룬�ú�������������ǩ����
		LONG WINAPI ExceptionHandler(struct _EXCEPTION_POINTERS* pExceptionInfo);
	  �����bFirstInTheList�������ݵ�ֵΪ0�� ��ôͨ��pfnHandler���ݵ��쳣�������ͻ���ӵ��б��β�ˡ�������ݸ�bFirstInTheList���ݵ�ֵ��0�������ͻᱻ�����ڲ��б��ͷ�������쳣����ʱ��
	ϵͳ��ִ��SEH���˳���֮ǰ�������б�˳�����������Щ������ һ��ĳ�������ܹ��������������⣬Ӧ�����Ϸ���EXCEPTION_CONTINUE_EXECUTION�������׳��쳣��ָ���ٴ�ִ�С�ֻҪĳ�������������
	����EXCEPTION_CONTINUE_EXECUTION, SEH���˳���㲻���д����쳣�Ļ��ᡣ ����쳣���������ܾ������⣬Ӧ�÷���EXCEPTION_CONTINUE_SEARCH, ���б��е������������л���ȥ�����쳣�����
	���еĴ�����������EXCEPTION_CONTINUE_SEARCH, SEH���˳���ͻ�ִ�У���Ҫע����ǣ�VEH���˺������ܷ���EXCEPTION_EXECUTION_HANDLER��
	  ����ͨ�����º�����ɾ��֮ǰ��װ��VEH�쳣��������
	    ULONG RemoveVectoredExceptionHandler(PVOID pHandler);
	  Handle������֮ǰ��װ�ĺ�����������������Դ�AddVectoredExceptionHandler�ķ���ֵ�õ���

	  ��������SEH֮ǰ�����쳣��VEH������������δ�����쳣����ʱ�ܵõ�֪ͨ��Ҫ�õ����֪ͨ����ϸ��Ϣ��������Ҫͨ�����������������ע��һ�������������continue handler����
		PVOID AddVectoredContinueHandler(ULONG bFirstInTheList, PVECTORED_EXCEPTION_HANDLER pfnHandler);
	  ͬ���������bFirstInTheList��������0����ôͨ��pfnHandler����ĺ�������ӵ�������������б��β�������bFirstInTheList��һ����0ֵ�������ᱻ���ڼ�����������б��ͷ������δ�����쳣
	����ʱ��ϵͳ�ᰴ���б�������ִ�м����������������Ҫ�ر�˵�����ǣ���Щ��������ִ������SetUnhandledExceptionFilter����װ��ȫ���쳣������򷵻�EXCEPTION_CONTINUE_SEARCH֮��ſ�ʼ
	�ġ�һ���������������Է���EXCEPTION_CONTINUE_EXECUTION��ֹͣ��������ļ�����������ִ�У�����ϵͳ�ٴ�ִ��ʧ��������߷���EXCEPTION_CONTINUE_SEARCH��ϵͳִ����������Ĵ������
	  ���ǿ���ͨ������������������б���ɾ��֮ǰ��װ�ļ����������
		ULONG RemoveVectoredContinueHandler(PVOID pHandler);
	  Handle������ʾ֮ǰ��װ�Ĵ���������ľ���� ���������Դ�AddVectoredExceptionHandler�ķ���ֵ�õ���
	  �ɴ˿������󣬼����������һ������ʵ���������١�
* 27��C++�쳣��ṹ���쳣�ıȽ�
*	  SEH�ǲ���ϵͳ���ṩ�ı����������κ������ж�����ʹ�á���C++�쳣����ֻ���ڱ�дC++����ʱ�ſ���ʹ�á���������ڿ���C++Ӧ�ó����Ǿ�Ӧ��ʹ��C++�쳣�������ǽṹ���쳣��������C++�쳣
*	�����Ե�һ���֣�������֪��ʲô��һ��C++������Ҳ����ζ�ű��������Զ����ɴ���������C++�����������������֤������ͷš�
*	  ����Ӧ���˽�Microsoft��Visual C++������ʹ�ò���ϵͳ�Ľṹ���쳣������ʵ��C++�쳣������ơ����ԣ��ڴ���һ��C++try��ʱ����������Ϊ��������һ��SEH __try�顣C++��catch����ӦSEH
*   �쳣���˳���catch���еĴ������ӦSEH __except���еĴ��롣��������Ҳ��ΪC++throw������ɶ�Windows RaiseException�����ĵ��á�throw�����ʹ�õı������ΪRaiseException�ĸ��Ӳ�����
*		ʾ����
*		C++�쳣����										C++���������ɵĶ�Ӧ�ľͽṹ���쳣�������
*		void ChunkyFunky(){								void ChunkyFunky(){
*			try {											try {
*				// Try body										// Try body
*				...												...
*				throw 5;										RaiseException(Code=0xE06D7363, Flag=EXCEPTION_NONCONTINUABLE, Args=5);
*			}												}
*			catch (int x) {									__except((ArgType == Integer) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTIOIN_CONTINUE_SEARCH) {
*				// Catch body									// Catch body
*				...												...
*			}												}
*			...												...
*		}												}
* 28��Microsoft Visual Studio�ĵ�����Ϊ�쳣�����ṩ�˳�ɫ��֧�֡��������е�ĳ���߳��׳��쳣������ϵͳ������֪ͨ������������������Ѿ����ţ������֪ͨ����Ϊ���״λ���֪ͨ��
	��first-chance notification��������������Ӧ���֪ͨ����ʹ�߳�Ѱ���쳣���˳���������е��쳣���˳��򶼷���EXCEPTION_CONTINUE_SEARCH������ϵͳ���������һ����������֪ͨ��
	��last-chance notification����������֪ͨ�Ĵ���ʹ�����������Ա�ܸ��õؿ����쳣���ԵĹ��̡�
	  ���ÿ��������������ǿ���ͨ����������Exceptions�Ի�����������������Ӧ�״λ����쳣֪ͨ��
*****************************************************************************************************************************************************************************************
* 29��Windows���󱨸����̨
*	  ��һ��������Ϊδ�����쳣����ֹʱ��WER�ᴴ������δ�����쳣����ִ�������ģ�execution context���Ĵ��󱨸棨error reporting����
*	  ����õ��û�����ɣ��������ͻ�ͨ����ȫ�ŵ����͸�Microsoft�ķ���������������������ݿ������֪�������Ƚϡ�������ݿ����Ѿ����������Ľ���������������Ὣ�����͸��û�������
*	�û��Ϳ��Բ�ȡ��Ӧ��������������Լ��Ĺ�����
*	  Ӳ����������̿������������������������ע���Ʒ��صı��档�������Ҳ�������ں�ģʽ�豸��������ı������߹�����˿��ܵĽ���������Ƕ��ֶ����ġ�
*	  �����û���Ը���ͱ����Microsoft�ķ����������ɵı���Ҳ�ᱣ�����û��Ļ����ϡ�ͨ��WER����̨���û������ڱ��ػ����������Щ�������������Ⲣ�鿴��Ӧ�ı��棨�������\���п��������\��ȫ
*	��ά��\���ⱨ�棩��
* 30���ɱ�̵�Windows���󱨸�
*	  Ӧ�ó������ѡ����ʧ��ʱ����WER���ͱ��档���Ӧ�ó��򻹴��ڲ���ͷ���ǰ�Ŀ����Ͳ��Խ׶Σ���ô���ѡ������Ǻ����ô���
* 31����ʱ�����ǿ�������Ӧ�ó���ͨ������һϵ�е�WER�������ƴ��󱨸档���������ǿ�����Ҫ���ƴ��󱨸�����������
*		�������ڱ�д�Լ���δ�����쳣���˳���
*		��������ҪӦ�ó�����δ�����쳣û�з����������Ҳ���ɱ��档
*		�������������󱨸�����Ӹ�����Ϣ��
*	  ���ƴ��󱨸��һ�ּ򵥷�ʽ��ָ����Щ���ݿ�����ļ���Ҫ��ӵ�Ϊ�������ɵ�ÿ�����ⱨ���
* 32����Ӧ�ó����ж��ƴ��󱨸档���߿����ڴ��󣨰�����Щ���쳣����û��ʲô��ϵ�����⣩����ʱ��ʹ�����������ĺ�����Ӧ�ó����ڴ����������Ҫ������ֹ������Ӧ�þ���ʹ��Windows���󱨸棬
*	�����ǽ�һЩ����ģ������Ϣд�뵽Windows�¼���־�����ǣ�WER���ͨ��һЩע�����ֵ�����˱���Ĵ�С����Ŀ�����ǿ�����HKEY_CURRENT_USER\Software\Microsoft\Windows\Windows Error Reporting
*	�������ҵ���Щֵ��
*		��������������������������������������������������������������������������������������������������������������������������������������������������������
*				ע�������			|														����
*		��������������������������������������������������������������������������������������������������������������������������������������������������������
*			MaxArchiveCount			|	�浵���ļ���Ŀ���ޡ�Ϊ1~5000֮���һ��ֵ��Ĭ��ֵΪ1000
*		��������������������������������������������������������������������������������������������������������������������������������������������������������
*			MaxQueueCount			|	�ڷ��͸�Microsoft�ķ�����֮�䱣���ڱ��ػ���������ı�����Ŀ���ޡ�Ϊ1~500֮���һ��ֵ��Ĭ��ֵΪ50
*		��������������������������������������������������������������������������������������������������������������������������������������������������������
*	  ˵����
*		�ѷ��ͱ���ĸ�����Ϣ�����ڵ�ǰ�û���AppData\Local\Microsoft\Windows\WER\ReportArchiveĿ¼�����ǣ������ļ��������Ŀ¼��δ���ͱ�����б����ڵ�ǰ�û���AppData\Local\Microsoft
*	  Windows\WER\ReportQueueĿ¼���ź����ǣ�WER����̨����������Щ�����API��û�й������������ǲ���ö��Ӧ�ó�������ⱨ�档
* 33�����ⱨ��Ĵ��������ƺ��ύ��WER�ǰ�һ�²�����ò�ͬ�ĺ�����ʵ�ֵġ�
*		(1) ����WerReportCreate������һ�������ⱨ�档
*		(2) ����WerReportSetParameter��λ��߶�������ñ��������
*		(3) ����WerReportAddDump����С��ת���Ž����档
*		(4) ����WerReportAddFile��λ��߶�����������ļ��������û��ĵ����Ž����档
*		(5) ����WerReportSetUIOption���޸�һЩ�ַ�����������WerReportSubmitʱ����Щ�ַ������ܻ���ͬ��Ի����г��ָ��û���
*		(6) ����WerReportSubmit���ύһ�����档���ݲ�ͬ�ı�־��Windows���ܻὫ����Ž����У�������ʾ�û������淢�͸�������������ֱ�ӷ��ͱ��档
*		(7) ����WerReportCloseHandle���رձ��档
* 34��Ӧ�ó����������ڷ���һ�����ص�����ʱ��WER�����ڳ�����ֹ�Ժ��Զ�����������Windows Vistaϵͳ��˵������һ����������������Ҫ����ͬWindows Vistaһ�𷢲��Ĵ󲿷�Ӧ�ó��򣨱���
*	Windows Explorer��Internet Exporer��RegEdit�Լ�һЩ��Ϸ����֧���Զ��������������ߣ�WER����Ӧ�ó����ڱ���ֹǰ�ָ��κ���Ҫ�����ݡ�
* 35������Ӧ�ó�������������ʵ��һ�׸�Ϊͨ�õ�API��һ���֣�����API����Ϊ��������Restart Manager����Ϊ�˷�ֹ�ظ������������Ӧ�ó���WER��������֮ǰ���ȼ�����Ƿ��Ѿ�����������60�롣
* 36�����̿���ע��һ���ص�������WER�ڽ��̷�������ֹʱ���á�����ص��������Ա����������ݺ�״̬��Ϣ��
*		ʾ����
*			DWORD WINAPI ApplicationRecoveryCallback(PVOID pvParameter){
*				DWORD dwReturn = 0;
*				BOOL bCancelled = FALSE;
*				while (!bCancelled)
*				{
*					// Show progress
*					ApplicationRecoveryInProgress(&bCancelled);
*					
*					// Check for user cancellation
*					if (bCancelled) {
*						// The user clicked the Cancel button
*						// Notify that we have failed to finish the recovery process
*						ApplicationRecoveryFinished(FALSE);
*					}
*					else
*					{
*						// Save teh state required for recovery, chunk by chunk
*						if (MoreInformationToSave()) {
*							// Save a chunk of data in fewer milliseconds than the delay set with the dwPingInterval parameter you paased to RegisterApplicationRecoveryCallback
*						}
*						else 
*						{
*							// Notify that we have finished the recovery process 
*							ApplicationRecoveryFinished(TRUE);
*							// Set bCancelled to TRUE when finished to exit the loop
*							bCancelled = TRUE;
*						}
*					}
*				}
*				return(dwReturn);
*			}
*	  ��Ҫ��ס���ǣ����ص���������ʱ�����̿��ܴ���һ�����ƻ���״̬��ͬʱ�����쳣���˳��������ͬ���������������͵Ļص�������
*/

#include <Windows.h>
#include <werapi.h>

namespace HY_SEG
{
	class CHYSEG
	{
	public:
		CHYSEG(void);
		~CHYSEG(void);

		// ��ȡ�ոշ������쳣������
		//DWORD GetExceptionCode();

		// �׳�һ������쳣
		VOID RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, CONST ULONG_PTR *pArguments);

		// ʹӦ�ó����ܹ�ȡ�����̵�ÿ���̵߳Ķ����쳣�������
		LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(
			LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter
		);

		// ��⵱ǰ�����ǲ��Ǵ��ڵ������Ŀ���֮��
		BOOL IsDebuggerPresent();

		// *************** �ɱ�̵Ĵ��󱨸� ****************
		// ����ĺ�������������Ϊ���̸ı�һЩ����
		HRESULT WerSetFlags(DWORD dwFlags);
		// �����֪��ĳ�����̵ĵ�ǰ���ã����Ե��������������
		HRESULT WerGetFlags(HANDLE hProcess, PDWORD pdwFlags);
		// ���Ե�������������������ñ��������뷢��
		HRESULT WerAddExcludedApplication(PCWSTR pwzExeName, BOOL bAllUsers);
		// ���ô��󱨸�
		HRESULT WerRemoveExcludedApplication(PCWSTR pwzExeName, BOOL bAllUsers);
		
		// Ҫ����������ݿ�(���Զ�ε��ã�����������ݿⱣ����С��ת���ļ���)
		HRESULT WerRegisterMemoryBlock(PVOID pvAddress, DWORD dwSize);
		// Ҫ��������ļ������ⱨ���У�����ֻ��Ҫ����
		HRESULT WerRegisterFile(PCWSTR pszFileName, WER_REGISTER_FILE_TYPE regFileType, DWORD dwFlags);
		// �Ƴ��Ѿ�ע������ݿ�
		HRESULT WerUnregisterMemoryBlock(PVOID pvAddress);
		// �Ƴ��Ѿ�ע����ļ�
		HRESULT WerUnregisterFile(PCWSTR pszFileName);
		
		// ����һ���Զ���Ĵ��󱨸棬�����뱨�����ϸ��Ϣ
		HRESULT WerReportCreate(PCWSTR pwzEventType, WER_REPORT_TYPE repType, PWER_REPORT_INFORMATION pReportInformation, HREPORT* phReportHandle);
		// ����Ψһ��ʶָ��������¼��Ĳ���
		HRESULT WerReportSetParameter(HREPORT hReportHandle, DWORD dwparamID, PCWSTR pwzName, PCWSTR pwzValue);
		// ��С��ת���ļ����뱨��
		HRESULT WerReportAddDump(
			HREPORT hReportHandle,
			HANDLE hProcess,
			HANDLE hThread,
			WER_DUMP_TYPE dumpType, 
			PWER_EXCEPTION_INFORMATION pExceptionParam, 
			PWER_DUMP_CUSTOM_OPTIONS pDumpCustomOptions, 
			DWORD dwFlags
		);
		// �������ļ����뱨��
		HRESULT WerReportAddFile(
			HREPORT hReportHandle,
			PCWSTR pwzPath,
			WER_FILE_TYPE repFileType,
			DWORD dwFileFlags
		);
		// �޸ĶԻ����ı�
		HRESULT WerReportSetUIOption(
			HREPORT hReportHandle,
			WER_REPORT_UI repUITypeID,
			PCWSTR pwzValue
		);
		// �ύ���󱨸�
		HRESULT WerReportSubmit(
			HREPORT hReportHandle,
			WER_CONSENT consent,
			DWORD dwFlags,
			PWER_SUBMIT_RESULT pSubmitResult
		);
		// �ڱ����ύ�Ժ󣬼ǵ�Ҫ�ر����ⱨ��
		HRESULT WerReportCloseHandle(HREPORT hReportHandle);

		// Ӧ�ó�������������Զ�����������ͨ���������������WER��ע���Լ�
		HRESULT RegisterApplicationRestart(PCWSTR pwzCommandline, DWORD  dwFlags);
		// ����WER��Ҫ������ǰ��Ӧ�ó��򣬿���ͨ�������������
		HRESULT UnregisterApplicationRestart(void);

		// ���̿���ע��һ���ص�������WER�ڽ��̷�������ֹʱ���á�����ص��������Ա����������ݺ�״̬��Ϣ
		HRESULT RegisterApplicationRecoveryCallback(
			APPLICATION_RECOVERY_CALLBACK pRecoveyCallback,
			PVOID pvParameter,
			DWORD dwPingInterval,
			DWORD dwFlags
		);

	private:
		CHYSEG(const CHYSEG&) = delete;
		CHYSEG& operator=(const CHYSEG&) = delete;
		CHYSEG(CHYSEG&&) = delete;
		CHYSEG& operator=(CHYSEG&&) = delete;
	};
}

