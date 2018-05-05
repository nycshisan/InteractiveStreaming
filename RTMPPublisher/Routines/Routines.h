#pragma once

#include "RoleInfo.h"

class Routines {
protected:
	NT_SmartPublisherSDKAPI _publisherAPI;
	NT_HANDLE _publisherHandle;

	std::wstring _id;

	bool _openPublisherHandle(bool withScreen);

	virtual void _setAudioOptionsToPublisherSDK();
	virtual void _setOptionsToPublisherSDK() = 0;

public:
	Routines();

	~Routines() {
		_publisherAPI.StopPublisher(_publisherHandle);
		_publisherAPI.Close(_publisherHandle);
		_publisherAPI.UnInit();
	}

	virtual void setInfo(const RoleInfo &info) = 0;
	virtual void run() = 0;
};

#include "Publisher.h"
#include "Player.h"