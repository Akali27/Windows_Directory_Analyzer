# Windows Directory Analyzer

This C++ program mimics the functionality of the Windows dir command with options '/a', '/s', and '/q' options. It retrieves and displays various file and directory attributes from a specified path.

## Features

Retrieve and display file and directory attributes in the specified path.
Mimic the functionality of the following Windows dir command options:
- /a: Lists all files and directories, including hidden and system files.
- /s: Lists files in the specified directory and all subdirectories.
- /q: Displays the owner of the file in addition to the file attributes.

## Usage

- Compile the program using a C++ compiler (e.g., Visual Studio, MinGW, etc.) or run it using the Visual Studio debugger. 
- Run the compiled program.
- When prompted, enter the path you want to analyze.
- The program will display the results of the dir /a, dir /s, and dir /q commands for the specified path.

Traverse directories you have permissions for or update the app to add more privileges. 

### Compiling

To compile the app, launch Visual Studio's Developer Command Prompt for VS 
and navigate to the directory with the .cpp file 
Enter: cl /EHsc dir_code.cpp

## Acknowledgments

Thanks to Gordon Long, adjunct professor at GMU, for pushing his students to challenge themselves. 
