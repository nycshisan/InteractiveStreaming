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

std::wstring GetRole(int argc, WCHAR *argv[]) {
	if (argc != 2) {
		std::wcout << L"Usage: " + std::wstring(argv[0]) + L" [role]" << std::endl;
		return L"";
	}

	std::wstring role(argv[1] + 1);
	if (role != L"publisher" && role != L"player") {
		std::wcout << L"Unknown role \"" + role + L"\"." << std::endl;
		return L"";
	}

	return role;
}

std::atomic<bool> exitFlag = false;

auto ExitRoutines = [](DWORD fdwCtrlType) -> BOOL {
	switch (fdwCtrlType) {
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		std::wcout << L"Running exit routines." << std::endl;
		exitFlag.store(true);
		Sleep(10000);
		return TRUE;
	default:
		return FALSE;
	}
};

int wmain(int argc, WCHAR *argv[]) {
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ExitRoutines, TRUE);

	auto role = GetRole(argc, argv);

	if (role == L"publisher") {
		// Start up rtmp server
		ShellExecuteCmd(L".\\nginx.lnk");
	}

	while (!exitFlag.load()) {
		Sleep(1);
	}

	if (role == L"publisher") {
		// stop the rtmp server
		ShellExecuteCmd(L".\\stopnginx.lnk");
	}

	return 0;
}