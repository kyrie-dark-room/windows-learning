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
*			
*/

#include <Windows.h>

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

	private:
		CHYSEG(const CHYSEG&) = delete;
		CHYSEG& operator=(const CHYSEG&) = delete;
		CHYSEG(CHYSEG&&) = delete;
		CHYSEG& operator=(CHYSEG&&) = delete;
	};
}

