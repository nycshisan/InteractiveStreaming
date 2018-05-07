/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/
#pragma once


// nt_wrapper_render_wnd
#include <memory>
#include "nt_render_wnd.h"
#include "smart_player_sdk.h"

class nt_wrapper_render_wnd : public CWnd
{
	DECLARE_DYNAMIC(nt_wrapper_render_wnd)

public:
	nt_wrapper_render_wnd();
	virtual ~nt_wrapper_render_wnd();

public:
	HWND RenderWnd();
	void SetPlayerSDKAPI(const SmartPlayerSDKAPI* api);
	void SetPlayerHandle(NT_HANDLE player_handle);
	void SetVideoSize(int width, int height);
	void ClearVideoSize();
	bool IsFullScreen() const { return is_full_screen_;  }
	void CalScaleSize(int limit_w, int limit_h, int video_w, int video_h,
		int& w, int& h);
	void FullScreenSwitch();

	void OnRGB32Image(const std::shared_ptr<nt_rgb32_image>& sp_image);

	void CleanRender();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnRenderWndDBClick(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	DECLARE_MESSAGE_MAP()

private:
	bool IsCanFullScreen();
	
private:
	bool is_full_screen_;
	HWND old_parent_wnd_;
	CRect old_rect_;
	nt_render_wnd render_wnd_;
	SmartPlayerSDKAPI player_api_;
	NT_HANDLE player_handle_;
	int video_width_;
	int video_height_;
};


