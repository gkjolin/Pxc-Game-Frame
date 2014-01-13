#include "FileManage.h"

namespace PxcUtil
{


namespace FileManage
{

std::wstring GetAbsPath()
{
	wchar_t wszPath[MAX_PATH];
	memset(wszPath, 0, sizeof(wszPath));
	GetCurrentDirectoryW(MAX_PATH, wszPath);
	return wszPath;
}

BOOL IsRoot(LPCTSTR lpszPath)
{
	TCHAR szRoot[4];
	wsprintf(szRoot, L"%c:\\", lpszPath[0]);
	return (lstrcmp(szRoot, lpszPath) == 0);
}

void FindFilesRecursive(LPCTSTR lpszPath, const wchar_t* szExt, std::vector<std::wstring>& vecOut)
{
	TCHAR szFind[MAX_PATH];
	lstrcpy(szFind, lpszPath);
	if (!IsRoot(szFind))
		lstrcat(szFind, L"\\");
	lstrcat(szFind, L"*.*"); // �������ļ�
	int iExtLen = lstrlen(szExt);

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFileExW(szFind, FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
	if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ��
		return;
	do
	{
		if (wfd.cFileName[0] == '.')
			continue; // ����������Ŀ¼
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			TCHAR szFile[MAX_PATH];
			if (IsRoot(lpszPath))
				wsprintf(szFile, L"%s%s", lpszPath, wfd.cFileName);
			else
				wsprintf(szFile, L"%s\\%s", lpszPath, wfd.cFileName);
			FindFilesRecursive(szFile, szExt, vecOut); // ����ҵ�����Ŀ¼��������Ŀ¼���еݹ�
		}
		else
		{
			std::wstring wstrFileName = wfd.cFileName;
			int iFileLen = wstrFileName.size();
			if (!szExt || iExtLen == 0 ||
				(iFileLen > iExtLen + 1 && wstrFileName.substr(iFileLen - iExtLen, iExtLen).compare(szExt) == 0))
			{
				TCHAR szFile[MAX_PATH];
				if (IsRoot(lpszPath))
					wsprintf(szFile, L"%s%s", lpszPath, wfd.cFileName);
				else
					wsprintf(szFile, L"%s\\%s", lpszPath, wfd.cFileName);
				//printf("%s\n",szFile);
				vecOut.push_back(szFile);
			}
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind); // �رղ��Ҿ��

}

bool IsFileExist(LPCTSTR lpszFileName)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFile = FindFirstFileExW(lpszFileName, FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	FindClose(hFile);
	return true;
}

FILE* OpenFileWithCreate(LPCTSTR lpszFileName)
{
	std::wstring wstrFolder = lpszFileName;
	int ipos = wstrFolder.find_last_of(L'\\');
	if (ipos != std::wstring::npos)
	{
		if (!CreateFolder(wstrFolder.substr(0, ipos).c_str()))
			return NULL;
	}
	FILE* pf = _wfopen(lpszFileName, L"w");
	return pf;
}

bool RemoveFile(LPCTSTR lpszFileName)
{
	if (DeleteFileW(lpszFileName))
		return true;
	return false;
}

bool CreateFolder(LPCTSTR lpszPath)
{
	if (!CreateDirectoryW(lpszPath, NULL))
	{
		DWORD dwError = GetLastError();
		if (dwError == ERROR_PATH_NOT_FOUND)
		{
			std::wstring wstrParent = lpszPath;
			int iParentLen = wstrParent.find_last_of(L'\\');
			if (iParentLen != std::wstring::npos)
			{
				if (CreateFolder(wstrParent.substr(0 , iParentLen).c_str()))
					return (bool)CreateDirectoryW(lpszPath, NULL);
				else
					return false;
			}
			else
				return false;
		}
		else if (dwError == ERROR_ALREADY_EXISTS)
			return true;
		else
			return false;
	}
	return true;
}

bool RemoveFolder(LPCTSTR lpszPath)
{
	if (RemoveDirectoryW(lpszPath))
		return true;
	return false;
}

}


}