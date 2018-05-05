#pragma once

#include "Routines.h"

class PublisherRoutines : public Routines {
	int _frameRate = 0;

	virtual void _setOptionsToPublisherSDK();

public:
	virtual void setInfo(const RoleInfo &info);
	virtual void run();
};