// RTMPPublisher.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "RoleInfo.h"
#include "Routines\Routines.h"

RoleInfo GetRole(int argc, WCHAR *argv[]) {
	RoleInfo roleInfo;

	if (argc < 2) {
		return roleInfo;
	}

	roleInfo.role = std::wstring(argv[1] + 1);
	if (roleInfo.role != L"publisher" && roleInfo.role != L"player") {
		std::wcout << L"Unknown role \"" + roleInfo.role + L"\"." << std::endl;
		return roleInfo;
	}

	if (roleInfo.role == L"publisher") {
		if (argc < 5) {
			return roleInfo;
		}
		roleInfo.id = std::wstring(argv[2]);
		roleInfo.frameRate = std::stoi(std::wstring(argv[3]));
		roleInfo.remoteURL = std::wstring(argv[4]);
	}

	if (roleInfo.role == L"player") {
		if (argc < 4) {
			return roleInfo;
		}
		roleInfo.id = std::wstring(argv[2]);
		roleInfo.remoteURL = std::wstring(argv[3]);
	}

	roleInfo.isValid = true;
	return roleInfo;
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

    auto roleInfo = GetRole(argc, argv);
	if (!roleInfo.isValid) {
		std::wcout << L"Invalid Role information - Wrong command line arguments." << std::endl;
		return 1;
	}

	std::unique_ptr<Routines> routines;

	if (roleInfo.role == L"publisher") {
		routines = std::make_unique<PublisherRoutines>();
	}
	if (roleInfo.role == L"player") {
		routines = std::make_unique<PlayerRoutines>();
	}

	routines->setInfo(roleInfo);
	routines->run();

	int timer = 0;

	while (!exitFlag.load() && timer < 1000 * 60 * 30) {
		Sleep(1000);
	}

    return 0;
}