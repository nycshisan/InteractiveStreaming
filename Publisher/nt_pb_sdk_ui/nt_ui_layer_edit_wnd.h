/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

/*
��MFC�̳е���һ�ɲ������ֿռ䣬�������ֿռ��MFC�򵼻ᱨ��, 
�����Ƿ���nt_pb_sdk_uiĿ¼��
*/

/*
Ŀǰ��Щ���붼��չʾ���ʹ�ýӿڵģ���ʱ�����
*/

#pragma once

#include <vector>
#include <string>
#include <memory>
#include "nt_image_wrapper.h"

struct CameraImageInfo
{
	CameraImageInfo()
	{
		image_handle_ = nullptr;
	}

	std::string id_;
	NT_HANDLE image_handle_;
	std::shared_ptr<nt_pb_sdk::image_wrapper> image_;
};

// nt_ui_layer_edit_wnd

class nt_ui_layer_edit_wnd : public CWnd
{
	DECLARE_DYNAMIC(nt_ui_layer_edit_wnd)

public:
	explicit nt_ui_layer_edit_wnd(NT_SmartPublisherSDKAPI* pb_sdk_api);
	virtual ~nt_ui_layer_edit_wnd();

public:
	void setSDKAPI(NT_SmartPublisherSDKAPI* api) { pb_sdk_api_ = api; }
	void setScreenRegion(int left, int top, int width, int height);
	void createScreenRegionBmp();
	void addCamera(const std::string& camera_id);

	void setImage(std::shared_ptr<nt_pb_sdk::image_wrapper> image);
protected:
	DECLARE_MESSAGE_MAP()

private:
	enum
	{
		IDT_TIMER_PULL_CAMERA_IMAGE_DRIVER = 300,
		IDT_TIMER_PULL_CAMERA_IMAGE_ELAPSE = 125, // 125ms��ȥһ�Σ�GDI���ܱȽ�����̫Ƶ��Ӱ������
	};

private:
	void drawScreen(HDC hdc, const CRect& rc_client);
	void drawCameras(HDC hdc, const CRect& rc_client);
	void drawImage(HDC hdc, const CRect& rc_client);
private:
	void releaseScreenRegionBmp();
	void updateCameraImage();

private:
	NT_SmartPublisherSDKAPI* pb_sdk_api_;

private:
	int screen_left_;
	int screen_top_;
	int screen_width_;
	int srceen_height_;

	HDC		screen_mem_dc_;
	HBITMAP screen_bmp_;

private:
	std::vector<std::shared_ptr<CameraImageInfo> > camera_infos_;

private:
	std::shared_ptr<nt_pb_sdk::image_wrapper> image_;

public:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
};




