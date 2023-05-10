# Windows dir Command in C++

This is a C++ script that returns the equivalent results of the Windows dir command with the following options: 

dir /a 

dir /s 

dir /q

## Description

On a Windows terminal, the dir command returns a list of a directory's files and subdirectories. 

Dir Options:

/a: Prints all files and directories, including hidden ones. 

/s: Lists every occurrence of the specified file name within the specified directory and all subdirectories.

/q: Displays file ownership information.

This script will return equivelant results when executed from Visual Studio or when compiled and executed as an app.

### Using the program

Launch from Visual Studio or compile then launch from a Windows terminal. The shell will ask you to specify the directory and it will then run the dir command with all 3 options on the target directory. 

### Compiling

To compile it, launch Visual Studio's Developer Command Prompt for VS 
and navigate to the directory with the .cpp file 
Enter: cl /EHsc dir_code.cpp

## Help

Make sure to keep #pragma comment(lib, "advapi32.lib") which links necessary libraries to be able to use GetSecurityDescriptorOwner, LookupAccountSidA, and GetSecurityInfo functions in the dir /q function. 

Traverse directories you have permissions for or update the app to add more privileges. 

## Acknowledgments

Thanks to Gordon Long, adjunct professor at GMU, for pushing his students to challenge themselves. 
