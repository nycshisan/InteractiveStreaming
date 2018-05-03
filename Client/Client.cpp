// Client.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int wmain(int argc, WCHAR *argv[]) {
	if (argc != 2) {
		std::wcout << L"Usage: " + std::wstring(argv[0]) + L" [role]" << std::endl;
		return 1;
	}

	std::wstring role(argv[1] + 1);
	if (role != L"publisher" && role != L"player") {
		std::wcout << L"Unknown role \"" + role + L"\"." << std::endl;
		return 2;
	}

	if (role == L"publisher") {
		// Start up rtmp server
	}


	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;

	//ZeroMemory(&si, sizeof(si));
	//si.cb = sizeof(si);
	//ZeroMemory(&pi, sizeof(pi));



	//if (!CreateProcess(NULL,   // No module name (use command line)
	//	argv[1],        // Command line
	//	NULL,           // Process handle not inheritable
	//	NULL,           // Thread handle not inheritable
	//	FALSE,          // Set handle inheritance to FALSE
	//	0,              // No creation flags
	//	NULL,           // Use parent's environment block
	//	NULL,           // Use parent's starting directory 
	//	&si,            // Pointer to STARTUPINFO structure
	//	&pi)           // Pointer to PROCESS_INFORMATION structure
	//	) {
	//	printf("CreateProcess failed (%d).\n", GetLastError());
	//	return;
	//}

	return 0;
}