//Link Necessary Libraries 
#pragma comment(lib, "advapi32.lib")
//Libraries 
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <aclapi.h>
#include <string>
#include <cstdlib>

using namespace std;

// Function to get the path from the user
string get_user_path() {
    string path;
    cout << "Please enter the path: ";
    getline(cin, path);
    return path;
}

//Handle Time Conversion
void print_time(FILETIME ft)
{
    //Convert File Time to Local File Time
    FILETIME local_ft;
    FileTimeToLocalFileTime(&ft, &local_ft);

    //Convert Local File Time to System Time 
    SYSTEMTIME st;
    FileTimeToSystemTime(&local_ft, &st);

    //Use MM/DD/YYYY HH:MM:SS to Print Date & Time
    printf("%02d/%02d/%04d %02d:%02d:%02d",
        st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);
}

// Return equivalent of dir /a
void dir_a(const string& path)
{
    WIN32_FIND_DATAA data;
    HANDLE hFind;
    string searchPath = path + "\\*"; // Add a wildcard to search for all files and folders
    hFind = FindFirstFileA(searchPath.c_str(), &data);

    // Loop through directory and recurse through subdirectories
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // Print existing files and directories
            cout << data.cFileName << " ";

            // Determine type
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Attach <DIR> next to directories
                cout << "<DIR>";
            }
            else
            {
                // Print file size (bytes)
                cout << data.nFileSizeLow << " bytes";
            }
            // Print file creation date and time
            cout << " ";
            print_time(data.ftCreationTime);
            cout << endl;

            // Continue to next file or directory
        } while (FindNextFileA(hFind, &data) != 0);

        FindClose(hFind);
    }
}


// Return equivalent of dir /s
void dir_s(const string& path)
{
    WIN32_FIND_DATAA data;
    HANDLE hFind;
    hFind = FindFirstFileA((path + "\\*").c_str(), &data);

    // Loop through directory and recurse through subdirectories
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // Determine type
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Avoid directories that begin with "." and ".."
                if (strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0)
                {
                    // Print name, <DIR> for directories, date and time
                    cout << path + "\\" + data.cFileName << " ";
                    cout << "<DIR>";
                    cout << " ";
                    print_time(data.ftCreationTime);
                    cout << endl;

                    // Recurse through subdirectories
                    dir_s(path + "\\" + data.cFileName);
                }
            }
            else
            {
                // Print file name, size (bytes), file creation date and time
                cout << path + "\\" + data.cFileName << " ";
                cout << data.nFileSizeLow << " bytes";
                cout << " ";
                print_time(data.ftCreationTime);
                cout << endl;
            }
            // Continue to next file or directory
        } while (FindNextFileA(hFind, &data) != 0);

        FindClose(hFind);
    }
}


// Return equivalent of dir /q
void dir_q(const string& path)
{
    string searchPath = path + "\\*.*";
    WIN32_FIND_DATAA data;
    HANDLE hFind;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    // Search files and directories
    hFind = FindFirstFileA(searchPath.c_str(), &data);
    if (hFind == INVALID_HANDLE_VALUE) {
        // List errors
        cerr << "Error: " << GetLastError() << endl;
        return;
    }

    // Iterate through files and directories
    do {
        string fileName = data.cFileName;
        // Avoid directories that begin with "." and ".."
        if (fileName == "." || fileName == "..") {
            continue;
        }

        // Path
        string filePath = path + "\\" + fileName;
        // Open file
        hFile = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            cerr << "Error: " << GetLastError() << endl;
            continue;
        }
        // Return file size
        DWORD dwSize = GetFileSize(hFile, NULL);

        // Print file name, size (bytes)
        cout << data.cFileName << " ";
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            cout << "<DIR>";
        }
        else {
            cout << dwSize << " bytes";
        }

        // Retrieve file security descriptor
        PSECURITY_DESCRIPTOR pSD = NULL;
        DWORD dwRes = GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, NULL, NULL, NULL, NULL, &pSD);
        if (dwRes == ERROR_SUCCESS)
        {
            // Retrieve file owner SID
            PSID pOwnerSid = NULL;
            BOOL bOwnerDefaulted = FALSE;
            if (GetSecurityDescriptorOwner(pSD, &pOwnerSid, &bOwnerDefaulted) == TRUE)
            {
                // Retrieve account name for SID
                TCHAR szAccountName[MAX_PATH];
                DWORD cchAccountName = MAX_PATH;
                TCHAR szDomainName[MAX_PATH];
                DWORD cchDomainName = MAX_PATH;
                SID_NAME_USE eUse;

                if (LookupAccountSid(NULL, pOwnerSid, szAccountName, &cchAccountName, szDomainName, &cchDomainName, &eUse) == TRUE)
                {
                    basic_string<TCHAR> ownerAccountName(szAccountName, cchAccountName);
                    basic_string<TCHAR> ownerDomainName(szDomainName, cchDomainName);

                    // Convert TCHAR strings to readable strings
                    string ownerAccountNameStr(ownerAccountName.begin(), ownerAccountName.end());
                    string ownerDomainNameStr(ownerDomainName.begin(), ownerDomainName.end());

                    // Print file owner name
                    cout << " " << ownerDomainNameStr << "\\" << ownerAccountNameStr;
                }
            }
        }

        // Print file creation date and time
        cout << " ";
        print_time(data.ftCreationTime);

        cout << endl;

        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    } while (FindNextFileA(hFind, &data));

    FindClose(hFind);
}


int main()
{
    // Get the user path
    string userPath = get_user_path();

    //Call dir_a Function & Print Results 
    cout << "dir /a from " << userPath << ":" << endl;
    dir_a(userPath);
    cout << endl;

    //Call dir_s Function & Print Results 
    cout << "dir /s from " << userPath << ":" << endl;
    dir_s(userPath);
    cout << endl;

    //Call dir_q Function & Print Results 
    cout << "dir /q from " << userPath << ":" << endl;
    dir_q(userPath);
    cout << endl;

    //Wait for User Input Before Exiting
    system("pause");

    //Print 0 for Successful Execution
    return 0;
}
