#include "stdafx.h"
#include "Player.h"

void PlayerRoutines::_setOptionsToPublisherSDK() {
	_setAudioOptionsToPublisherSDK();
}

void PlayerRoutines::setInfo(const RoleInfo &info) {
	_id = info.id;
	_remoteURL = info.remoteURL;
}

void PlayerRoutines::run() {
	_openPublisherHandle(false);
	_setOptionsToPublisherSDK();
	auto wURL = L"rtmp://" + _remoteURL + L":1935/live/" + _id + L"_player";
	_publisherAPI.SetURL(_publisherHandle, std::string(wURL.begin(), wURL.end()).c_str(), NULL);

	if (NT_ERC_OK != _publisherAPI.StartPublisher(_publisherHandle, NULL)) {
		std::wcout << L"Call publisher_api.StartPublisher failed" << std::endl;
		return;
	}
}
