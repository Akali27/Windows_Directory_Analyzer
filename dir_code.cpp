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

//Return Equivelant of dir /a  
void dir_a()
{
    WIN32_FIND_DATAA data;
    HANDLE hFind;
    string path = "C:\\Program Files\\Common Files\\*";
    hFind = FindFirstFileA(path.c_str(), &data);

    // Loop Through Directory & Recurse Through Subdirectories 
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {   //Print Existing Files & Directories 
            cout << data.cFileName << " ";

            //Determine Type 
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {   
                //Attach <DIR> Next to Directories
                cout << "<DIR>";
            }
            else
            {   //Print File Size (bytes)
                cout << data.nFileSizeLow << " bytes";
            }
            //Print Date & Time of Last Modification
            cout << " ";
            print_time(data.ftLastWriteTime);
            cout << endl;

            //Continue to Next File or Directory 
        } while (FindNextFileA(hFind, &data) != 0);

        FindClose(hFind);
    }
}

//Return Equivelant of dir /s
void dir_s(string path)
{
    WIN32_FIND_DATAA data;
    HANDLE hFind;
    hFind = FindFirstFileA((path + "\\*").c_str(), &data);

    // Loop Through Directory & Recurse Through Subdirectories
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {   //Determine Type
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //Avoid Directories that Begin with "." and ".."
                if (strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0)
                {
                    // Print Name, <DIR> for Directories, Date & Time 
                    cout << path + "\\" + data.cFileName << " ";
                    cout << "<DIR>";
                    cout << " ";
                    print_time(data.ftLastWriteTime);
                    cout << endl;
                    
                    //Recurse Through Subdirectories 
                    dir_s(path + "\\" + data.cFileName);
                }
            }
            else
            {
                //Print File Name, Size (bytes), Date & Time of Last Modification
                cout << path + "\\" + data.cFileName << " ";
                cout << data.nFileSizeLow << " bytes";
                cout << " ";
                print_time(data.ftLastWriteTime);
                cout << endl;
            }
            //Continue to Next File or Directory 
        } while (FindNextFileA(hFind, &data) != 0);

        FindClose(hFind);
    }
}

//Return Equivelant of dir /q
void dir_q()
{
    string path = "C:\\Users\\Ed\\Desktop\\SysInternalsSuite\\*.*";
    WIN32_FIND_DATAA data;
    HANDLE hFind;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    //Search Files and Directories 
    hFind = FindFirstFileA(path.c_str(), &data);
    if (hFind == INVALID_HANDLE_VALUE) {
        //List Errors
        cerr << "Error: " << GetLastError() << endl;
        return;
    }

    //Iterate Through Files & Directories  
    do {
        string fileName = data.cFileName;
        //Avoid Directories that Begin with "." and ".."
        if (fileName == "." || fileName == "..") {
            continue;
        }

        //Path
        string filePath = "C:\\Users\\Ed\\Desktop\\SysInternalsSuite\\" + fileName;
        //Open File 
        hFile = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            cerr << "Error: " << GetLastError() << endl;
            continue;
        }
        //Return File Size 
        DWORD dwSize = GetFileSize(hFile, NULL);

        //Print File Name, Size (bytes)
        cout << data.cFileName << " ";
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            cout << "<DIR>";
        }
        else {
            cout << dwSize << " bytes";
        }

        // Retrieve File Security Descriptor 
        PSECURITY_DESCRIPTOR pSD = NULL;
        DWORD dwRes = GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, NULL, NULL, NULL, NULL, &pSD);
        if (dwRes == ERROR_SUCCESS)
        {
            // Retrieve File Owner SID
            PSID pOwnerSid = NULL;
            BOOL bOwnerDefaulted = FALSE;
            if (GetSecurityDescriptorOwner(pSD, &pOwnerSid, &bOwnerDefaulted) == TRUE)
            {
                // Retrieve Account Name for SID
                TCHAR szAccountName[MAX_PATH];
                DWORD cchAccountName = MAX_PATH;
                TCHAR szDomainName[MAX_PATH];
                DWORD cchDomainName = MAX_PATH;
                SID_NAME_USE eUse;

                if (LookupAccountSid(NULL, pOwnerSid, szAccountName, &cchAccountName, szDomainName, &cchDomainName, &eUse) == TRUE)
                {
                    basic_string<TCHAR> ownerAccountName(szAccountName, cchAccountName);
                    basic_string<TCHAR> ownerDomainName(szDomainName, cchDomainName);

                    // Convert TCHAR Strings to Readable Strings
                    string ownerAccountNameStr(ownerAccountName.begin(), ownerAccountName.end());
                    string ownerDomainNameStr(ownerDomainName.begin(), ownerDomainName.end());

                    // Print File Owner Name
                    cout << " " << ownerDomainNameStr << "\\" << ownerAccountNameStr;
                }
            }
        }

        //Print Date & Time of Last Modification
        cout << " ";
        print_time(data.ftLastWriteTime);

        cout << endl;

        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    } while (FindNextFileA(hFind, &data));

    FindClose(hFind);
}

int main()
{
    //Call dir_a Function & Print Results 
    cout << "dir /a from C:\\Program Files\\Common Files:" << endl;
    dir_a();
    cout << endl;

    //Call dir_s Function & Print Results 
    cout << "dir /s from C:\\Program Files\\Common Files:" << endl;
    dir_s("C:\\Program Files\\Common Files");
    cout << endl;

    //Call dir_q Function & Print Results 
    cout << "dir /q from C:\\Users\\Ed\\Desktop\\SysInternalsSuite:" << endl;
    dir_q();
    cout << endl;

    //Pause & Wait for User Input Before Exiting
    system("pause");

    //Print 0 for Successful Execution
    return 0;
}