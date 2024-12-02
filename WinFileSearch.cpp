#include"headers/WinFileSearch.h"
using namespace std;


// search the file in the current directory
bool files_in_directory(const wstring &FileName, string &result_path, const wstring &current_path) 
{
	
    WIN32_FIND_DATAW wfd;
    wstring filepathW = current_path + L"\\" + FileName;
    LPCWSTR filepath = filepathW.c_str();

    HANDLE const hFind = FindFirstFileW(filepath, &wfd);
    setlocale(LC_ALL, "");

    if (INVALID_HANDLE_VALUE != hFind)
    {
        do {
            if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) { 
                
                result_path = string(current_path.begin(), current_path.end()) + "\\" + string(FileName.begin(), FileName.end());
                 FindClose(hFind);
				return true;
            }
        } while (NULL != FindNextFileW(hFind, &wfd));

        FindClose(hFind);
    }
    return false;
}

bool WinFileSearch(const wstring& FileName, string& result_path, const wstring& current_path)
{
    if (files_in_directory(FileName, result_path, current_path)) return true;


    WIN32_FIND_DATAW wfd;
    wstring pathW = current_path + L"\\*";
    LPCWSTR path = pathW.c_str();

    HANDLE const hFind = FindFirstFileW(path, &wfd);
    setlocale(LC_ALL, "");

    if (INVALID_HANDLE_VALUE != hFind)
    {
        do {          
            if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && wfd.cFileName[0] != '.') { 
                // search in the subdirectory          
                if (WinFileSearch(FileName, result_path, current_path + L"\\" + wstring(wfd.cFileName))) {
					FindClose(hFind);
					return true; 
				}
                      
            }

        } while (NULL != FindNextFileW(hFind, &wfd));

        FindClose(hFind);
    }

    return false;
    
}


bool FileSearch(const string &FileName, string &result_path) {	
    wchar_t path[MAX_PATH + 1] = { 0 };
    GetCurrentDirectoryW(MAX_PATH, path);
    wstring current_pathW(&path[0]);
    wstring file_nameW = wstring(FileName.begin(), FileName.end());
    return WinFileSearch(file_nameW, result_path, current_pathW);
}