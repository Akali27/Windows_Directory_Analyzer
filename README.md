# Dir in C++

This is a C++ script that returns the equivalent results of the Windows commands: 
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

### Installing program

Change the paths of the directories you want to traverse for each of the three functions. 

For dir /a: 
Change string path and under int main ()

for dir /s: 
Change path under int main() 

for dir /q: 
Change string path, string file path, and path under int main()

### Executing program

Run the script from Visual Studio or compile it as an app and run it. 

To compile it, launch Visual Studio's Developer Command Prompt for VS 
and navigate to the directory with the .cpp file 
Enter: cl /EHsc dir_code.cpp

## Help

Make sure to keep #pragma comment(lib, "advapi32.lib") which links necessary libraries to be able to use GetSecurityDescriptorOwner, LookupAccountSidA, and GetSecurityInfo functions in the dir /q function. 

Traverse directories you have permissions for or update the app to add more privileges. 

## Acknowledgments

Thanks to Gordon Long, adjunct professor at GMU, for pushing his students to challenge themselves. 
