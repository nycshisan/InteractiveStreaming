/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/
// nt_render_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "SmartPlayer.h"
#include "nt_render_wnd.h"


nt_rgb32_image::nt_rgb32_image()
{
	data_ = NULL;
	size_ = 0;

	width_	= 0;
	height_ = 0;
	stride_ = 0;
}

nt_rgb32_image::~nt_rgb32_image()
{
	if (data_ != NULL)
	{
		delete[] data_;
		data_ = NULL;
	}
}

// nt_render_wnd

IMPLEMENT_DYNAMIC(nt_render_wnd, CWnd)

nt_render_wnd::nt_render_wnd()
	:player_handle_(NULL)
{
	memset(&player_api_, 0, sizeof(player_api_));
}

nt_render_wnd::~nt_render_wnd()
{
}

void nt_render_wnd::SetPlayerSDKAPI(const SmartPlayerSDKAPI* api)
{
	ASSERT(api != nullptr);
	player_api_ = *api;
}

void nt_render_wnd::SetPlayerHandle(NT_HANDLE player_handle)
{
	player_handle_ = player_handle;
}

void nt_render_wnd::OnRGB32Image(const std::shared_ptr<nt_rgb32_image>& sp_image)
{
	rgb32_image_ = sp_image;

	if (m_hWnd != NULL && ::IsWindow(m_hWnd))
	{
		InvalidateRect(NULL);
		UpdateWindow();
	}
}

void nt_render_wnd::CleanRender()
{
	rgb32_image_.reset();

	if (m_hWnd != NULL && ::IsWindow(m_hWnd))
	{
		InvalidateRect(NULL);
		UpdateWindow();
	}
}


BEGIN_MESSAGE_MAP(nt_render_wnd, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// nt_render_wnd message handlers


void nt_render_wnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	auto parent_wnd = ::GetParent(m_hWnd);
	if ( parent_wnd != NULL )
	{
		::PostMessage(parent_wnd, WM_USER_RENDR_WND_DB_CLICK, 0, 0);
	}
	
	CWnd::OnLButtonDblClk(nFlags, point);
}


void nt_render_wnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	if ( IsIconic() )
	{
		return;
	}

	// 先绘制一个黑色的背景
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

	if ( rgb32_image_ )
	{
		if ( player_api_.GDIDrawRGB32 != NULL
			&& player_handle_ != NULL )
		{
			player_api_.GDIDrawRGB32(player_handle_, mem_dc,
				0, 0,
				rc_client.Width(), rc_client.Height(),
				0, 0,
				rgb32_image_->width_, rgb32_image_->height_,
				rgb32_image_->data_, rgb32_image_->size_,
				rgb32_image_->width_, rgb32_image_->height_,
				rgb32_image_->stride_);
		}
	}

	::BitBlt(dc.GetSafeHdc(),
		0, 0,
		rc_client.Width(), rc_client.Height(),
		mem_dc,
		0, 0,
		SRCCOPY);

	::DeleteObject(mem_bitmap);

	::DeleteDC(mem_dc);
}





