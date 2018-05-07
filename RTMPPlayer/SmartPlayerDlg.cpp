/*
* Author:YIENIJAN
* Copyright (C) 2015-2018 YIENIJAN. All rights reserved.
*
*/
// SmartPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SmartPlayer.h"
#include "SmartPlayerDlg.h"
#include "afxdialogex.h"
#include <locale>
#include <codecvt>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_USER_SDK_VIDEO_SIZE			 (WM_USER + 200)
#define WM_USER_SDK_START_PLAY			 (WM_USER + 201)
#define WM_USER_SDK_RGB32_IMAGE			 (WM_USER + 202)
#define WM_USER_SDK_SP_EVENT			 (WM_USER + 203)			 


extern "C" NT_VOID NT_CALLBACK NT_SP_SDKEventHandle(NT_HANDLE handle, NT_PVOID user_data,
	NT_UINT32 event_id,
	NT_INT64  param1,
	NT_INT64  param2,
	NT_UINT64 param3,
	NT_PCSTR  param4,
	NT_PCSTR  param5,
	NT_PVOID  param6) {
	if (user_data == NULL)
		return;

	HWND hwnd = (HWND)user_data;
	if (::IsWindow(hwnd)) {
		::PostMessage(hwnd, WM_USER_SDK_SP_EVENT, event_id, (LPARAM)param1);
	}
}


extern "C" NT_VOID NT_CALLBACK SP_SDKVideoSizeHandle(NT_HANDLE handle, NT_PVOID userData,
	NT_INT32 width, NT_INT32 height) {
	if (userData == NULL)
		return;

	HWND hwnd = (HWND)userData;
	if (::IsWindow(hwnd)) {
		::PostMessage(hwnd, WM_USER_SDK_VIDEO_SIZE, width, height);
	}
}


extern "C" NT_VOID NT_CALLBACK SM_SDKStartPlayHandle(NT_HANDLE handle, NT_PVOID userData, NT_UINT32 result) {
	if (userData == NULL)
		return;

	HWND hwnd = (HWND)userData;
	if (::IsWindow(hwnd)) {
		::PostMessage(hwnd, WM_USER_SDK_START_PLAY, result, NULL);
	}
}


extern "C" NT_VOID NT_CALLBACK SM_SDKVideoFrameHandle(NT_HANDLE handle, NT_PVOID userData, NT_UINT32 status,
	const NT_SP_VideoFrame* frame) {
	/*if (frame != NULL)
	{
	std::ostringstream ss;
	ss << "Receive frame time_stamp:" << frame->timestamp_ << "ms" << "\r\n";
	OutputDebugStringA(ss.str().c_str());
	}*/

	if (frame != NULL) {
		if (NT_SP_E_VIDEO_FRAME_FORMAT_RGB32 == frame->format_
			&& frame->plane0_ != NULL
			&& frame->stride0_ > 0
			&& frame->height_ > 0) {
			std::unique_ptr<nt_rgb32_image > pImage(new nt_rgb32_image());

			pImage->size_ = frame->stride0_* frame->height_;
			pImage->data_ = new NT_BYTE[pImage->size_];

			memcpy(pImage->data_, frame->plane0_, pImage->size_);

			pImage->width_ = frame->width_;
			pImage->height_ = frame->height_;
			pImage->stride_ = frame->stride0_;

			HWND hwnd = (HWND)userData;
			if (hwnd != NULL && ::IsWindow(hwnd)) {
				::PostMessage(hwnd, WM_USER_SDK_RGB32_IMAGE, (WPARAM)handle, (LPARAM)pImage.release());
			}
		}
	}
}


extern "C" NT_VOID NT_CALLBACK SP_SDKRenderVideoFrameTimestampHandle(NT_HANDLE handle, NT_PVOID userData,
	NT_UINT64 timestamp,
	NT_UINT64 reserve1, NT_PVOID reserve2) {
	/*SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);

	char buffer[256];
	sprintf_s(buffer, "%02d:%02d:%02d.%03d receive render frame time_stamp:%llu\r\n",
	sysTime.wHour, sysTime.wMinute,
	sysTime.wSecond, sysTime.wMilliseconds,
	timestamp);

	OutputDebugStringA(buffer);*/
}


extern "C" NT_VOID NT_CALLBACK NT_SP_SDKAudioPCMFrameHandle(NT_HANDLE handle,
	NT_PVOID user_data, NT_UINT32 status,
	NT_BYTE* data, NT_UINT32 size,
	NT_INT32 sample_rate, NT_INT32 channel, NT_INT32 per_channel_sample_number) {
	std::ostringstream ss;

	ss << "Receive audio pcm frame, status=" << status << " data:" << ((void*)data)
		<< " size=" << size << " sample_rate=" << sample_rate
		<< " channel=" << channel << " per_channel_sample_number=" << per_channel_sample_number
		<< "\r\n";

	OutputDebugStringA(ss.str().c_str());
}


extern "C" NT_VOID NT_CALLBACK SM_SDKCaptureImageHandle(NT_HANDLE handle, NT_PVOID userData, NT_UINT32 result,
	NT_PCSTR file_name) {
	std::ostringstream ss;
	ss << "SM_SDKCaptureImageHandle result:" << result;

	if (result == NT_ERC_OK) {
		ss << " ok, ";
	} else {
		ss << " failed! ";
	}

	if (file_name != NULL) {
		ss << " file_name:" << file_name;
	}

	ss << "\r\n";

	OutputDebugStringA(ss.str().c_str());
}


extern "C" NT_VOID NT_CALLBACK SP_SDKRecorderHandle(NT_HANDLE handle, NT_PVOID userData, NT_UINT32 status,
	NT_PCSTR file_name) {
	std::ostringstream ss;
	ss << "SP_SDKRecorderHandle ";

	if (1 == status) {
		ss << " new file:";
	} else if (2 == status) {
		ss << " finish file:";
	}

	if (file_name != NULL) {
		ss << file_name;
	}

	ss << "\r\n";

	OutputDebugStringA(ss.str().c_str());
}


extern "C" NT_VOID NT_CALLBACK SP_SDKPullStreamVideoDataHandle(NT_HANDLE handle, NT_PVOID userData,
	NT_UINT32 video_codec_id, NT_BYTE* data, NT_UINT32 size,
	NT_SP_PullStreamVideoDataInfo* info,
	NT_PVOID reserve) {

	std::ostringstream oss;

	oss << "pull stream video t=" << info->timestamp_ << "\r\n";

	OutputDebugStringA(oss.str().c_str());
}

extern "C" NT_VOID NT_CALLBACK SP_SDKPullStreamAudioDataHandle(NT_HANDLE handle, NT_PVOID userData,
	NT_UINT32 auido_codec_id, NT_BYTE* data, NT_UINT32 size,
	NT_SP_PullStreamAuidoDataInfo* info,
	NT_PVOID reserve) {

	std::ostringstream oss;

	oss << "pull stream audio t=" << info->timestamp_ << "\r\n";

	OutputDebugStringA(oss.str().c_str());
}

// CSmartPlayerDlg 对话框



CSmartPlayerDlg::CSmartPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSmartPlayerDlg::IDD, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(&player_api_, 0, sizeof(player_api_));

	player_handle_ = NULL;

	width_ = 0;
	width_ = 0;

	is_gdi_render_ = false;
	is_playing_ = false;

	connection_status_ = 0;
	buffer_status_ = 0;
	buffer_percent_ = 0;
	download_speed_ = -1;
}

void CSmartPlayerDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_URL, edit_url);
	DDX_Control(pDX, IDC_BUTTON_PLAY, btn_play_);
	DDX_Control(pDX, IDC_CHECK_MUTE, btn_mute_);
	DDX_Control(pDX, IDC_BUTTON_FULL_SCREEN, btn_full_screen_);
}

BEGIN_MESSAGE_MAP(CSmartPlayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CSmartPlayerDlg::OnBnClickedButtonPlay)
	ON_MESSAGE(WM_USER_SDK_SP_EVENT, &CSmartPlayerDlg::OnSDKEvent)
	ON_MESSAGE(WM_USER_SDK_VIDEO_SIZE, &CSmartPlayerDlg::OnSDKVideoSize)
	ON_MESSAGE(WM_USER_SDK_START_PLAY, &CSmartPlayerDlg::OnSDKStartPlay)
	ON_MESSAGE(WM_USER_SDK_RGB32_IMAGE, &CSmartPlayerDlg::OnSDKRGB32Image)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_MUTE, &CSmartPlayerDlg::OnBnClickedCheckMute)
	ON_BN_CLICKED(IDC_BUTTON_FULL_SCREEN, &CSmartPlayerDlg::OnBnClickedButtonFullScreen)
END_MESSAGE_MAP()


// CSmartPlayerDlg 消息处理程序

BOOL CSmartPlayerDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();


	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	GetWindowText(base_title_);

	// TODO:  在此添加额外的初始化代码
	edit_url.SetWindowTextW(__wargv[1]);
	edit_url.EnableWindow(false);

	auto wc = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW));

	CRect rc_edit_url;
	edit_url.GetWindowRect(rc_edit_url);
	ScreenToClient(rc_edit_url);

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rc_render(20, rc_edit_url.bottom + 10, rcClient.right - 20, rcClient.bottom - 20);
	if (rc_render.IsRectEmpty() || rc_render.IsRectNull()) {
		rc_render.SetRect(0, 0, 4, 4);
	}

	auto wnd_ret = wrapper_render_wnd_.Create(wc, _T("wrapper-smartrender"),
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rc_render, this, 220);

	btn_play_.EnableWindow(FALSE);
	btn_full_screen_.EnableWindow(FALSE);

	GetSmartPlayerSDKAPI(&player_api_);

	if (NT_ERC_OK != player_api_.Init(0, NULL)) {
		return FALSE;
	}

	if (NT_ERC_OK != player_api_.Open(&player_handle_, NULL, 0, NULL)) {
		return FALSE;
	}

	player_api_.SetEventCallBack(player_handle_, GetSafeHwnd(), &NT_SP_SDKEventHandle);

	btn_play_.EnableWindow(TRUE);

	wrapper_render_wnd_.SetPlayerSDKAPI(&player_api_);

	// 800ms拉1次就可以了
	SetTimer(IDT_TIMER_SP_SDK_POS_DRIVER, 800, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSmartPlayerDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSmartPlayerDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

void CSmartPlayerDlg::OnBnClickedButtonPlay() {
	if (player_handle_ == NULL)
		return;

	CString btn_play_str;

	btn_play_.GetWindowTextW(btn_play_str);

	if (btn_play_str == _T("播放")) {
		if (!InitCommonSDKParam()) {
			AfxMessageBox(_T("设置参数错误!"));
			return;
		}

		player_api_.SetVideoSizeCallBack(player_handle_, GetSafeHwnd(), SP_SDKVideoSizeHandle);

		bool is_support_d3d_render = false;
		NT_INT32 in_support_d3d_render = 0;

		if (NT_ERC_OK == player_api_.IsSupportD3DRender(player_handle_,
			wrapper_render_wnd_.RenderWnd(), &in_support_d3d_render)) {
			if (1 == in_support_d3d_render) {
				is_support_d3d_render = true;
			}
		}

		if (is_support_d3d_render) {
			is_gdi_render_ = false;
			// 支持d3d绘制的话，就用D3D绘制
			player_api_.SetRenderWindow(player_handle_, wrapper_render_wnd_.RenderWnd());
		} else {
			is_gdi_render_ = true;
			// 不支持D3D就让播放器吐出数据来，用GDI绘制
			player_api_.SetVideoFrameCallBack(player_handle_, NT_SP_E_VIDEO_FRAME_FORMAT_RGB32,
				GetSafeHwnd(), SM_SDKVideoFrameHandle);
		}

		player_api_.SetLowLatencyMode(player_handle_, 1);


		if (NT_ERC_OK != player_api_.StartPlay(player_handle_)) {
			AfxMessageBox(_T("播放器失败!"));
			return;
		}

		btn_play_.SetWindowTextW(_T("停止"));
		is_playing_ = true;
	} else {
		StopPlayback();
	}
}


void CSmartPlayerDlg::StopPlayback() {
	if (player_handle_ == NULL)
		return;

	is_gdi_render_ = false;

	btn_full_screen_.EnableWindow(FALSE);
	wrapper_render_wnd_.ClearVideoSize();
	wrapper_render_wnd_.SetPlayerHandle(NULL);

	width_ = 0;
	height_ = 0;


	player_api_.StopPlay(player_handle_);

	wrapper_render_wnd_.CleanRender();

	btn_play_.SetWindowTextW(_T("播放"));
	is_playing_ = false;

	SetWindowText(base_title_);
}

void CSmartPlayerDlg::OnSize(UINT nType, int cx, int cy) {
	CDialogEx::OnSize(nType, cx, cy);

	if (cx < 200 || cy < 200)
		return;

	if (::IsWindow(wrapper_render_wnd_.m_hWnd)
		&& !wrapper_render_wnd_.IsFullScreen()) {
		if (width_ == 0 || height_ == 0) {
			CRect rc_edit_url;
			edit_url.GetWindowRect(rc_edit_url);
			ScreenToClient(rc_edit_url);

			CRect rect(10, rc_edit_url.bottom + 10, cx - 50, cy - 20);
			if (rect.IsRectEmpty() || rect.IsRectNull()) {
				rect.SetRect(0, 0, 4, 4);
			}

			wrapper_render_wnd_.MoveWindow(&rect, TRUE);
		} else {
			CRect rc_edit_url;
			edit_url.GetWindowRect(rc_edit_url);

			ScreenToClient(rc_edit_url);

			if ((rc_edit_url.bottom + 10 + 5 + 20) > cy)
				return;

			int dw = 1;
			int dh = 1;

			wrapper_render_wnd_.CalScaleSize(cx - 60, cy - rc_edit_url.bottom - 20 - 10 - 5,
				width_, height_, dw, dh);

			CRect rect(10, rc_edit_url.bottom + 10, 10 + dw, rc_edit_url.bottom + 10 + dh);
			wrapper_render_wnd_.MoveWindow(&rect, TRUE);
		}
	}
}

void CSmartPlayerDlg::OnDestroy() {
	CDialogEx::OnDestroy();

	KillTimer(IDT_TIMER_SP_SDK_POS_DRIVER);

	if (player_handle_ != NULL) {
		if (is_playing_) {
			wrapper_render_wnd_.ClearVideoSize();
			wrapper_render_wnd_.SetPlayerHandle(NULL);

			player_api_.StopPlay(player_handle_);

			is_playing_ = false;
		}

	}

	if (player_handle_ != NULL) {
		player_api_.Close(player_handle_);
	}

	player_api_.UnInit();
}

LRESULT CSmartPlayerDlg::OnSDKEvent(WPARAM wParam, LPARAM lParam) {
	if (!is_playing_) {
		return S_OK;
	}

	NT_UINT32 event_id = (NT_UINT32)(wParam);


	if (NT_SP_E_EVENT_ID_CONNECTING == event_id
		|| NT_SP_E_EVENT_ID_CONNECTION_FAILED == event_id
		|| NT_SP_E_EVENT_ID_CONNECTED == event_id
		|| NT_SP_E_EVENT_ID_DISCONNECTED == event_id
		|| NT_SP_E_EVENT_ID_NO_MEDIADATA_RECEIVED == event_id) {
		if (NT_SP_E_EVENT_ID_CONNECTING == event_id) {
			OutputDebugStringA("connection status: connecting\r\n");
		} else if (NT_SP_E_EVENT_ID_CONNECTION_FAILED == event_id) {
			OutputDebugStringA("connection status: connection failed\r\n");
		} else if (NT_SP_E_EVENT_ID_CONNECTED == event_id) {
			OutputDebugStringA("connection status: connected\r\n");
		} else if (NT_SP_E_EVENT_ID_DISCONNECTED == event_id) {
			OutputDebugStringA("connection status: disconnected\r\n");
		} else if (NT_SP_E_EVENT_ID_NO_MEDIADATA_RECEIVED == event_id) {
			OutputDebugStringA("connection status: no mediadata received\r\n");
		}

		connection_status_ = event_id;
	}

	if (NT_SP_E_EVENT_ID_START_BUFFERING == event_id
		|| NT_SP_E_EVENT_ID_BUFFERING == event_id
		|| NT_SP_E_EVENT_ID_STOP_BUFFERING == event_id) {
		buffer_status_ = event_id;

		if (NT_SP_E_EVENT_ID_BUFFERING == event_id) {
			buffer_percent_ = (NT_INT32)lParam;

			std::wostringstream ss;
			ss << L"buffering:" << buffer_percent_ << "%";
			OutputDebugStringW(ss.str().c_str());
			OutputDebugStringW(L"\r\n");
		}
	}

	if (NT_SP_E_EVENT_ID_DOWNLOAD_SPEED == event_id) {
		download_speed_ = (NT_INT32)lParam;
	}

	CString show_str = base_title_;

	if (connection_status_ != 0) {
		show_str += _T("--链接状态: ");

		if (NT_SP_E_EVENT_ID_CONNECTING == connection_status_) {
			show_str += _T("链接中");
		} else if (NT_SP_E_EVENT_ID_CONNECTION_FAILED == connection_status_) {
			show_str += _T("链接失败");
		} else if (NT_SP_E_EVENT_ID_CONNECTED == connection_status_) {
			show_str += _T("链接成功");
		} else if (NT_SP_E_EVENT_ID_DISCONNECTED == connection_status_) {
			show_str += _T("链接断开");
		} else if (NT_SP_E_EVENT_ID_NO_MEDIADATA_RECEIVED == connection_status_) {
			show_str += _T("收不到数据");
		}
	}

	if (download_speed_ != -1) {
		std::wostringstream ss;
		ss << L"--下载速度:" << (download_speed_ * 8 / 1000) << "kbps"
			<< L"(" << (download_speed_ / 1024) << "KB/s)";

		show_str += ss.str().c_str();
	}

	if (buffer_status_ != 0) {
		show_str += _T("--缓冲状态: ");

		if (NT_SP_E_EVENT_ID_START_BUFFERING == buffer_status_) {
			show_str += _T("开始缓冲");
		} else if (NT_SP_E_EVENT_ID_BUFFERING == buffer_status_) {
			std::wostringstream ss;
			ss << L"缓冲中" << buffer_percent_ << "%";
			show_str += ss.str().c_str();
		} else if (NT_SP_E_EVENT_ID_STOP_BUFFERING == buffer_status_) {
			show_str += _T("结束缓冲");
		}
	}


	SetWindowText(show_str);

	return S_OK;
}

LRESULT CSmartPlayerDlg::OnSDKVideoSize(WPARAM wParam, LPARAM lParam) {
	width_ = (int)wParam;
	height_ = (int)lParam;

	std::wstringstream wss;
	wss << width_ << L"*" << height_;

	if (width_ == 0 || height_ == 0)
		return S_OK;

	if (player_handle_ == NULL)
		return S_OK;;

	wrapper_render_wnd_.SetPlayerHandle(player_handle_);

	wrapper_render_wnd_.SetVideoSize(width_, height_);

	btn_full_screen_.EnableWindow(TRUE);

	if (!wrapper_render_wnd_.IsFullScreen()
		&& ::IsWindow(wrapper_render_wnd_.m_hWnd)) {
		CRect rect;
		GetClientRect(&rect);

		if (rect.Width() < 200 || rect.Height() < 200)
			return S_OK;

		CRect rc_edit_url;
		edit_url.GetWindowRect(rc_edit_url);

		ScreenToClient(rc_edit_url);

		if ((rc_edit_url.bottom + 10 + 5 + 20) > rect.Height())
			return S_OK;

		int dw = 1;
		int dh = 1;

		wrapper_render_wnd_.CalScaleSize(rect.Width() - 60, rect.Height() - rc_edit_url.bottom - 20 - 10 - 5,
			width_, height_, dw, dh);

		CRect drect(10, rc_edit_url.bottom + 10, 10 + dw, rc_edit_url.bottom + 10 + dh);
		wrapper_render_wnd_.MoveWindow(&drect, TRUE);
	}

	return S_OK;
}

LRESULT CSmartPlayerDlg::OnSDKStartPlay(WPARAM wParam, LPARAM lParam) {
	NT_UINT32 result = (NT_UINT32)wParam;

	if (NT_ERC_OK == result) {
		btn_play_.SetWindowTextW(_T("停止"));
		is_playing_ = true;
	} else {
		btn_play_.SetWindowTextW(_T("播放"));
		is_playing_ = false;
	}

	return S_OK;
}

LRESULT CSmartPlayerDlg::OnSDKRGB32Image(WPARAM wParam, LPARAM lParam) {
	nt_rgb32_image* pImage = (nt_rgb32_image*)(lParam);
	if (pImage == NULL)
		return S_OK;

	std::shared_ptr<nt_rgb32_image> sp_image(pImage);

	if (is_gdi_render_) {
		wrapper_render_wnd_.OnRGB32Image(sp_image);
	}

	return S_OK;
}

bool CSmartPlayerDlg::InitCommonSDKParam() {
	ASSERT(!is_playing_);

	if (NULL == player_handle_)
		return false;

	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;

	player_api_.SetBuffer(player_handle_, 100);

	player_api_.SetFastStartup(player_handle_, 1);

	player_api_.SetReportDownloadSpeed(player_handle_, 1, 1);

	CString wURL;
	edit_url.GetWindowTextW(wURL);

	auto url = conv.to_bytes(wURL);

	if (NT_ERC_OK != player_api_.SetURL(player_handle_, url.c_str())) {
		return false;
	}

	connection_status_ = 0;
	buffer_status_ = 0;
	buffer_percent_ = 0;
	download_speed_ = -1;

	return true;
}

void CSmartPlayerDlg::OnBnClickedCheckMute() {
	if (player_handle_ == NULL) {
		return;
	}

	if (btn_mute_.GetCheck() == BST_CHECKED) {
		player_api_.SetMute(player_handle_, 1);
	} else {
		player_api_.SetMute(player_handle_, 0);
	}
}

void CSmartPlayerDlg::OnBnClickedButtonFullScreen() {
	wrapper_render_wnd_.FullScreenSwitch();
}