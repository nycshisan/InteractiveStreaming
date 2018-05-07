/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/
#pragma once

#include <memory>
#include "smart_player_sdk.h"


#define WM_USER_RENDR_WND_DB_CLICK			 (WM_USER + 301)

struct nt_rgb32_image
{
	nt_rgb32_image();
	~nt_rgb32_image();

	NT_BYTE*  data_;
	NT_UINT32 size_;
	NT_INT32 width_;
	NT_INT32 height_;
	NT_INT32 stride_;
};

// nt_render_wnd

class nt_render_wnd : public CWnd
{
	DECLARE_DYNAMIC(nt_render_wnd)

public:
	nt_render_wnd();
	virtual ~nt_render_wnd();

public:
	void SetPlayerSDKAPI(const SmartPlayerSDKAPI* api);
	void SetPlayerHandle(NT_HANDLE player_handle);

	void OnRGB32Image(const std::shared_ptr<nt_rgb32_image>& sp_image);

	void CleanRender();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

private:
	SmartPlayerSDKAPI player_api_;
	NT_HANDLE player_handle_;
	std::shared_ptr<nt_rgb32_image> rgb32_image_;
};


