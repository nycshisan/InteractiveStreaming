#include "stdafx.h"
#include "Player.h"

void PlayerRoutines::_setOptionsToPublisherSDK() {
	_setAudioOptionsToPublisherSDK();
}

void PlayerRoutines::setInfo(const RoleInfo &info) {
	_id = info.id;
	_remoteURL = info.remoteURL;
	_port = info.port;
}

void PlayerRoutines::run() {
	_openPublisherHandle(false);
	_setOptionsToPublisherSDK();
	auto wURL = L"rtmp://localhost:" + std::to_wstring(_port) + L"/live/" + _id + L"_player";
	std::wcout << L"publishing at: " + wURL << std::endl;
	_publisherAPI.SetURL(_publisherHandle, std::string(wURL.begin(), wURL.end()).c_str(), NULL);

	if (NT_ERC_OK != _publisherAPI.StartPublisher(_publisherHandle, NULL)) {
		std::wcout << L"Call publisher_api.StartPublisher failed" << std::endl;
		return;
	}
}
