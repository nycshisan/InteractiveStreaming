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

void RunProcess(const std::wstring &appName, const std::wstring &cmd, PROCESS_INFORMATION &pi) {
	STARTUPINFO si;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	const int maxPathLength = 200; WCHAR cd[maxPathLength];
	GetCurrentDirectory(maxPathLength, cd);
	std::wstring cd_wstr(cd);

	WCHAR cmdBuf[32768];
	wcscpy_s(cmdBuf, (cd_wstr + L"\\" + appName + L" " + cmd).c_str());

	std::wcout << L"Running \"" + cd_wstr + L"\\" + appName + L" " + cmd + L"\"" << std::endl;

	if (!CreateProcess(appName.c_str(), cmdBuf, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		auto errCode = GetLastError();
		std::wcout << L"Create process \"" + appName + L"\" failed(" << errCode << L")." << std::endl;
	}

	return;
}

DWORD WINAPI RunPublisher(LPVOID lpParam) {
	std::wstring *params = static_cast<std::wstring*>(lpParam);
	std::wstring &role = params[0], &url = params[1], &id = params[2];
	PROCESS_INFORMATION pi;
	if (role == L"publisher") {
		RunProcess(L"SmartPublisherDemo.exe", L"-" + role + L" " + id + L" " + L"30", pi);
	}
	if (role == L"player") {
		RunProcess(L"SmartPublisherDemo.exe", L"-" + role + L" " + id + L" " + url, pi);
	}


	return 0;
}

#define SET_CURL(handle, option, value) do { \
	if (curl_easy_setopt((handle), CURLOPT_##option, (value)) != CURLE_OK) { \
		std::wcout << L"Curl setopt " << L#option << L" failed." << std::endl; \
		return false; \
	} \
} while (0)

size_t PublisherCheckWriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
	bool *ok = reinterpret_cast<bool*>(userdata);
	std::string s(ptr, nmemb);
	*ok = s == "OK!";
	return nmemb;
}

bool PostRemoteInfo(const std::wstring &remoteIP, const std::wstring &id) {
	std::string remoteIPStr(remoteIP.begin(), remoteIP.end()), idStr(id.begin(), id.end());
	std::string remoteAddr = "http://" + remoteIPStr + ":8080";
	std::string data = "id=" + idStr;

	bool ok = false;

	auto hCurl = curl_easy_init();
	SET_CURL(hCurl, URL, remoteAddr.c_str());
	SET_CURL(hCurl, LOCALPORT, 1935L);
	SET_CURL(hCurl, POST, 1L);
	SET_CURL(hCurl, COPYPOSTFIELDS, data.c_str());
	SET_CURL(hCurl, WRITEFUNCTION, PublisherCheckWriteCallback);
	SET_CURL(hCurl, WRITEDATA, &ok);

	auto code = curl_easy_perform(hCurl);
	curl_easy_cleanup(hCurl);
	if (code != 0) {
		std::wcout << L"Curl failed: " << code << std::endl;
		return false;
	}
	return ok;
}

bool GetRemoteInfo(const std::wstring &remoteIP, const std::wstring &id, std::wstring &resultURL) {
	return false;
}

int wmain(int argc, WCHAR *argv[]) {
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ExitRoutines, TRUE);

	std::wstring role = GetRole(argc, argv);

	// Read Confs
	std::wifstream confFileStream("conf.txt");
	std::map<std::wstring, std::wstring> confs;
	while (!confFileStream.eof()) {
		std::wstring confVar, confSign, confValue;
		confFileStream >> confVar >> confSign >> confValue;
		assert(confSign == L"=");
		confs[confVar] = confValue;
	}
	std::wstring id = confs[L"id"];

	// Get remote infos
	std::wstring url, remoteIP = confs[L"server"];
	bool success;
	if (role == L"publisher") {
		url = L"rtmp://localhost:1935/live/";
		success = PostRemoteInfo(remoteIP, id);
	}
	if (role == L"player") {
		success = GetRemoteInfo(remoteIP, id, url);
	}
	if (!success) {
		std::wcout << L"Connect to P2P server failed." << std::endl;
		return 1;
	}


	// Start up rtmp server
	if (role == L"publisher") {
		std::wcout << L"Start nginx-rtmp server." << std::endl;
		ShellExecuteCmd(L".\\nginx.lnk");
	}

	// Run publisher
	std::wstring pPublisherThreadParams[3] = { role, url, id };
	auto hPublisherThread = CreateThread(NULL, 0, RunPublisher, pPublisherThreadParams, 0, NULL);

	// Run player
	PROCESS_INFORMATION pi;
	RunProcess(L"SmartPlayer.exe", role + L" " + url + id + L"_publisher", pi);

	while (!exitFlag.load()) {
		Sleep(1);
	}

	if (role == L"publisher") {
		// stop the rtmp server
		std::wcout << L"Stop nginx-rtmp server" << std::endl;
		ShellExecuteCmd(L".\\stopnginx.lnk");
	}

	return 0;
}