/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

#include "stdafx.h"
#include "SmartPublisherDemo.h"
#include "nt_image_layer_config_dlg.h"
#include "afxdialogex.h"
#include "nt_image_wrapper.h"

#include <locale>
#include <codecvt>
#include <memory>
#include <sstream>

// nt_image_layer_config_dlg dialog

IMPLEMENT_DYNAMIC(nt_image_layer_config_dlg, CDialogEx)

nt_image_layer_config_dlg::nt_image_layer_config_dlg(NT_SmartPublisherSDKAPI* pb_sdk_api, CWnd* pParent /*=NULL*/)
	: CDialogEx(nt_image_layer_config_dlg::IDD, pParent)
{
	pb_sdk_api_ = pb_sdk_api;
	ASSERT(pb_sdk_api_ != nullptr);

	left_	= 0;
	top_	= 0;
	width_	= 0;
	height_ = 0;
}

nt_image_layer_config_dlg::~nt_image_layer_config_dlg()
{
}

void nt_image_layer_config_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DISPLAY_IAMGE_FILE_NAME, edit_display_file_name_);
	DDX_Control(pDX, IDC_EDIT_IMAGE_LEFT, edit_image_left_);
	DDX_Control(pDX, IDC_EDIT_IMAGE_TOP, edit_image_top_);
	DDX_Control(pDX, IDC_EDIT_IMAGE_WIDTH, edit_image_width_);
	DDX_Control(pDX, IDC_EDIT_IMAGE_HEIGHT, edit_image_height_);
}


BEGIN_MESSAGE_MAP(nt_image_layer_config_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_IMAGE_FILE, &nt_image_layer_config_dlg::OnBnClickedButtonChooseImageFile)
END_MESSAGE_MAP()


// nt_image_layer_config_dlg message handlers


void nt_image_layer_config_dlg::OnBnClickedButtonChooseImageFile()
{
	ASSERT(pb_sdk_api_ != nullptr);

	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST,
		(LPCTSTR)_T("PNG Files (*.png)|*.png||"),
		NULL);

	if ( IDOK == dlg.DoModal() )
	{
		auto file_name = dlg.GetPathName();

		std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;

		auto file_name_utf8 = conv.to_bytes(file_name);

		auto sp_image = std::make_shared<nt_pb_sdk::image_wrapper>(pb_sdk_api_->ImageAPI_);

		auto ret = pb_sdk_api_->ImageAPI_.LoadImage(file_name_utf8.c_str(), sp_image->getPP());

		if ( NT_ERC_OK != ret)
		{
			AfxMessageBox(_T("打开文件失败,请确保是png文件!"));
			return;
		}

		file_name_utf8_ = file_name_utf8;
		edit_display_file_name_.SetWindowTextW(file_name);

		std::wostringstream wss;
		wss << sp_image->Width();

		edit_image_width_.SetWindowTextW(wss.str().c_str());

		wss.str(std::wstring());
		wss << sp_image->Height();

		edit_image_height_.SetWindowTextW(wss.str().c_str());
	}
}

BOOL nt_image_layer_config_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	edit_image_left_.SetWindowText(_T("0"));
	edit_image_top_.SetWindowText(_T("0"));
	edit_image_width_.SetWindowText(_T("0"));
	edit_image_height_.SetWindowText(_T("0"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void nt_image_layer_config_dlg::OnOK()
{
	left_	= GetIntValueFromEdit(&edit_image_left_);
	top_	= GetIntValueFromEdit(&edit_image_top_);
	width_  = GetIntValueFromEdit(&edit_image_width_);
	height_ = GetIntValueFromEdit(&edit_image_height_);
	
	CDialogEx::OnOK();
}

int nt_image_layer_config_dlg::GetIntValueFromEdit(CEdit* pEdit)
{
	if ( pEdit == NULL )
		return 0;

	if ( !::IsWindow(pEdit->m_hWnd) )
		return 0;

	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;

	CString w_val_str;
	pEdit->GetWindowTextW(w_val_str);

	auto val_str = conv.to_bytes(w_val_str);

	return atoi(val_str.c_str());
}
