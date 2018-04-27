/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/


// SmartPublisherDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSmartPublisherDemoApp:
// See SmartPublisherDemo.cpp for the implementation of this class
//

class CSmartPublisherDemoApp : public CWinApp
{
public:
	CSmartPublisherDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSmartPublisherDemoApp theApp;