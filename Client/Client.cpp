// Client.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

bool ShellExecuteCmd(std::wstring cmd) {
	HINSTANCE hInst =  ShellExecute(NULL, L"open", cmd.c_str(), NULL, NULL, SW_HIDE);
	if (reinterpret_cast<int>(hInst) < 32) {
		std::wcout << L"ShellExecuteCmd \"" + cmd + L"\" failed (" + std::to_wstring(reinterpret_cast<int>(hInst)) + L")." << std::endl;
		return false;
	}
	return true;
}

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
		ShellExecuteCmd(L".\\nginx.lnk");
	}

	Sleep(50000);

	if (role == L"publisher") {
		// stop the rtmp server
		ShellExecuteCmd(L".\\stopnginx.lnk");
	}


	

	return 0;
}