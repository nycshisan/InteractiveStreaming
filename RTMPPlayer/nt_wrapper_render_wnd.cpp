/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/
// nt_wrapper_render_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "SmartPlayer.h"
#include "nt_wrapper_render_wnd.h"


// nt_wrapper_render_wnd

IMPLEMENT_DYNAMIC(nt_wrapper_render_wnd, CWnd)

nt_wrapper_render_wnd::nt_wrapper_render_wnd()
	:is_full_screen_(false),
	old_parent_wnd_(NULL),
	player_handle_(NULL),
	video_width_(0),
	video_height_(0)
{
	memset(&player_api_, 0, sizeof(player_api_));
}

nt_wrapper_render_wnd::~nt_wrapper_render_wnd()
{
}


BEGIN_MESSAGE_MAP(nt_wrapper_render_wnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_USER_RENDR_WND_DB_CLICK, &nt_wrapper_render_wnd::OnRenderWndDBClick)
	ON_WM_PAINT()
	ON_WM_CHAR()
END_MESSAGE_MAP()


HWND nt_wrapper_render_wnd::RenderWnd()
{
	return render_wnd_.m_hWnd;
}

void nt_wrapper_render_wnd::SetPlayerSDKAPI(const SmartPlayerSDKAPI* api)
{
	ASSERT(api != nullptr);
	player_api_ = *api;

	render_wnd_.SetPlayerSDKAPI(api);
}

void nt_wrapper_render_wnd::SetPlayerHandle(NT_HANDLE player_handle)
{
	player_handle_ = player_handle;

	render_wnd_.SetPlayerHandle(player_handle);
}

void nt_wrapper_render_wnd::SetVideoSize(int width, int height)
{
	auto old_w = video_width_;
	auto old_h = video_height_;

	video_width_  = width;
	video_height_ = height;

	if ( ( old_w != video_width_ || old_h != video_height_ )
		&& video_width_ > 0
		&& video_height_ > 0
		&& IsFullScreen()
		&& ::IsWindow(m_hWnd)
		&& ::IsWindow(render_wnd_.m_hWnd) )
	{
		// 这种情况一般出现在播放过程中分辨率改变了，并且还是全屏状态

		CRect rc_client;
		GetClientRect(&rc_client);

		if ( rc_client.Width() > 10 && rc_client.Height() > 10 )
		{
			int w = rc_client.Width();
			int h = rc_client.Height();

			CalScaleSize(rc_client.Width(), rc_client.Height(), video_width_, video_height_, w, h);

			auto x = rc_client.Width() / 2  - w / 2;
			auto y = rc_client.Height() / 2 - h / 2;

			if ( x < 0 )
				x = 0;

			if ( y < 0 )
				y = 0;

			render_wnd_.MoveWindow(x, y, w, h, TRUE);

			if ( player_handle_ != NULL )
			{
				CRect wndC;
				render_wnd_.GetClientRect(&wndC);

				ASSERT(player_api_.OnWindowSize != NULL);
				player_api_.OnWindowSize(player_handle_, wndC.Width(), wndC.Height());
			}
		}
	}
}

void nt_wrapper_render_wnd::ClearVideoSize()
{
	video_width_  = 0;
	video_height_ = 0;
}

// nt_wrapper_render_wnd message handlers

int nt_wrapper_render_wnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 )
		return -1;

	auto wc = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW));

	CRect rcClient;
	GetClientRect(rcClient);

	auto wnd_ret = render_wnd_.Create(wc, _T("smartrender"),
		WS_CHILD | WS_VISIBLE, rcClient, this, 228);

	return 0;
}

void nt_wrapper_render_wnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if ( ::IsWindow(render_wnd_.m_hWnd) )
	{
		if ( !IsFullScreen() )
		{
			render_wnd_.MoveWindow(0, 0, cx, cy, TRUE);

			if ( player_handle_ != NULL )
			{
				CRect wndC;
				render_wnd_.GetClientRect(&wndC);

				ASSERT(player_api_.OnWindowSize != NULL);
				player_api_.OnWindowSize(player_handle_, wndC.Width(), wndC.Height());
			}
		}
		else
		{
			if ( cx < 10 || cy < 10 )
				return;

			if ( video_height_ < 1 || video_width_ < 1 )
				return;

			int w = cx;
			int h = cy;

			CalScaleSize(cx, cy, video_width_, video_height_, w, h);

			auto x = cx/2 - w/2;
			auto y = cy/2 - h/2;

			if ( x < 0 )
				x = 0;

			if ( y < 0 )
				y = 0;

			render_wnd_.MoveWindow(x, y, w, h, TRUE);

			if ( player_handle_ != NULL )
			{
				CRect wndC;
				render_wnd_.GetClientRect(&wndC);

				ASSERT(player_api_.OnWindowSize != NULL);
				player_api_.OnWindowSize(player_handle_, wndC.Width(), wndC.Height());
			}
		}
	}
}

void nt_wrapper_render_wnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	FullScreenSwitch();

	CWnd::OnLButtonDblClk(nFlags, point);
}

LRESULT nt_wrapper_render_wnd::OnRenderWndDBClick(WPARAM wParam, LPARAM lParam)
{
	FullScreenSwitch();

	return S_OK;
}

bool nt_wrapper_render_wnd::IsCanFullScreen()
{
	return video_width_ > 0 && video_height_ > 0;
}

void nt_wrapper_render_wnd::FullScreenSwitch()
{
	if ( !::IsWindow(m_hWnd) )
		return;

	if ( !IsFullScreen() )
	{
		if ( !IsCanFullScreen() )
			return;

		auto old_wnd = GetParent();
		if ( old_wnd == NULL )
			return;

		old_parent_wnd_ = old_wnd->m_hWnd;
		ASSERT(::IsWindow(old_parent_wnd_));

		GetWindowRect(&old_rect_);
		old_wnd->ScreenToClient(old_rect_);
	
		HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
		if (hMonitor == NULL)
			return;

		MONITORINFOEX mi;
		mi.cbSize = sizeof(mi);

		if ( !GetMonitorInfo(hMonitor, &mi) )
			return;

		/*if ( mi.dwFlags&MONITORINFOF_PRIMARY )
		{
		OutputDebugStringA("This is the primary display monitor\r\n");
		}*/

		::SetParent(m_hWnd, NULL);

		auto wnd_styles = GetWindowLongPtr(m_hWnd, GWL_STYLE);
		wnd_styles = wnd_styles & (~WS_CHILD);	
		wnd_styles |= WS_POPUP;
		::SetWindowLongPtr(m_hWnd, GWL_STYLE, wnd_styles);

		// 影藏老窗口
		::ShowWindow(old_parent_wnd_, SW_HIDE);

		is_full_screen_ = true;

		::SetWindowPos(m_hWnd, HWND_TOP, mi.rcMonitor.left,
			mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			SWP_FRAMECHANGED);

		::SetForegroundWindow(m_hWnd);

		::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW
			| RDW_ERASE);

		::RedrawWindow(NULL, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW
			| RDW_ERASE);
	}
	else
	{
		::ShowWindow(old_parent_wnd_, SW_SHOW);

		auto wnd_styles = GetWindowLongPtr(m_hWnd, GWL_STYLE);
		wnd_styles = wnd_styles & (~WS_POPUP);
		wnd_styles |= WS_CHILD;
		auto ret = SetWindowLongPtr(m_hWnd, GWL_STYLE, wnd_styles);

		::SetParent(m_hWnd, old_parent_wnd_);

		is_full_screen_ = false;

		MoveWindow(old_rect_, TRUE);
	}
}

void nt_wrapper_render_wnd::OnRGB32Image(const std::shared_ptr<nt_rgb32_image>& sp_image)
{
	render_wnd_.OnRGB32Image(sp_image);
}

void nt_wrapper_render_wnd::CleanRender()
{
	render_wnd_.CleanRender();

	if ( m_hWnd != NULL && ::IsWindow(m_hWnd) )
	{
		InvalidateRect(NULL);
		UpdateWindow();
	}
}

void nt_wrapper_render_wnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	// 绘制一个黑色的背景

	CRect rc_client(0, 0, 0, 0);
	GetClientRect(rc_client);

	if ( rc_client.IsRectNull()
		|| rc_client.IsRectEmpty() )
	{
		return;
	}

	auto mem_dc = ::CreateCompatibleDC(dc.GetSafeHdc());
	if ( mem_dc == NULL )
		return;

	auto mem_bitmap = ::CreateCompatibleBitmap(dc.GetSafeHdc(), rc_client.Width(), rc_client.Height());
	if ( mem_bitmap == NULL )
	{
		::DeleteDC(mem_dc);
		return;
	}

	::SelectObject(mem_dc, mem_bitmap);

	HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0));
	::FillRect(mem_dc, &rc_client, brush);
	::DeleteObject(brush);

	::BitBlt(dc.GetSafeHdc(),
		0, 0,
		rc_client.Width(), rc_client.Height(),
		mem_dc,
		0, 0,
		SRCCOPY);

	::DeleteObject(mem_bitmap);

	::DeleteDC(mem_dc);
}

void nt_wrapper_render_wnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	
	// 全屏的时候，按ESC将推出全屏
	if ( nChar == VK_ESCAPE )
	{
		if ( IsFullScreen() )
		{
			FullScreenSwitch();
		}
	}

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void nt_wrapper_render_wnd::CalScaleSize(int limit_w, int limit_h, 
	int video_w, int video_h,
	int& w, int& h)
{
	w = limit_w;
	h = limit_h;

	ASSERT(limit_h != 0);
	ASSERT(video_w != 0);
	ASSERT(video_h != 0);

	if (video_width_ < 1)
		return;

	if (video_height_ < 1)
		return;

	auto limit_ratio = limit_w*1.0 / limit_h;
	auto video_ratio = video_width_*1.0 / video_height_;

	auto diff_ratio = abs(limit_ratio - video_ratio);
	if ( diff_ratio < 0.119 )
	{
		//  比例差距很小的话，就直接缩放
		w = limit_w;
		h = limit_h;
	}
	else
	{
		if ( video_ratio >= limit_ratio )
		{
			// 按宽缩放
			w = limit_w;
			h = (int)(w * video_height_*1.0 / video_width_);
		}
		else
		{
			// 按高缩放
			h = limit_h;
			w = (int)(h * video_width_*1.0 / video_height_);
		}
	}

	if ( w > limit_w )
		w = limit_w;

	if ( h > limit_h )
		h = limit_h;
}


