    #include <windows.h> 
    #include <tchar.h>
    #include <stdio.h> 
    #include <strsafe.h>
    #include <iostream>
    using namespace std;
    
    #define BUFFER_SIZE  1024
    
    void ErrorExit(PTSTR lpszFunction) 
    
    // Format a readable error message, display a message box, 
    // and exit from the application.
    { 
        LPVOID lpMsgBuf;
        LPVOID lpDisplayBuf;
        DWORD dw = GetLastError(); 
    
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL );
    
        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
            (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
        StringCchPrintf((LPTSTR)lpDisplayBuf, 
            LocalSize(lpDisplayBuf) / sizeof(TCHAR),
            TEXT("%s failed with error %d: %s"), 
            lpszFunction, dw, lpMsgBuf); 
        MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
    
        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
        ExitProcess(1);
    }
    
    int main()
    {
        HANDLE  hReadPipe = NULL;
        HANDLE hWritePipe = NULL;
        SECURITY_ATTRIBUTES saAttr;
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
        saAttr.bInheritHandle = TRUE; 
        saAttr.lpSecurityDescriptor = NULL; 
        if (!CreatePipe(&hReadPipe , &hWritePipe, &saAttr, BUFFER_SIZE  )) {
            ErrorExit(TEXT("StdoutRd CreatePipe")); 
        }
    
        
        PROCESS_INFORMATION piProcInfo; 
        STARTUPINFO siStartInfo;
        ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
     
        ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
        siStartInfo.cb = sizeof(STARTUPINFO); 
        siStartInfo.hStdError = hWritePipe, 
        siStartInfo.hStdOutput = hWritePipe, 
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
     
         // Create the child process. 
         WCHAR szCmdline[] = TEXT("sfc.exe /scannow");
         PVOID OldValue = NULL;
         Wow64DisableWow64FsRedirection(&OldValue);
         CreateProcess(NULL, 
          szCmdline,     // command line 
          NULL,          // process security attributes 
          NULL,          // primary thread security attributes 
          TRUE,          // handles are inherited 
          0,             // creation flags 
          NULL,          // use parent's environment 
          NULL,          // use parent's current directory 
          &siStartInfo,  // STARTUPINFO pointer 
          &piProcInfo);  // receives PROCESS_INFORMATION 
         Wow64RevertWow64FsRedirection(OldValue);     
         BOOL flag = TRUE;
         char buffer[BUFFER_SIZE] = {0}
         DWORD   dwRead = 0;
         while (flag ) {
             ZeroMemory(buffer, sizeof(char)*BUFFER_SIZE);
             if (WAIT_OBJECT_0 ==WaitForSingleObject (piProcInfo.hProcess, 50)) {
                 ReadFile(hReadPipe, buffer, BUFFER_SIZE, &dwRead , NULL)
                 std::cout << buffer <<std::endl;
                 flag = false;
                 break;
             } else if (WAIT_TIMEOUT == WaitForSingleObject (piProcInfo.hProcess, 50)) {
                 ReadFile(hReadPipe, buffer, BUFFER_SIZE, &dwRead , NULL)
                 std::cout << buffer <<std::endl;
             }
         }
    
         CloseHandle(piProcInfo.hProcess);
         CloseHandle(piProcInfo.hThread);
         CloseHanlde(hReadPipe);     
         CloseHanlde(hWritePipe);  
         return 0;
    }
