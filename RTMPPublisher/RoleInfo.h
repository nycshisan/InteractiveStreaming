#pragma once

struct RoleInfo {
	bool isValid = false;
	std::wstring role;
	std::wstring id;
	std::wstring remoteURL;
	int frameRate = 0;
	int port;
};