/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

#pragma once

#include "SmartPublisherSDK/nt_smart_publisher_sdk.h"

#include <string>
#include "afxwin.h"

// nt_image_layer_config_dlg dialog

class nt_image_layer_config_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(nt_image_layer_config_dlg)

public:
	explicit nt_image_layer_config_dlg(NT_SmartPublisherSDKAPI* pb_sdk_api, CWnd* pParent = NULL);   // standard constructor
	virtual ~nt_image_layer_config_dlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMAGE_LAYER_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	std::string FileNameUtf8() const { return file_name_utf8_; }
	int Left() const { return left_; }
	int Top() const { return top_; }
	int Width() const { return width_; }
	int Height() const { return height_; }

private:
	NT_SmartPublisherSDKAPI* pb_sdk_api_;
	std::string file_name_utf8_;
	int left_;
	int top_;
	int width_;
	int height_;
	
private:
	CEdit edit_display_file_name_;
	CEdit edit_image_left_;
	CEdit edit_image_top_;
	CEdit edit_image_width_;
	CEdit edit_image_height_;

private:
	int GetIntValueFromEdit(CEdit* pEdit);

public:
	afx_msg void OnBnClickedButtonChooseImageFile();
	
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
