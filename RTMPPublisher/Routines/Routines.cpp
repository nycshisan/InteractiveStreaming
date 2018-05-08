#include "stdafx.h"
#include "Routines.h"

bool Routines::_openPublisherHandle(bool withScreen) {
	NT_PB_E_VIDEO_OPTION videoOption;
	NT_PB_E_AUDIO_OPTION audioOption;

	if (withScreen) {
		videoOption = NT_PB_E_VIDEO_OPTION_SCREEN;
	} else {
		videoOption = NT_PB_E_VIDEO_OPTION_NO_VIDEO;
	}

	audioOption = NT_PB_E_AUDIO_OPTION_CAPTURE_MIC;

	if (NT_ERC_OK != _publisherAPI.Open(&_publisherHandle, videoOption, audioOption, 0, NULL)) {
		std::wcout << L"Call publisher_api.Open failed" << std::endl;
		return false;
	}

	return true;
}

void Routines::_setAudioOptionsToPublisherSDK() {
	_publisherAPI.SetAuidoInputDeviceId(_publisherHandle, 0);
	_publisherAPI.SetPublisherAudioCodecType(_publisherHandle, 2);
	_publisherAPI.SetPublisherSpeexEncoderQuality(_publisherHandle, 8);
	_publisherAPI.SetEchoCancellation(_publisherHandle, 1, 100);
	_publisherAPI.SetNoiseSuppression(_publisherHandle, 1);
	_publisherAPI.SetAGC(_publisherHandle, 1);
	_publisherAPI.SetVAD(_publisherHandle, 1);
}

Routines::Routines() {
	ZeroMemory(&_publisherAPI, sizeof(_publisherAPI));
	NT_GetSmartPublisherSDKAPI(&_publisherAPI);

	if (NT_ERC_OK != _publisherAPI.Init(0, NULL)) {
		std::wcout << L"Call publisher_api.Init failed" << std::endl;
		return;
	}
}

Routines::~Routines() {
	_publisherAPI.StopPublisher(_publisherHandle);
	_publisherAPI.Close(_publisherHandle);
	_publisherAPI.UnInit();
}
