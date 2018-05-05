#pragma once

#include "Routines.h"

class PlayerRoutines : public Routines {
	std::wstring _remoteURL;

	virtual void _setOptionsToPublisherSDK();

public:
	virtual void setInfo(const RoleInfo &info);
	virtual void run();
};