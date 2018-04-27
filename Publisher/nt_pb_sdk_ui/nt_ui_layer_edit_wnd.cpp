/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

/*
从MFC继承的类一律不加名字空间，加了名字空间后，MFC向导会报错,
但还是放入nt_pb_sdk_ui目录下
*/

// nt_pb_layer_edit_wnd.cpp : implementation file
//

#include "stdafx.h"
#include "SmartPublisherDemo.h"
#include "nt_ui_layer_edit_wnd.h"
#include <algorithm>

// nt_ui_layer_edit_wnd

IMPLEMENT_DYNAMIC(nt_ui_layer_edit_wnd, CWnd)

nt_ui_layer_edit_wnd::nt_ui_layer_edit_wnd(NT_SmartPublisherSDKAPI* pb_sdk_api)
 :pb_sdk_api_(pb_sdk_api)
{
	screen_left_	= 0;
	screen_top_		= 0;
	screen_width_	= 0;
	srceen_height_	= 0;

	screen_mem_dc_	= NULL;
	screen_bmp_		= NULL;
}

nt_ui_layer_edit_wnd::~nt_ui_layer_edit_wnd()
{
	releaseScreenRegionBmp();
}


BEGIN_MESSAGE_MAP(nt_ui_layer_edit_wnd, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// nt_pb_layer_edit_wnd message handlers


void nt_ui_layer_edit_wnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	CRect rc_client(0, 0, 0, 0);
	GetClientRect(rc_client);

	if (rc_client.IsRectNull()
		|| rc_client.IsRectEmpty())
	{
		return;
	}

	auto mem_dc     = ::CreateCompatibleDC(dc.GetSafeHdc());
	auto mem_bitmap = ::CreateCompatibleBitmap(dc.GetSafeHdc(), rc_client.Width(), rc_client.Height());
	::SelectObject(mem_dc, mem_bitmap);

	HBRUSH brush = ::CreateSolidBrush(RGB(238, 243, 250));
	::FillRect(mem_dc, &rc_client, brush);
	::DeleteObject(brush);

	drawScreen(mem_dc, rc_client);

	drawCameras(mem_dc, rc_client);

	drawImage(mem_dc, rc_client);

	::BitBlt(dc.GetSafeHdc(),
		0, 0,
		rc_client.Width(), rc_client.Height(),
		mem_dc,
		0, 0,
		SRCCOPY);

	::DeleteObject(mem_bitmap);

	::DeleteDC(mem_dc);
}

void nt_ui_layer_edit_wnd::drawScreen(HDC hdc, const CRect& rc_client)
{
	ASSERT(hdc != NULL);
	ASSERT(!rc_client.IsRectNull());
	ASSERT(!rc_client.IsRectEmpty());

	if (screen_left_ < 0 || screen_top_ < 0)
		return;

	if (screen_width_ < 1 || srceen_height_ < 1)
		return;

	if (screen_left_ + screen_width_ > ::GetSystemMetrics(SM_CXSCREEN))
		return;

	if (screen_top_ + srceen_height_ > ::GetSystemMetrics(SM_CYSCREEN))
		return;

	if (screen_mem_dc_ == NULL)
		return;

	::SetStretchBltMode(hdc, HALFTONE);

	if (!::StretchBlt(hdc,
		0, 0,
		rc_client.right - rc_client.left,
		rc_client.bottom - rc_client.top,
		screen_mem_dc_,
		screen_left_, screen_top_,
		screen_width_,
		srceen_height_,
		SRCCOPY))
	{
	}
}

void nt_ui_layer_edit_wnd::drawCameras(HDC hdc, const CRect& rc_client)
{
	ASSERT(hdc != NULL);
	ASSERT(!rc_client.IsRectNull());
	ASSERT(!rc_client.IsRectEmpty());

	auto i = 0;

	for (auto& info : camera_infos_)
	{
		if (info->image_)
		{
			ASSERT(pb_sdk_api_ != NULL);

			pb_sdk_api_->drawImageAPI_.Draw(hdc,
				10, 10 + i*160, 180, 136,
				0, 0, info->image_->Width(), info->image_->Height(),
				info->image_->Get());


			if (i == 0)
			{
				pb_sdk_api_->drawImageAPI_.Draw(hdc,
					200, 10 + i * 160, 180, 136,
					0, 0, info->image_->Width(), info->image_->Height(),
					info->image_->Get());
			}
			else
			{
				pb_sdk_api_->drawImageAPI_.Draw(hdc,
					200, 10 + i * 100, 180, 136,
					0, 0, info->image_->Width(), info->image_->Height(),
					info->image_->Get());
			}
			

			i++;
		}
	}
}

void nt_ui_layer_edit_wnd::drawImage(HDC hdc, const CRect& rc_client)
{
	ASSERT(hdc != NULL);
	ASSERT(!rc_client.IsRectNull());
	ASSERT(!rc_client.IsRectEmpty());

	if (!image_)
		return;

	auto dst_w = image_->Width() < (rc_client.Width() - 2) ?
		image_->Width() : (rc_client.Width() - 2);

	auto dst_h = image_->Height() < (rc_client.Height() - 2) ?
		image_->Height() : (rc_client.Height() - 2);

	auto dst_x = rc_client.Width() / 2 - dst_w/ 2;
	auto dst_y = rc_client.Height() / 2 - dst_h / 2;

	//dst_x -= 100;

	ASSERT(pb_sdk_api_ != NULL);

	pb_sdk_api_->drawImageAPI_.Draw(hdc,
		dst_x, dst_y, dst_w, dst_h,
		0, 0, image_->Width(), image_->Height(),
		image_->Get());
}

void nt_ui_layer_edit_wnd::setScreenRegion(int left, int top, int width, int height)
{
	screen_left_	= left;
	screen_top_		= top;
	screen_width_	= width;
	srceen_height_	= height;
}

void nt_ui_layer_edit_wnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
	releaseScreenRegionBmp();

	KillTimer(IDT_TIMER_PULL_CAMERA_IMAGE_DRIVER);

	for ( auto& i : camera_infos_ )
	{
		ASSERT(pb_sdk_api_ != nullptr);
		pb_sdk_api_->StopGetVideoCaptureDeviceImage(i->image_handle_);
		i->image_handle_ = nullptr;
	}

	camera_infos_.clear();
}

void nt_ui_layer_edit_wnd::releaseScreenRegionBmp()
{
	if (screen_bmp_ != NULL)
	{
		::DeleteObject(screen_bmp_);
		screen_bmp_ = NULL;
	}

	if (screen_mem_dc_ != NULL)
	{
		::DeleteDC(screen_mem_dc_);
		screen_mem_dc_ = NULL;
	}
}

void nt_ui_layer_edit_wnd::createScreenRegionBmp()
{
	if (!::IsWindow(GetSafeHwnd()))
		return;

	releaseScreenRegionBmp();

	HDC hdcScreen = ::GetDC(NULL);
	if (hdcScreen == NULL)
		return;

	HDC hdcWindow = ::GetDC(GetSafeHwnd());
	if (hdcWindow == NULL)
	{
		goto done;
	}

	screen_mem_dc_ = ::CreateCompatibleDC(hdcWindow);
	if (screen_mem_dc_ == NULL)
	{
		goto done;
	}

	screen_bmp_ = ::CreateCompatibleBitmap(hdcWindow,
		::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));

	if (screen_bmp_ == NULL)
	{
		::DeleteDC(screen_mem_dc_);
		screen_mem_dc_ = NULL;

		goto done;
	}

	::SelectObject(screen_mem_dc_, screen_bmp_);

	if (!BitBlt(screen_mem_dc_,
		0, 0,
		::GetSystemMetrics(SM_CXSCREEN),
		::GetSystemMetrics(SM_CYSCREEN),
		hdcScreen,
		0, 0,
		SRCCOPY))
	{
		goto done;
	}

done:

	if (hdcScreen != NULL)
	{
		::ReleaseDC(NULL, hdcScreen);
		hdcScreen = NULL;
	}

	if (hdcWindow != NULL)
	{
		::ReleaseDC(GetSafeHwnd(), hdcWindow);
		hdcWindow = NULL;
	}
}


void nt_ui_layer_edit_wnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if ( IDT_TIMER_PULL_CAMERA_IMAGE_DRIVER == nIDEvent )
	{
		updateCameraImage();
	}

	CWnd::OnTimer(nIDEvent);
}


int nt_ui_layer_edit_wnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(IDT_TIMER_PULL_CAMERA_IMAGE_DRIVER, IDT_TIMER_PULL_CAMERA_IMAGE_ELAPSE, NULL);

	return 0;
}

void nt_ui_layer_edit_wnd::addCamera(const std::string& camera_id)
{
	auto iter = std::find_if(begin(camera_infos_), end(camera_infos_),
		[&camera_id](const std::shared_ptr<CameraImageInfo>& info)->bool
	{
		return camera_id == info->id_;
	});

	if ( iter != end(camera_infos_) )
	{
		return;
	}

	NT_HANDLE pDevcieImageHandle = NULL;

	auto ret = pb_sdk_api_->StartGetVideoCaptureDeviceImage(&pDevcieImageHandle, camera_id.c_str());
	if ( NT_ERC_OK == ret )
	{
		auto info = std::make_shared < CameraImageInfo >();
		info->id_ = camera_id;
		info->image_handle_ = pDevcieImageHandle;
		camera_infos_.push_back(info);
	}
}

void nt_ui_layer_edit_wnd::updateCameraImage()
{
	auto is_need_update = false;

	for (auto& i : camera_infos_)
	{
		auto sp_image = std::make_shared<nt_pb_sdk::image_wrapper>(pb_sdk_api_->ImageAPI_);

		auto ret = pb_sdk_api_->GetVideoCaptureDeviceImage(i->image_handle_, 1, sp_image->getPP());

		if ( NT_ERC_OK == ret )
		{
			i->image_ = sp_image;

			is_need_update = true;
		}
	}

	// 更新界面
	if ( is_need_update )
	{
		InvalidateRect(NULL, FALSE);
	}
	
}

void nt_ui_layer_edit_wnd::setImage(std::shared_ptr<nt_pb_sdk::image_wrapper> image)
{
	image_ = image;

	if (::IsWindow(m_hWnd))
	{
		InvalidateRect(NULL, FALSE);
	}
}



