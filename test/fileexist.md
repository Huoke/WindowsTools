<h1 align="center">Win32 API</h1>

# FindFirstFileA
FindFirstFileA function (fileapi.h)
判断文件、文件夹是否存在
```C++
#include <iostream>
#include <windows.h>

using namespace std;

BOOL FindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpPath, &fd);
	BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;
	BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
	FindClose(hFind);
	return RetValue;
}

// BOOL FolderExists(LPCTSTR lpPath)
// {
// 	return FindFirstFileExists(lpPath, FILE_ATTRIBUTE_DIRECTORY);
// }

/*
 * 检查一个  路径 是否存在（绝对路径、相对路径，文件或文件夹均可）
 * 存在则返回 1 (TRUE)
 */
BOOL FilePathExists(LPCTSTR lpPath)
{
	return FindFirstFileExists(lpPath, FALSE);
}
int main()
{
	BOOL isE = FilePathExists(L"D:\\InstallAPP\\Timsh.tpc");
	cout << isE << endl;
	return isE;

}
```
