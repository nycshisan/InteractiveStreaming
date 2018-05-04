/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/


// SmartPublisherDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartPublisherDemo.h"
#include "SmartPublisherDemoDlg.h"
#include "afxdialogex.h"

#include <locale>
#include <codecvt>
#include <string>
#include <sstream>
#include <VersionHelpers.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	wchar_t w_def_publish_url[] = L"rtmp://player.daniulive.com:1935/hls/stream";
	char def_publish_url[]		= "rtmp://player.daniulive.com:1935/hls/stream";

	//wchar_t w_def_publish_url[] = L"rtmp://localhost:1935/live/stream";
	
	//wchar_t w_def_publish_url[] = L"rtmp://localhost:1935/live/streama";
	//char def_publish_url[]		= "rtmp://localhost:1935/live/stream";

	wchar_t w_publish_note[] = L"为防止未经许可的第三方利用本软件从事恶意盗录等非法活动，未经授权版本，每次只允许实时推送、录像1小时（1小时后，如需继续试用，请关闭应用程序再重新打开），请在法律许可范围内使用本软件。";
}

namespace
{
	BOOL CALLBACK My_MonitorEnumProcSize(HMONITOR hm, HDC, LPRECT prect, LPARAM user_data)
	{
		if (hm == NULL)
		{
			return TRUE;
		}

		MONITORINFOEX mi;
		mi.cbSize = sizeof(mi);

		if (GetMonitorInfo(hm, &mi))
		{
			if (mi.dwFlags&MONITORINFOF_PRIMARY)
			{
				if (user_data != NULL)
				{
					std::pair<int, int>* my_size = (std::pair<int, int>*)(user_data);
					my_size->first  =  mi.rcMonitor.right - mi.rcMonitor.left;
					my_size->second  = mi.rcMonitor.bottom - mi.rcMonitor.top;

					return FALSE;
				}
			}
		}

		return TRUE;
	}

	void GetScreenSize(int& w, int& h)
	{
		w = 0;
		h = 0;

		std::pair<int, int> my_size(0, 0);

		auto ret = ::EnumDisplayMonitors(NULL, NULL, &My_MonitorEnumProcSize, (LPARAM)&my_size);
		if ( my_size.first == 0 || my_size.second == 0 )
		{
			w = ::GetSystemMetrics(SM_CXSCREEN);
			h = ::GetSystemMetrics(SM_CYSCREEN);

		}
		else
		{
			w = my_size.first;
			h = my_size.second;
		}
	}

	bool IsWin7System()
	{
		if (::IsWindows7OrGreater()
			&& !::IsWindows8OrGreater()
			&& !::IsWindowsServer()
			)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

extern "C" NT_VOID NT_CALLBACK NT_PB_SDKScreenRegionChooseHandle(NT_HANDLE tool_handle, NT_PVOID userData,
	NT_UINT32 msg_id, NT_PVOID msg_data)
{
	if ( NT_PB_E_SCREEN_REGION_CHOOSE_MSG_CANCEL == msg_id )
	{
		OutputDebugString(L"ScreenRegionChooseHandle cancel msg\r\n");

		HWND hwnd = (HWND)userData;
		if ( ::IsWindow(hwnd) )
		{
			::PostMessage(hwnd, WM_USER_PB_SDK_SCREEN_REGION_CHOOSE_HANDLE, (WPARAM)msg_id, NULL);
		}
	}
	else if ( NT_PB_E_SCREEN_REGION_CHOOSE_MSG_OK == msg_id )
	{
		NT_PB_RectRegion* pSendRegion = NULL;

		NT_PB_RectRegion* rect_region = (NT_PB_RectRegion*)(msg_data);
		if (rect_region != nullptr)
		{
			std::wstringstream ss;
			ss << L"ScreenRegionChooseHandle ok msg, x:" << rect_region->x_
				<< " y:" << rect_region->y_ << " w:" << rect_region->width_
				<< " h:" << rect_region->height_ << "\r\n";

			OutputDebugString(ss.str().c_str());

			pSendRegion = new NT_PB_RectRegion();
			*pSendRegion = *rect_region;
		}

		HWND hwnd = (HWND)userData;
		if ( ::IsWindow(hwnd) )
		{
			::PostMessage(hwnd, WM_USER_PB_SDK_SCREEN_REGION_CHOOSE_HANDLE, (WPARAM)msg_id, (LPARAM)pSendRegion);
		}
		else
		{
			if (pSendRegion != NULL)
			{
				delete pSendRegion;
			}
		}
	}
	else if ( NT_PB_E_SCREEN_REGION_CHOOSE_MSG_MOVE == msg_id )
	{
		NT_PB_RectRegion* pSendRegion = NULL;

		NT_PB_RectRegion* rect_region = (NT_PB_RectRegion*)(msg_data);
		if ( rect_region != nullptr )
		{
			/*std::wstringstream ss;
			ss << L"ScreenRegionChooseHandle Move msg, x:" << rect_region->x_
			<< " y:" << rect_region->y_ << " w:" << rect_region->width_
			<< " h:" << rect_region->height_ << "\r\n";

			OutputDebugString(ss.str().c_str());*/

			pSendRegion = new NT_PB_RectRegion();
			*pSendRegion = *rect_region;
		}

		HWND hwnd = (HWND)userData;
		if ( ::IsWindow(hwnd) )
		{
			::PostMessage(hwnd, WM_USER_PB_SDK_SCREEN_REGION_CHOOSE_HANDLE, (WPARAM)msg_id, (LPARAM)pSendRegion);
		}
		else
		{
			if (pSendRegion != NULL)
			{
				delete pSendRegion;
			}
		}
	}
}

extern "C" NT_VOID NT_CALLBACK NT_PB_SDKCaptureImageHandle(NT_HANDLE handle, NT_PVOID userData, NT_UINT32 result,
	NT_PCSTR file_name)
{
	std::ostringstream ss;
	ss << "NT_PB_SDKCaptureImageHandle result:" << result;

	if (result == NT_ERC_OK)
	{
		ss << " ok, ";
	}
	else
	{
		ss << " failed! ";
	}

	if (file_name != NULL)
	{
		ss << " file_name:" << file_name;
	}

	ss << "\r\n";

	OutputDebugStringA(ss.str().c_str());
}


extern "C" NT_VOID NT_CALLBACK NT_PB_SDKVideoPacketTimestampHandle(NT_HANDLE handle, NT_PVOID userData,
	NT_PCSTR url, NT_INT32 is_changed, NT_UINT64 timestamp,
	NT_UINT64 reserve1, NT_PVOID reserve2)
{

	/*
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);

	char buffer[256];
	sprintf_s(buffer, "%02d:%02d:%02d.%03d video packet time_stamp:%llu, is_changed=%d\r\n",
	sysTime.wHour, sysTime.wMinute,
	sysTime.wSecond, sysTime.wMilliseconds,
	timestamp,
	is_changed);

	OutputDebugStringA(buffer);
	*/
}


extern "C" NT_VOID NT_CALLBACK NT_PB_SDKPublisherStatusHandle(NT_HANDLE handle, NT_PVOID user_data,
	NT_PCSTR url, NT_UINT32 status)
{

	std::ostringstream ss;

	ss << "Publisher status:";

	if (NT_PB_E_PUBLISHER_STATUS_CONNECTING == status)
	{
		ss << " connecting";
	}
	else if (NT_PB_E_PUBLISHER_STATUS_CONNECTION_FAILED == status)
	{
		ss << " connection failed";
	}
	else if (NT_PB_E_PUBLISHER_STATUS_CONNECTED == status)
	{
		ss << " connected";
	}
	else if (NT_PB_E_PUBLISHER_STATUS_DISCONNECTED == status)
	{
		ss << " disconnected";
	}

	if (url != NULL)
	{
		ss << " url:" << url;
	}

	ss << "\r\n";

	OutputDebugStringA(ss.str().c_str());
}


extern "C" NT_VOID NT_CALLBACK NT_PB_SDKPublisherEventHandle(NT_HANDLE handle, NT_PVOID user_data,
	NT_UINT32 event_id,
	NT_INT64  param1,
	NT_INT64  param2,
	NT_UINT64 param3,
	NT_UINT64 param4,
	NT_PCSTR  param5,
	NT_PCSTR  param6,
	NT_PVOID  param7
	)
{
	std::ostringstream ss;

	ss << "PublisherSDK onEvent handle:" << handle << " ";

	switch (event_id)
	{
	case NT_PB_E_EVENT_ID_CONNECTING:
		ss << " push connecting";
		if (param5 != NULL)
		{
			ss << " url:" << param5;
		}
		break;

	case NT_PB_E_EVENT_ID_CONNECTION_FAILED:
		ss << " push connection failed";
		if ( param5 != NULL )
		{
			ss << " url:" << param5;
		}
		break;

	case NT_PB_E_EVENT_ID_CONNECTED:
		ss << " push connected";
		if ( param5 != NULL )
		{
			ss << " url:" << param5;
		}
		break;

	case NT_PB_E_EVENT_ID_DISCONNECTED:
		ss << " push disconnected";
		if ( param5 != NULL )
		{
			ss << " url:" << param5;
		}
		break;

	case NT_PB_E_EVENT_ID_RECORDER_START_NEW_FILE:
		ss << " start new recorder file";
		if (param5 != NULL)
		{
			ss << " file name:" << param5;
		}
		break;

	case NT_PB_E_EVENT_ID_ONE_RECORDER_FILE_FINISHED:
		ss << " finish recorder file";
		if (param5 != NULL)
		{
			ss << " file name:" << param5;
		}
		break;

	case NT_PB_E_EVENT_ID_CAPTURE_WINDOW_INVALID:
		{
			ss << " capture window invalid";
			ss << " hwnd=" << reinterpret_cast<HWND>(param1);

			HWND hwnd = reinterpret_cast<HWND>(user_data);
			if (hwnd != NULL && ::IsWindow(hwnd))
			{
				::PostMessage(hwnd, WM_USER_PB_SDK_CAPTURE_WINDOW_INVALID, (WPARAM)(handle), (LPARAM)(param1));
			}
		}

		break;

	default:
		break;
	}

	ss << "\r\n";

	OutputDebugStringA(ss.str().c_str());
}


// CSmartPublisherDemoDlg dialog

CSmartPublisherDemoDlg::CSmartPublisherDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSmartPublisherDemoDlg::IDD, pParent)
{	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	cur_sel_camera_index_ = CB_ERR;
	cur_sel_camera_resolutions_index_ = CB_ERR;
	cur_sel_camera_frame_rate_index_ = CB_ERR;

	cur_sel_capture_window_ = nullptr;

	image_layer_left_	= 0;
	image_layer_top_	= 0;
	image_layer_width_	= 0;
	image_layer_height_ = 0;

	memset(&publisher_api_, 0, sizeof(publisher_api_));

	publisher_handle_ = NULL;

	publisher_handle_count_ = 0;

	region_choose_tool_handle_ = NULL;
}

void CSmartPublisherDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1_PB_QQ1, edit_pb_qq1);
	DDX_Control(pDX, IDC_EDIT2_PB_QQ2, edit_pb_qq2_);
	DDX_Control(pDX, IDC_EDIT_FRAME_RATE, edit_frame_rate_);
	DDX_Control(pDX, IDC_EDIT_VIDEO_BITRATE, edit_bit_rate_);
	DDX_Control(pDX, IDC_EDIT_KEYFRAME, edit_key_frame_);
	DDX_Control(pDX, IDC_EDIT_PUBLISH_URL, edit_publish_url1_);
	DDX_Control(pDX, IDC_BUTTON_PUBLISH, btn_publish_);
	DDX_Control(pDX, IDC_BUTTON_STOP, btn_stop_publish_);
	DDX_Control(pDX, IDC_EDIT_GEN_URL, edit_gen_url_);
	DDX_Control(pDX, IDC_EDIT_CLIP_LEFT, edit_clip_left_);
	DDX_Control(pDX, IDC_EDIT_CLIP_TOP, edit_clip_top_);
	DDX_Control(pDX, IDC_EDIT_CLIP_WIDTH, edit_clip_width_);
	DDX_Control(pDX, IDC_EDIT_CLIP_HEIGHT, edit_clip_height_);
	DDX_Control(pDX, IDC_CHECK_VIDEO_BITRATE, btn_check_video_bitrate_);
	DDX_Control(pDX, IDC_EDIT_VIDEO_QUALITY, edit_video_quality_);
	DDX_Control(pDX, IDC_EDIT_VIDEO_MAX_BITRATE, edit_video_max_bitrate_);
	DDX_Control(pDX, IDC_CHECK_AUDIO_INPUT, btn_check_auido_mic_input_);
	DDX_Control(pDX, IDC_COMBO_AUDIO_INPUT_DEVICES, combox_auido_input_devices_);
	DDX_Control(pDX, IDC_CHECK_SET_MUTE, btn_check_set_mute_);
	DDX_Control(pDX, IDC_CHECK_AUIDO_SPEAKER_INPUT, btn_check_auido_speaker_input_);
	DDX_Control(pDX, IDC_EDIT_NOTE, edit_note_);
	DDX_Control(pDX, IDC_EDIT_REC_DIR, edit_rec_dir_);
	DDX_Control(pDX, IDC_EDIT_REC_FILE_NAME_FREFIX, edit_file_name_prefix_);
	DDX_Control(pDX, IDC_CHECK_REC_APPEND_DATE, btn_check_rec_append_date_);
	DDX_Control(pDX, IDC_CHECK_REC_APPEND_TIME, btn_check_append_time_);
	DDX_Control(pDX, IDC_BUTTON_REC, btn_rec_);
	DDX_Control(pDX, IDC_BUTTON_STOP_REC, btn_stop_rec_);
	DDX_Control(pDX, IDC_EDIT_REC_MAX_FILE_SIZE, edit_rec_max_file_size_);
	DDX_Control(pDX, IDC_CHECK_DESKTOP_INPUT, btn_check_desktop_input_);
	DDX_Control(pDX, IDC_CHECK_camera_INPUT, btn_check_camera_input_);
	DDX_Control(pDX, IDC_COMBO_CAMERAS, combox_camera_list_);
	DDX_Control(pDX, IDC_COMBO_CAMERA_RESOLUTIONS, combox_camera_resolutions_);
	DDX_Control(pDX, IDC_COMBO_CAMERA_FRAME_RATE, combox_camera_frame_rate_);
	DDX_Control(pDX, IDC_CHECK_FLIP_VERTICAL_CAMERA, btn_check_flip_vertical_camera_);
	DDX_Control(pDX, IDC_CHECK_FLIP_HORIZONTALCAMERA, btn_check_flip_horizontal_camera_);
	DDX_Control(pDX, IDC_BUTTON_ROTATE_CAMERA, btn_rotate_camera_);
	DDX_Control(pDX, IDC_BUTTON_Camera_preview, btn_camera_preview_);
	DDX_Control(pDX, IDC_BUTTON_CHOOSE_SCREEN_REGION, btn_choose_screen_region_);
	DDX_Control(pDX, IDC_CHECK_DXGI_SCREEN_CAPTURER, btn_check_dxgi_screen_capturer_);
	DDX_Control(pDX, IDC_CHECK_CAPTURE_DISABLE_AERO, btn_check_capturer_disable_aero_);
	DDX_Control(pDX, IDC_EDIT_PUBLISH_URL2, edit_publish_url2_);
	DDX_Control(pDX, IDC_EDIT_PUBLISH_URL3, edit_publish_url3_);
	DDX_Control(pDX, IDC_CHECK_DESKTOP_CAMERA_SWITCH, btn_check_desktop_camera_switch_);
	DDX_Control(pDX, IDC_CHECK_CAMERA_OVERLAY_TO_DESKTOP, btn_check_camera_overlay_to_desktop_);
	DDX_Control(pDX, IDC_CHECK_DESKTOP_OVERLAY_TO_CAMERA, btn_check_desktop_overlay_to_camera_);
	DDX_Control(pDX, IDC_BUTTON_DESKTOP_CAMERA_SWITCH, btn_desktop_camera_switch_);
	DDX_Control(pDX, IDC_BUTTON_DISABLE_CAMERA_OVERLAY, btn_disable_camera_overlay_);
	DDX_Control(pDX, IDC_BUTTON_DISABLE_DESKTOP_OVERLAY, btn_disable_desktop_overlay_);
	DDX_Control(pDX, IDC_EDIT_CAMERA_OVERLAY_LEFT, edit_camera_overlay_left_);
	DDX_Control(pDX, IDC_EDIT_CAMERA_OVERLAY_TOP, edit_camera_overlay_top_);
	DDX_Control(pDX, IDC_EDIT_CAMERA_OVERLAY_WIDTH, edit_camera_overlay_width_);
	DDX_Control(pDX, IDC_EDIT_CAMERA_OVERLAY_HEIGHT, edit_camera_overlay_height_);
	DDX_Control(pDX, IDC_EDIT_RGBA_RECT_LAYER_R, edit_rgba_rect_layer_red_);
	DDX_Control(pDX, IDC_EDIT_RGBA_RECT_LAYER_G, edit_rgba_rect_layer_green_);
	DDX_Control(pDX, IDC_EDIT_RGBA_RECT_LAYER_B, edit_rgba_rect_layer_blue_);
	DDX_Control(pDX, IDC_EDIT_RGBA_RECT_LAYER_A, edit_rgba_rect_layer_alpha_);
	DDX_Control(pDX, IDC_BUTTON_DISABLE_IMAGE_WATERMARK, btn_disable_image_watermark_);
	DDX_Control(pDX, IDC_BUTTON_ADD_IMAGE_WATERMARK, btn_add_image_watermark_);
	DDX_Control(pDX, IDC_CHECK_ECHO_CANCEL, btn_check_echo_cancel_);
	DDX_Control(pDX, IDC_EDIT_ECHO_DELAY, edit_echo_delay_);
	DDX_Control(pDX, IDC_CHECK_SET_NoiseSuppression, btn_check_noise_suppression_);
	DDX_Control(pDX, IDC_CHECK_SET_AGC, btn_check_agc_);
	DDX_Control(pDX, IDC_CHECK_SET_SPEEX_ENCODER, btn_check_speex_encoder_);
	DDX_Control(pDX, IDC_EDIT_SPEEX_QUALITY, edit_speex_quality_);
	DDX_Control(pDX, IDC_COMBO_H264_PROFILE, combox_h264_profile_);
	DDX_Control(pDX, IDC_EDIT_VIDEO_ENCODE_SPEED, edit_video_encode_speed_);
	DDX_Control(pDX, IDC_CHECK_SET_VAD, btn_check_vad_);
	DDX_Control(pDX, IDC_CHECK_VIDEO_SAME_IAMGE, btn_check_same_image_);
	DDX_Control(pDX, IDC_EDIT_MAX_VIDEO_FRAME_Interval, edit_max_video_frame_interval_);
	DDX_Control(pDX, IDC_EDIT_MAX_KEY_FRAME_INTERVAL, edit_max_video_key_frame_interval_);
	DDX_Control(pDX, IDC_CHECK_WINDOW_INPUT, btn_check_window_input_);
	DDX_Control(pDX, IDC_COMBO_WINDOW_LIST, combox_window_list_);
	DDX_Control(pDX, IDC_EDIT_CAMERA_ROTATE_DEGRESS, edit_camera_rotate_degress_);
}

BEGIN_MESSAGE_MAP(CSmartPublisherDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PUBLISH, &CSmartPublisherDemoDlg::OnBnClickedButtonPublish)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSmartPublisherDemoDlg::OnBnClickedButtonStop)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_VIDEO_BITRATE, &CSmartPublisherDemoDlg::OnBnClickedCheckVideoBitrate)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE_CONTROL_DEF_VALUE, &CSmartPublisherDemoDlg::OnBnClickedButtonBitrateControlDefValue)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE_CONTROL_DEF_VALUE2, &CSmartPublisherDemoDlg::OnBnClickedButtonBitrateControlDefValue2)
	ON_BN_CLICKED(IDC_BUTTON_BITRATE_CONTROL_DEF_VALUE3, &CSmartPublisherDemoDlg::OnBnClickedButtonBitrateControlDefValue3)
	ON_BN_CLICKED(IDC_CHECK_SET_MUTE, &CSmartPublisherDemoDlg::OnBnClickedCheckSetMute)
	ON_BN_CLICKED(IDC_CHECK_AUDIO_INPUT, &CSmartPublisherDemoDlg::OnBnClickedCheckAudioMicInput)
	ON_BN_CLICKED(IDC_CHECK_AUIDO_SPEAKER_INPUT, &CSmartPublisherDemoDlg::OnBnClickedCheckAuidoSpeakerInput)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_REC_DIR, &CSmartPublisherDemoDlg::OnBnClickedButtonChooseRecDir)
	ON_BN_CLICKED(IDC_BUTTON_REC, &CSmartPublisherDemoDlg::OnBnClickedButtonRec)
	ON_BN_CLICKED(IDC_BUTTON_STOP_REC, &CSmartPublisherDemoDlg::OnBnClickedButtonStopRec)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERAS, &CSmartPublisherDemoDlg::OnCbnSelchangeComboCameras)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERA_RESOLUTIONS, &CSmartPublisherDemoDlg::OnCbnSelchangeComboCameraResolutions)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERA_FRAME_RATE, &CSmartPublisherDemoDlg::OnCbnSelchangeComboCameraFrameRate)
	ON_BN_CLICKED(IDC_CHECK_DESKTOP_INPUT, &CSmartPublisherDemoDlg::OnBnClickedCheckDesktopInput)
	ON_BN_CLICKED(IDC_CHECK_camera_INPUT, &CSmartPublisherDemoDlg::OnBnClickedCheckcameraInput)
	ON_BN_CLICKED(IDC_BUTTON_Camera_preview, &CSmartPublisherDemoDlg::OnBnClickedButtonCameraPreview)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_SCREEN_REGION, &CSmartPublisherDemoDlg::OnBnClickedButtonChooseScreenRegion)
	ON_MESSAGE(WM_USER_PB_SDK_SCREEN_REGION_CHOOSE_HANDLE, &CSmartPublisherDemoDlg::OnSDKScreenRegionChooseHandle)
	ON_BN_CLICKED(IDC_CHECK_DESKTOP_CAMERA_SWITCH, &CSmartPublisherDemoDlg::OnBnClickedCheckDesktopCameraSwitch)
	ON_BN_CLICKED(IDC_CHECK_CAMERA_OVERLAY_TO_DESKTOP, &CSmartPublisherDemoDlg::OnBnClickedCheckCameraOverlayToDesktop)
	ON_BN_CLICKED(IDC_CHECK_DESKTOP_OVERLAY_TO_CAMERA, &CSmartPublisherDemoDlg::OnBnClickedCheckDesktopOverlayToCamera)
	ON_BN_CLICKED(IDC_BUTTON_DESKTOP_CAMERA_SWITCH, &CSmartPublisherDemoDlg::OnBnClickedButtonDesktopCameraSwitch)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE_CAMERA_OVERLAY, &CSmartPublisherDemoDlg::OnBnClickedButtonDisableCameraOverlay)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE_DESKTOP_OVERLAY, &CSmartPublisherDemoDlg::OnBnClickedButtonDisableDesktopOverlay)
	ON_BN_CLICKED(IDC_BUTTON_SET_RBGA_RECT_LAYER, &CSmartPublisherDemoDlg::OnBnClickedButtonSetRbgaRectLayer)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CAPTURE_IMAGE_PATH, &CSmartPublisherDemoDlg::OnBnClickedButtonSettingCaptureImagePath)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE_IMAGE, &CSmartPublisherDemoDlg::OnBnClickedButtonCaptureImage)
	ON_BN_CLICKED(IDC_CHECK_VIDEO_SAME_IAMGE, &CSmartPublisherDemoDlg::OnBnClickedCheckVideoSameIamge)
	ON_BN_CLICKED(IDC_CHECK_WINDOW_INPUT, &CSmartPublisherDemoDlg::OnBnClickedCheckWindowInput)
	ON_CBN_DROPDOWN(IDC_COMBO_WINDOW_LIST, &CSmartPublisherDemoDlg::OnCbnDropdownComboWindowList)
	ON_CBN_SELCHANGE(IDC_COMBO_WINDOW_LIST, &CSmartPublisherDemoDlg::OnCbnSelchangeComboWindowList)
	ON_BN_CLICKED(IDC_CHECK_FLIP_VERTICAL_CAMERA, &CSmartPublisherDemoDlg::OnBnClickedCheckFlipVerticalCamera)
	ON_BN_CLICKED(IDC_CHECK_FLIP_HORIZONTALCAMERA, &CSmartPublisherDemoDlg::OnBnClickedCheckFlipHorizontalcamera)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_CAMERA, &CSmartPublisherDemoDlg::OnBnClickedButtonRotateCamera)
	ON_MESSAGE(WM_USER_PB_SDK_CAPTURE_WINDOW_INVALID, &CSmartPublisherDemoDlg::OnSDKCaptureWindowInvalid)
	ON_BN_CLICKED(IDC_MFCLINK1, &CSmartPublisherDemoDlg::OnBnClickedMfclink1)
END_MESSAGE_MAP()


// CSmartPublisherDemoDlg message handlers

BOOL CSmartPublisherDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	edit_note_.SetWindowTextW(w_publish_note);

	edit_pb_qq1.SetWindowTextW(_T("89030985"));
	edit_pb_qq2_.SetWindowTextW(_T("2679481035"));


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	btn_check_desktop_camera_switch_.EnableWindow(FALSE);
	btn_check_camera_overlay_to_desktop_.EnableWindow(FALSE);
	btn_check_desktop_overlay_to_camera_.EnableWindow(FALSE);

	edit_rgba_rect_layer_red_.SetWindowTextW(_T("0"));
	edit_rgba_rect_layer_green_.SetWindowTextW(_T("0"));
	edit_rgba_rect_layer_blue_.SetWindowTextW(_T("0"));
	edit_rgba_rect_layer_alpha_.SetWindowTextW(_T("0"));

	edit_camera_overlay_left_.SetWindowTextW(_T("0"));
	edit_camera_overlay_top_.SetWindowTextW(_T("0"));
	edit_camera_overlay_width_.SetWindowTextW(_T("0"));
	edit_camera_overlay_height_.SetWindowTextW(_T("0"));

	btn_check_desktop_input_.SetCheck(BST_CHECKED);

	edit_clip_left_.SetWindowTextW(_T("0"));
	edit_clip_top_.SetWindowTextW(_T("0"));
	edit_clip_width_.SetWindowTextW(_T("0"));
	edit_clip_height_.SetWindowTextW(_T("0"));

	btn_check_camera_input_.EnableWindow(FALSE);
	combox_camera_list_.EnableWindow(FALSE);
	combox_camera_resolutions_.EnableWindow(FALSE);
	combox_camera_frame_rate_.EnableWindow(FALSE);

	btn_camera_preview_.EnableWindow(FALSE);

	auto wc = AfxRegisterWndClass(0, 0, (HBRUSH)COLOR_WINDOW);

	CRect preview_rect(0, 0, 2,2);
	preview_rect = GetCameraPreviewRect();
	if (preview_rect.IsRectEmpty())
		preview_rect = CRect(0, 0, 2, 2);

	auto rc = camera_preview_wnd_.Create(wc, _T("camerapreview2x0"),
		WS_CHILD | WS_VISIBLE | WS_BORDER, preview_rect, this, 230);

	
	/*CRect preview_rect2(1, 1, 150, 110);
	camera_preview_wnd2_.Create(wc, _T("camerapreview2x1"),
	WS_CHILD | WS_VISIBLE | WS_BORDER, preview_rect2, this, 231);*/
	

	auto screen_w = 0;
	auto screen_h = 0;
	GetScreenSize(screen_w, screen_h);

	edit_frame_rate_.SetWindowTextW(GetIntString(DEF_FRAME_RATE).c_str());

	btn_check_dxgi_screen_capturer_.SetCheck(BST_CHECKED);

	if ( !IsWin7System() )
	{
		btn_check_capturer_disable_aero_.EnableWindow(FALSE);
	}
	else
	{
		btn_check_capturer_disable_aero_.SetCheck(BST_CHECKED);
	}

	btn_check_video_bitrate_.SetCheck(BST_CHECKED);
	edit_bit_rate_.EnableWindow(TRUE);
	edit_bit_rate_.SetWindowTextW(GetIntString(CalBitRate(DEF_FRAME_RATE, screen_w, screen_h)).c_str());

	edit_key_frame_.SetWindowTextW(GetIntString(DEF_KEY_FRAME).c_str());

	edit_video_quality_.EnableWindow(FALSE);
	edit_video_quality_.SetWindowTextW(GetIntString(DEF_VIDEO_QUALITY).c_str());
	edit_video_max_bitrate_.SetWindowTextW(GetIntString(CalMaxKBitRate(DEF_FRAME_RATE, screen_w, screen_h)).c_str());

	combox_h264_profile_.AddString(L"Baseline");
	combox_h264_profile_.AddString(L"Main");
	combox_h264_profile_.AddString(L"High");
	combox_h264_profile_.SetCurSel(0);

	edit_video_encode_speed_.SetWindowTextW(_T("3"));


	edit_publish_url1_.SetWindowTextW(w_def_publish_url);

	//edit_publish_url_.SetWindowTextW(L"rtmp://localhost:1935/live/streama");

	btn_check_auido_mic_input_.EnableWindow(FALSE);
	combox_auido_input_devices_.EnableWindow(FALSE);
	btn_check_set_mute_.EnableWindow(FALSE);
	btn_check_echo_cancel_.EnableWindow(FALSE);
	edit_echo_delay_.EnableWindow(FALSE);
	//edit_echo_delay_.SetWindowTextW(_T("280"));
	edit_echo_delay_.SetWindowTextW(_T("100"));


	btn_publish_.EnableWindow(FALSE);
	btn_stop_publish_.EnableWindow(FALSE);

	btn_rec_.EnableWindow(FALSE);
	btn_stop_rec_.EnableWindow(FALSE);

	NT_GetSmartPublisherSDKAPI(&publisher_api_);

	if (NT_ERC_OK != publisher_api_.Init(0, NULL))
	{
		AfxMessageBox(_T("call publisher_api_.Init failed"));

		return FALSE;
	}

	btn_publish_.EnableWindow(TRUE);
	btn_rec_.EnableWindow(TRUE);

	NT_INT32 auido_devices = 0;
	if (NT_ERC_OK == publisher_api_.GetAuidoInputDeviceNumber(&auido_devices))
	{
		if (auido_devices > 0 )
		{
			btn_check_auido_mic_input_.EnableWindow(TRUE);

			for (auto i = 0; i < auido_devices; ++i)
			{
				std::wstring w_name;

				char name_buffer[512] = { 0 };
				if (NT_ERC_OK == publisher_api_.GetAuidoInputDeviceName(i, name_buffer, sizeof(name_buffer) / sizeof(char)))
				{
					std::string name = name_buffer;
					w_name = ConvertToWString(name);
				}

				std::wstringstream ss;

				if (w_name.empty())
				{
					ss << L"音频采集设备-";
				}
				else
				{
					ss << w_name << L"-";
				}
				
				ss << (i+1);

				combox_auido_input_devices_.AddString(ss.str().c_str());
			}

			combox_auido_input_devices_.SetCurSel(0);

			//combox_auido_input_devices_.EnableWindow(FALSE);
			//btn_check_set_mute_.EnableWindow(FALSE);
		}
	}

	if ( IsCanCaptureSpeaker() )
	{
		btn_check_auido_speaker_input_.EnableWindow(TRUE);
	}
	else
	{
		btn_check_auido_speaker_input_.EnableWindow(FALSE);
	}

	if ( btn_check_auido_mic_input_.IsWindowEnabled()
		|| btn_check_auido_speaker_input_.IsWindowEnabled() )
	{
		btn_check_speex_encoder_.EnableWindow(TRUE);
		btn_check_noise_suppression_.EnableWindow(TRUE);
		btn_check_agc_.EnableWindow(TRUE);
		btn_check_vad_.EnableWindow(TRUE);
	}
	else
	{
		btn_check_speex_encoder_.EnableWindow(FALSE);
		btn_check_noise_suppression_.EnableWindow(FALSE);
		btn_check_agc_.EnableWindow(FALSE);
		btn_check_vad_.EnableWindow(FALSE);
	}

	edit_speex_quality_.SetWindowTextW(_T("8"));

	edit_rec_max_file_size_.SetWindowTextW(_T("200"));
	btn_check_rec_append_date_.SetCheck(BST_CHECKED);
	btn_check_append_time_.SetCheck(BST_CHECKED);

	FillCameraInfo();

	InitCameraControl();

	if ( !cameras_.empty() )
	{
		btn_check_desktop_camera_switch_.EnableWindow(TRUE);
		btn_check_camera_overlay_to_desktop_.EnableWindow(TRUE);;
		btn_check_desktop_overlay_to_camera_.EnableWindow(TRUE);;
	}

	edit_camera_rotate_degress_.SetWindowTextW(_T("0度"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSmartPublisherDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSmartPublisherDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSmartPublisherDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSmartPublisherDemoDlg::OnBnClickedButtonPublish()
{
	if ( BST_CHECKED == btn_check_window_input_.GetCheck() )
	{
		if ( nullptr == cur_sel_capture_window_ )
		{
			AfxMessageBox(_T("请先下拉选择采集窗口"));
			return;
		}
	}

	edit_gen_url_.SetWindowTextW(_T(""));

	if ( publisher_handle_ == NULL)
	{
		if ( !OpenPublisherHandle() )
		{
			return;
		}
	}

	if ( publisher_handle_count_ < 1 )
	{
		SetCommonOptionToPublisherSDK();
	}

	auto url2 = GetStringValueFromEdit(&edit_publish_url2_);
	auto url3 = GetStringValueFromEdit(&edit_publish_url3_);
	
	std::string display_url;

	auto url = GetStringValueFromEdit(&edit_publish_url1_);
	if ( url == def_publish_url )
	{
		if ( url2.empty() && url3.empty() )
		{
			std::stringstream ss;
			ss << url;
			ss << (::GetTickCount() % 100000);

			url = ss.str();

			display_url = url;
		}
		else
		{
			url.clear();

			if ( !url2.empty() )
			{
				display_url = url2;
			}
			else if ( !url3.empty() )
			{
				display_url = url3;
			}
		}
	}
	else
	{
		if ( !url.empty())
		{
			display_url = url;
		}
		else if ( !url2.empty() )
		{
			display_url = url2;
		}
		else if (!url3.empty())
		{
			display_url = url3;
		}
	}

	if ( display_url.empty() )
	{
		if ( 0 == publisher_handle_count_ )
		{
			publisher_api_.Close(publisher_handle_);
			publisher_handle_ = NULL;
		}

		AfxMessageBox(_T("请输入推送地址"));
		return;
	}


	ASSERT(publisher_handle_ != NULL);
	if ( !url.empty() )
	{
		publisher_api_.SetURL(publisher_handle_, url.c_str(), NULL);

		// 测试
		/*
		publisher_api_.SetVideoPacketTimestampCallBack(publisher_handle_, url.c_str(),
			2000, NULL, &NT_PB_SDKVideoPacketTimestampHandle);
		*/
	}
	
	if ( !url2.empty() )
	{
		publisher_api_.SetURL(publisher_handle_, url2.c_str(), NULL);
	}

	if ( !url3.empty() )
	{
		publisher_api_.SetURL(publisher_handle_, url3.c_str(), NULL);
	}

	// 这个接口可以不要用，直接使用 SetEventCallBack 接口, 相关对应的事件ID都已经定义了,只要对应处理就好
	//publisher_api_.SetPublisherStatusCallBack(publisher_handle_, GetSafeHwnd(), &NT_PB_SDKPublisherStatusHandle);

	if ( NT_ERC_OK != publisher_api_.StartPublisher(publisher_handle_, NULL) )
	{
		if ( 0 == publisher_handle_count_ )
		{
			publisher_api_.Close(publisher_handle_);
			publisher_handle_ = NULL;
		}

		AfxMessageBox(_T("调用推流接口失败"));
		return;
	}

	publisher_handle_count_++;

	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	auto w_url = conv.from_bytes(display_url);

	edit_gen_url_.SetWindowTextW(w_url.c_str());

	btn_publish_.EnableWindow(FALSE);
	btn_stop_publish_.EnableWindow(TRUE);

	if ( 1 == publisher_handle_count_ )
	{
		if (BST_CHECKED == btn_check_desktop_input_.GetCheck())
		{
			btn_choose_screen_region_.SetWindowTextW(L"移动屏幕区域");
		}

		btn_check_dxgi_screen_capturer_.EnableWindow(FALSE);

		btn_check_desktop_camera_switch_.EnableWindow(FALSE);
		btn_check_camera_overlay_to_desktop_.EnableWindow(FALSE);
		btn_check_desktop_overlay_to_camera_.EnableWindow(FALSE);

		btn_add_image_watermark_.EnableWindow(FALSE);

		if (BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
			|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck()
			|| BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck())
		{

		}
		else
		{
			btn_check_desktop_input_.EnableWindow(FALSE);
			btn_check_camera_input_.EnableWindow(FALSE);
		}

		DisableAuidoInputControl();
	}
}

void CSmartPublisherDemoDlg::OnBnClickedButtonStop()
{
	publisher_handle_count_--;
	publisher_api_.StopPublisher(publisher_handle_);

	if ( 0 == publisher_handle_count_ )
	{
		publisher_api_.Close(publisher_handle_);
		publisher_handle_ = NULL;
	}
	
	btn_publish_.EnableWindow(TRUE);
	btn_stop_publish_.EnableWindow(FALSE);

	if ( 0 == publisher_handle_count_ )
	{
		if ( BST_CHECKED == btn_check_desktop_input_.GetCheck() )
		{
			btn_choose_screen_region_.SetWindowTextW(L"选择屏幕区域");
		}

		btn_check_dxgi_screen_capturer_.EnableWindow(TRUE);

		btn_desktop_camera_switch_.SetWindowTextW(L"切换到摄像头");
		btn_disable_image_watermark_.SetWindowTextW(L"停止水印");
		btn_disable_camera_overlay_.SetWindowTextW(L"停止叠加摄像头");
		btn_disable_desktop_overlay_.SetWindowTextW(L"停止叠加屏幕");

		btn_check_desktop_camera_switch_.EnableWindow(TRUE);
		btn_check_camera_overlay_to_desktop_.EnableWindow(TRUE);
		btn_check_desktop_overlay_to_camera_.EnableWindow(TRUE);

		btn_add_image_watermark_.EnableWindow(TRUE);

		if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
			|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck()
			|| BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck())
		{

		}
		else
		{
			btn_check_desktop_input_.EnableWindow(TRUE);
			btn_check_camera_input_.EnableWindow(TRUE);
		}

		EnableAuidoInputControl();
	}
}

void CSmartPublisherDemoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if ( region_choose_tool_handle_ != NULL )
	{
		publisher_api_.CloseScreenRegionChooseTool(region_choose_tool_handle_);
		region_choose_tool_handle_ = NULL;
	}

	if ( publisher_handle_ != NULL )
	{
		publisher_api_.StopPublisher(publisher_handle_);
		publisher_api_.StopRecorder(publisher_handle_);
		publisher_api_.Close(publisher_handle_);

		publisher_handle_ = NULL;

	}

	publisher_api_.UnInit();
}

void CSmartPublisherDemoDlg::SetIntValueToEdit(CEdit* pEdit, int val)
{
	if (pEdit == NULL)
		return;

	if ( !::IsWindow(pEdit->m_hWnd) )
		return;

	std::wostringstream ss;
	ss << val;

	pEdit->SetWindowTextW(ss.str().c_str());
}

int CSmartPublisherDemoDlg::GetIntValueFromEdit(CEdit* pEdit)
{
	if (pEdit == NULL)
		return 0;

	if (!::IsWindow(pEdit->m_hWnd))
		return 0;

	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;

	CString w_val_str;
	pEdit->GetWindowTextW(w_val_str);

	auto val_str = conv.to_bytes(w_val_str);

	return atoi(val_str.c_str());

}

int CSmartPublisherDemoDlg::ClipIntValue(int val, int min, int max)
{
	if (val < min)
	{
		return min;
	}

	if (val > max)
	{
		return max;
	}

	return val; 
}

std::string CSmartPublisherDemoDlg::GetStringValueFromEdit(CEdit* pEdit)
{
	if (pEdit == NULL)
		return std::string();

	if (!::IsWindow(pEdit->m_hWnd))
		return std::string();

	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;

	CString w_val_str;
	pEdit->GetWindowTextW(w_val_str);

	auto val_str = conv.to_bytes(w_val_str);

	return val_str;
}

std::wstring CSmartPublisherDemoDlg::GetIntString(int val)
{
	std::wostringstream ss;
	ss << val;

	return ss.str();
}

std::wstring CSmartPublisherDemoDlg::ConvertToWString(const std::string& val)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;

	auto val_str = conv.from_bytes(val);

	return val_str;
}


int CSmartPublisherDemoDlg::CalMaxKBitRate(int frame_rate, int w, int h)
{
	auto max_kbit_rate = 2000;

	int area = w*h;

	if (area <= (320 * 300))
	{
		max_kbit_rate = 600;
	}
	else if (area <= (360 * 320))
	{
		max_kbit_rate = 800;
	}
	else if (area <= (640 * 480))
	{
		max_kbit_rate = 1300;
	}
	else if (area <= (800 * 600))
	{
		max_kbit_rate = 1500;
	}
	else if (area <= (900 * 700))
	{
		max_kbit_rate = 2200;
	}
	else if (area <= (1280 * 720))
	{
		max_kbit_rate = 3000;
	}
	else if (area <= (1366 * 768))
	{
		max_kbit_rate = 4000;
	}
	else if (area <= (1600 * 900))
	{
		max_kbit_rate = 4500;
	}
	else if (area <= (1600 * 1050))
	{
		max_kbit_rate = 4800;
	}
	else
	{
		max_kbit_rate = 5500;
	}

	max_kbit_rate = max_kbit_rate*frame_rate / 25;

	if ( max_kbit_rate < 130 )
		max_kbit_rate = 140;

	return max_kbit_rate;
}


void CSmartPublisherDemoDlg::OnBnClickedCheckVideoBitrate()
{
	if ( BST_CHECKED == btn_check_video_bitrate_.GetCheck() )
	{
		edit_bit_rate_.EnableWindow(TRUE);

		int kbit_rate = 0;
		int max_kbit_rate = 0;
		CalDefBitrate(kbit_rate, max_kbit_rate);

		edit_bit_rate_.SetWindowTextW(GetIntString(kbit_rate).c_str());
		edit_video_quality_.EnableWindow(FALSE);
	}
	else
	{
		edit_bit_rate_.EnableWindow(FALSE);
		edit_bit_rate_.SetWindowTextW(_T("0"));
		edit_video_quality_.EnableWindow(TRUE);
	}
}

void CSmartPublisherDemoDlg::OnBnClickedCheckVideoSameIamge()
{
	if ( BST_CHECKED == btn_check_same_image_.GetCheck() )
	{
		edit_max_video_frame_interval_.SetWindowTextW(_T("10000"));
		edit_max_video_key_frame_interval_.SetWindowTextW(_T("20000"));
	}
	else
	{
		edit_max_video_frame_interval_.SetWindowTextW(_T(""));
		edit_max_video_key_frame_interval_.SetWindowTextW(_T(""));
	}
}

int CSmartPublisherDemoDlg::CalBitRate(int frame_rate, int w, int h)
{
	auto kbit_rate = 2000;

	int area = w*h;

	if (area <= (320 * 300))
	{
		kbit_rate = 280;
	}
	else if (area <= (360 * 320))
	{
		kbit_rate = 360;
	}
	else if (area <= (640 * 480))
	{
		kbit_rate = 580;
	}
	else if (area <= (800 * 600))
	{
		kbit_rate = 620;
	}
	else if (area <= (900 * 700))
	{
		kbit_rate = 820;
	}
	else if (area <= (1280 * 720))
	{
		kbit_rate = 1600;
	}
	else if (area <= (1366 * 768))
	{
		kbit_rate = 2000;
	}
	else if (area <= (1600 * 900))
	{
		kbit_rate = 2300;
	}
	else if (area <= (1600 * 1050))
	{
		kbit_rate = 2500;
	}
	else
	{
		kbit_rate = 2800;
	}

	kbit_rate = kbit_rate*frame_rate / 25;

	if ( kbit_rate < 80 )
		kbit_rate = 80;

	return kbit_rate;
}


void CSmartPublisherDemoDlg::OnBnClickedButtonBitrateControlDefValue()
{
	FillBitrateControlDefValue();
}

void CSmartPublisherDemoDlg::OnBnClickedButtonBitrateControlDefValue2()
{
	FillBitrateControlDefValue();
}

void CSmartPublisherDemoDlg::OnBnClickedButtonBitrateControlDefValue3()
{
	FillBitrateControlDefValue();
}


void CSmartPublisherDemoDlg::CalDefBitrate(int& kbit_rate, int& max_kbit_rate)
{
	kbit_rate = 400;
	max_kbit_rate = 800;

	auto w = 640;
	auto h = 480;
	auto frame_rate = 5;

	if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck() 
		|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck() )
	{
		GetScreenSize(w, h);

		if ( GetIntValueFromEdit(&edit_clip_width_) > 0
			&& GetIntValueFromEdit(&edit_clip_height_) > 0 )
		{
			w = GetIntValueFromEdit(&edit_clip_width_);
			h = GetIntValueFromEdit(&edit_clip_height_);
		}

		frame_rate = GetIntValueFromEdit(&edit_frame_rate_);
	}
	else if (BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck())
	{
		if ( CB_ERR != cur_sel_camera_index_
			&& CB_ERR != cur_sel_camera_resolutions_index_
			&& CB_ERR != cur_sel_camera_frame_rate_index_ )
		{
			auto& camera = cameras_[cur_sel_camera_index_];
			auto& cap = camera.capabilities_[cur_sel_camera_resolutions_index_];

			w = cap.width_;
			h = cap.height_;

			frame_rate = cur_sel_camera_frame_rate_index_ + 1;
		}
	}
	else if ( BST_CHECKED == btn_check_desktop_input_.GetCheck() )
	{
		GetScreenSize(w, h);

		if (GetIntValueFromEdit(&edit_clip_width_) > 0
			&& GetIntValueFromEdit(&edit_clip_height_) > 0)
		{
			w = GetIntValueFromEdit(&edit_clip_width_);
			h = GetIntValueFromEdit(&edit_clip_height_);
		}

		frame_rate = GetIntValueFromEdit(&edit_frame_rate_);
	}
	else if ( BST_CHECKED == btn_check_camera_input_.GetCheck() )
	{
		if ( CB_ERR != cur_sel_camera_index_
			&& CB_ERR != cur_sel_camera_resolutions_index_
			&& CB_ERR != cur_sel_camera_frame_rate_index_ )
		{
			auto& camera = cameras_[cur_sel_camera_index_];
			auto& cap = camera.capabilities_[cur_sel_camera_resolutions_index_];

			w = cap.width_;
			h = cap.height_;

			frame_rate = cur_sel_camera_frame_rate_index_ + 1;
		}
	}

	kbit_rate = CalBitRate(frame_rate, w, h);
	max_kbit_rate = CalMaxKBitRate(frame_rate, w, h);

	if (BST_CHECKED == btn_check_camera_input_.GetCheck())
	{
		max_kbit_rate += kbit_rate / 3;
		kbit_rate += kbit_rate / 3;
	}
}


void CSmartPublisherDemoDlg::FillBitrateGroupItem(NT_HANDLE publisher_handle, int frame_rate, int w, int h)
{
	ASSERT(publisher_handle != nullptr);

	NT_PB_VideoEncoderBitrateGroupItem item = { 0 };

	item.video_area_ = w*h;
	item.kbitrate_ = CalBitRate(frame_rate, w, h);
	item.max_kbitrate_ = CalMaxKBitRate(frame_rate, w, h);

	publisher_api_.AddVideoEncoderBitrateGroupItem(publisher_handle, &item);
}

void CSmartPublisherDemoDlg::SetBitrateGroup(NT_HANDLE publisher_handle, int frame_rate)
{
	if (publisher_handle == nullptr)
		return;

	publisher_api_.ClearVideoEncoderBitrateGroup(publisher_handle_);

	FillBitrateGroupItem(publisher_handle, frame_rate, 320, 300);

	FillBitrateGroupItem(publisher_handle, frame_rate, 360, 320);

	FillBitrateGroupItem(publisher_handle, frame_rate, 640, 480);

	FillBitrateGroupItem(publisher_handle, frame_rate, 800, 600);

	FillBitrateGroupItem(publisher_handle, frame_rate, 900, 700);

	FillBitrateGroupItem(publisher_handle, frame_rate, 1280, 720);

	FillBitrateGroupItem(publisher_handle, frame_rate, 1366, 768);

	FillBitrateGroupItem(publisher_handle, frame_rate, 1600, 900);

	FillBitrateGroupItem(publisher_handle, frame_rate, 1600, 1050);
	
	FillBitrateGroupItem(publisher_handle, frame_rate, 1920, 1080);

}

void CSmartPublisherDemoDlg::OnBnClickedCheckSetMute()
{
	if (publisher_handle_ != NULL)
	{
		if (BST_CHECKED == btn_check_set_mute_.GetCheck())
		{
			publisher_api_.SetMute(publisher_handle_, 1);
		}
		else
		{
			publisher_api_.SetMute(publisher_handle_, 0);
		}
	}
}

void CSmartPublisherDemoDlg::OnBnClickedCheckAudioMicInput()
{
	if ( BST_CHECKED == btn_check_auido_mic_input_.GetCheck() )
	{
		combox_auido_input_devices_.EnableWindow(TRUE);
		btn_check_set_mute_.EnableWindow(TRUE);

		if ( BST_CHECKED == btn_check_auido_speaker_input_.GetCheck() )
		{
			btn_check_echo_cancel_.EnableWindow(FALSE);
			edit_echo_delay_.EnableWindow(FALSE);
		}
		else
		{
			btn_check_echo_cancel_.EnableWindow(TRUE);
			edit_echo_delay_.EnableWindow(TRUE);
		}
	}
	else
	{
		if ( BST_CHECKED != btn_check_auido_speaker_input_.GetCheck() )
		{
			btn_check_set_mute_.EnableWindow(FALSE);
		}
		
		combox_auido_input_devices_.EnableWindow(FALSE);
		btn_check_echo_cancel_.EnableWindow(FALSE);
		edit_echo_delay_.EnableWindow(FALSE);
	}
}

void CSmartPublisherDemoDlg::OnBnClickedCheckAuidoSpeakerInput()
{
	if ( BST_CHECKED == btn_check_auido_speaker_input_.GetCheck() )
	{
		btn_check_set_mute_.EnableWindow(TRUE);

		if ( BST_CHECKED == btn_check_echo_cancel_.GetCheck() )
		{
			combox_auido_input_devices_.EnableWindow(FALSE);
			btn_check_auido_mic_input_.SetCheck(BST_UNCHECKED);
		}
	
		btn_check_echo_cancel_.EnableWindow(FALSE);
		btn_check_echo_cancel_.SetCheck(BST_UNCHECKED);
		edit_echo_delay_.EnableWindow(FALSE);
	}
	else
	{
		if ( BST_CHECKED != btn_check_auido_mic_input_.GetCheck() )
		{
			btn_check_set_mute_.EnableWindow(FALSE);
		}
		else
		{
			btn_check_echo_cancel_.EnableWindow(TRUE);
			edit_echo_delay_.EnableWindow(TRUE);
		}
	}
}

bool CSmartPublisherDemoDlg::IsCanCaptureSpeaker()
{
	NT_INT32 is_capture_speader = 0;
	if ( publisher_api_.IsCanCaptureSpeaker != NULL 
		&& NT_ERC_OK == publisher_api_.IsCanCaptureSpeaker(&is_capture_speader))
	{
		if (is_capture_speader)
		{
			return true;
		}
	}

	return false;
}


void CSmartPublisherDemoDlg::OnBnClickedButtonChooseRecDir()
{
	std::wstring    sel_dir;

	BROWSEINFO      bi;
	::ZeroMemory(&bi, sizeof(BROWSEINFO));

	bi.pidlRoot = 0;
	bi.lpszTitle = _T("请选择一个纯英文的录像目录：");
	bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	bi.lpfn = NULL;

	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&bi);
	if ( lpidlBrowse != NULL )
	{
		TCHAR  folder_path[MAX_PATH] = { 0 };
		if (::SHGetPathFromIDList(lpidlBrowse, folder_path))
		{
			sel_dir = folder_path;
		}
	}

	if ( lpidlBrowse != NULL )
	{
		::CoTaskMemFree(lpidlBrowse);
	}

	edit_rec_dir_.SetWindowTextW(sel_dir.c_str());
}


void CSmartPublisherDemoDlg::OnBnClickedButtonRec()
{
	if ( BST_CHECKED == btn_check_window_input_.GetCheck() )
	{
		if (nullptr == cur_sel_capture_window_)
		{
			AfxMessageBox(_T("请先下拉选择采集窗口"));
			return;
		}
	}

	if ( publisher_handle_ == NULL )
	{
		if (!OpenPublisherHandle())
		{
			return;
		}
	}

	if ( publisher_handle_count_ < 1 )
	{
		SetCommonOptionToPublisherSDK();
	}

	auto rec_dir = GetStringValueFromEdit(&edit_rec_dir_);
	
	ASSERT(publisher_handle_ != NULL);
	auto ret = publisher_api_.SetRecorderDirectory(publisher_handle_, rec_dir.c_str(), NULL);
	if ( NT_ERC_OK != ret )
	{
		if (0 == publisher_handle_count_)
		{
			publisher_api_.Close(publisher_handle_);
			publisher_handle_ = NULL;
		}

		AfxMessageBox(_T("设置录像目录失败，请确保目录存在且是英文目录"));
		return;
	}

	auto rec_max_file_size = GetIntValueFromEdit(&edit_rec_max_file_size_);
	rec_max_file_size *= 1024;
	publisher_api_.SetRecorderFileMaxSize(publisher_handle_, rec_max_file_size);

	NT_PB_RecorderFileNameRuler rec_name_ruler = { 0 };

	auto rec_file_name_prefix_ = GetStringValueFromEdit(&edit_file_name_prefix_);
	rec_name_ruler.file_name_prefix_ = rec_file_name_prefix_.c_str();

	if (BST_CHECKED == btn_check_rec_append_date_.GetCheck())
	{
		rec_name_ruler.append_date_ = 1;
	}

	if ( BST_CHECKED == btn_check_append_time_.GetCheck() )
	{
		rec_name_ruler.append_time_ = 1;
	}

	publisher_api_.SetRecorderFileNameRuler(publisher_handle_, &rec_name_ruler);

	if (NT_ERC_OK != publisher_api_.StartRecorder(publisher_handle_, NULL))
	{
		if ( 0 == publisher_handle_count_ )
		{
			publisher_api_.Close(publisher_handle_);
			publisher_handle_ = NULL;
		}

		AfxMessageBox(_T("调用录像接口失败!"));
		return;
	}

	publisher_handle_count_++;

	btn_rec_.EnableWindow(FALSE);
	btn_stop_rec_.EnableWindow(TRUE);

	if (1 == publisher_handle_count_)
	{
		if (BST_CHECKED == btn_check_desktop_input_.GetCheck())
		{
			btn_choose_screen_region_.SetWindowTextW(L"移动屏幕区域");
		}

		btn_check_dxgi_screen_capturer_.EnableWindow(FALSE);

		btn_check_desktop_camera_switch_.EnableWindow(FALSE);
		btn_check_camera_overlay_to_desktop_.EnableWindow(FALSE);
		btn_check_desktop_overlay_to_camera_.EnableWindow(FALSE);

		btn_add_image_watermark_.EnableWindow(FALSE);

		if (BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
			|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck()
			|| BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck())
		{

		}
		else
		{
			btn_check_desktop_input_.EnableWindow(FALSE);
			btn_check_camera_input_.EnableWindow(FALSE);
		}

		DisableAuidoInputControl();
	}
}


void CSmartPublisherDemoDlg::OnBnClickedButtonStopRec()
{
	publisher_handle_count_--;
	publisher_api_.StopRecorder(publisher_handle_);

	if ( 0 == publisher_handle_count_ )
	{
		publisher_api_.Close(publisher_handle_);
		publisher_handle_ = NULL;
	}

	btn_rec_.EnableWindow(TRUE);
	btn_stop_rec_.EnableWindow(FALSE);

	if (0 == publisher_handle_count_)
	{
		if (BST_CHECKED == btn_check_desktop_input_.GetCheck())
		{
			btn_choose_screen_region_.SetWindowTextW(L"选择屏幕区域");
		}

		btn_check_dxgi_screen_capturer_.EnableWindow(TRUE);

		btn_desktop_camera_switch_.SetWindowTextW(L"切换到摄像头");
		btn_disable_image_watermark_.SetWindowTextW(L"停止水印");
		btn_disable_camera_overlay_.SetWindowTextW(L"停止叠加摄像头");
		btn_disable_desktop_overlay_.SetWindowTextW(L"停止叠加屏幕");

		btn_check_desktop_camera_switch_.EnableWindow(TRUE);
		btn_check_camera_overlay_to_desktop_.EnableWindow(TRUE);
		btn_check_desktop_overlay_to_camera_.EnableWindow(TRUE);

		btn_add_image_watermark_.EnableWindow(TRUE);

		if (BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
			|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck()
			|| BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck())
		{

		}
		else
		{
			btn_check_desktop_input_.EnableWindow(TRUE);
			btn_check_camera_input_.EnableWindow(TRUE);
		}

		EnableAuidoInputControl();
	}
}

void CSmartPublisherDemoDlg::DisableAuidoInputControl()
{
	auto count = combox_auido_input_devices_.GetCount();
	if ( count != CB_ERR && count > 0 )
	{
		btn_check_auido_mic_input_.EnableWindow(FALSE);

		if (BST_CHECKED == btn_check_auido_mic_input_.GetCheck())
		{
			combox_auido_input_devices_.EnableWindow(FALSE);
		}

		btn_check_echo_cancel_.EnableWindow(FALSE);
		edit_echo_delay_.EnableWindow(FALSE);
	}

	if ( btn_check_auido_speaker_input_.IsWindowEnabled() )
	{
		btn_check_auido_speaker_input_.EnableWindow(FALSE);
	}

	btn_check_speex_encoder_.EnableWindow(FALSE);
	edit_speex_quality_.EnableWindow(FALSE);
	btn_check_noise_suppression_.EnableWindow(FALSE);
	btn_check_agc_.EnableWindow(FALSE);
	btn_check_vad_.EnableWindow(FALSE);
}

void CSmartPublisherDemoDlg::EnableAuidoInputControl()
{
	auto count = combox_auido_input_devices_.GetCount();
	if (count != CB_ERR && count > 0)
	{
		btn_check_auido_mic_input_.EnableWindow(TRUE);

		if ( BST_CHECKED == btn_check_auido_mic_input_.GetCheck() )
		{
			combox_auido_input_devices_.EnableWindow(TRUE);

			btn_check_echo_cancel_.EnableWindow(TRUE);
			edit_echo_delay_.EnableWindow(TRUE);
		}
	}

	if ( IsCanCaptureSpeaker() )
	{
		btn_check_auido_speaker_input_.EnableWindow(TRUE);
	}

	if ( btn_check_auido_mic_input_.IsWindowEnabled()
		|| btn_check_auido_speaker_input_.IsWindowEnabled() )
	{
		btn_check_speex_encoder_.EnableWindow(TRUE);
		edit_speex_quality_.EnableWindow(TRUE);
		btn_check_noise_suppression_.EnableWindow(TRUE);
		btn_check_agc_.EnableWindow(TRUE);
		btn_check_vad_.EnableWindow(TRUE);
	}

	if ( BST_CHECKED == btn_check_auido_mic_input_.GetCheck()
		&& BST_CHECKED == btn_check_auido_speaker_input_.GetCheck() )
	{
		btn_check_echo_cancel_.EnableWindow(FALSE);
		edit_echo_delay_.EnableWindow(FALSE);
	}
}

bool CSmartPublisherDemoDlg::OpenPublisherHandle()
{
	if ( publisher_handle_ != NULL )
	{
		return true;
	}

	// 视频
	auto video_option = NT_PB_E_VIDEO_OPTION_NO_VIDEO;

	if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
		|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck() 
		|| BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck() )
	{
		// 使用叠加模式
		video_option = NT_PB_E_VIDEO_OPTION_LAYER;
	}
	else if ( BST_CHECKED == btn_check_desktop_input_.GetCheck() )
	{
		video_option = NT_PB_E_VIDEO_OPTION_SCREEN;
	}
	else if (BST_CHECKED == btn_check_window_input_.GetCheck())
	{
		video_option = NT_PB_E_VIDEO_OPTION_WINDOW;

		// video_option = NT_PB_E_VIDEO_OPTION_LAYER;
	}
	else if (BST_CHECKED == btn_check_camera_input_.GetCheck())
	{
		video_option = NT_PB_E_VIDEO_OPTION_CAMERA;
	}

	// 音频
	auto audio_option = NT_PB_E_AUDIO_OPTION_NO_AUDIO;

	audio_option = NT_PB_E_AUDIO_OPTION_NO_AUDIO;

	if ( BST_CHECKED == btn_check_auido_mic_input_.GetCheck()
		&& BST_CHECKED == btn_check_auido_speaker_input_.GetCheck() )
	{
		audio_option = NT_PB_E_AUDIO_OPTION_CAPTURE_MIC_SPEAKER_MIXER;
	}
	else if ( BST_CHECKED == btn_check_auido_mic_input_.GetCheck() )
	{
		audio_option = NT_PB_E_AUDIO_OPTION_CAPTURE_MIC;
	}
	else if (BST_CHECKED == btn_check_auido_speaker_input_.GetCheck())
	{
		audio_option = NT_PB_E_AUDIO_OPTION_CAPTURE_SPEAKER;
	}

	publisher_handle_count_ = 0;

	if ( NT_ERC_OK != publisher_api_.Open(&publisher_handle_,
		video_option, audio_option, 0, NULL))
	{
		AfxMessageBox(_T("Call open failed!"));
		return false;
	}

	if ( publisher_handle_ != NULL )
	{
		// 设置事件回调
		publisher_api_.SetEventCallBack(publisher_handle_, GetSafeHwnd(), &NT_PB_SDKPublisherEventHandle);

		return true;
	}
	else
	{
		return false;
	}
}

void CSmartPublisherDemoDlg::SetCommonOptionToPublisherSDK()
{
	ASSERT(publisher_handle_ != NULL);

	layer_conf_wrappers_.clear();

	// 视频相关设置
	if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
		|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck()
		|| BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck()
		|| BST_CHECKED == btn_check_desktop_input_.GetCheck()
		|| BST_CHECKED == btn_check_window_input_.GetCheck()
		|| BST_CHECKED == btn_check_camera_input_.GetCheck() )
	{
		if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck() )
		{
			auto left = GetIntValueFromEdit(&edit_clip_left_);
			auto top = GetIntValueFromEdit(&edit_clip_top_);
			auto w = GetIntValueFromEdit(&edit_clip_width_);
			auto h = GetIntValueFromEdit(&edit_clip_height_);

			// 有一个是0， 就使用全屏
			if ( w == 0 || h == 0 )
			{
				left = 0;
				top  = 0;
				GetScreenSize(w, h);
			}
			else
			{
				// 保证4字节对齐
				w = NT_ByteAlign(w, 2);
				h = NT_ByteAlign(h, 4);
			}

			auto index = 0;

			// 第0层填充RGBA矩形, 目的是保证帧率, 颜色就填充全黑
			auto rgba_layer_c0 = std::make_shared<nt_pb_sdk::RGBARectangleLayerConfigWrapper>(index++, true,
				0, 0, w, h);

			rgba_layer_c0->conf_.red_	= 0;
			rgba_layer_c0->conf_.green_ = 0;
			rgba_layer_c0->conf_.blue_	= 0;
			rgba_layer_c0->conf_.alpha_ = 255;

			layer_conf_wrappers_.push_back(rgba_layer_c0);

			// 第1层是摄像头
			if ( CB_ERR != cur_sel_camera_index_ )
			{
				auto& camera = cameras_[cur_sel_camera_index_];
				
				auto camera_layer_c1 = std::make_shared<nt_pb_sdk::CameraLayerConfigWrapperV2>(index++, false,
					0, 0, w, h);

				strcpy_s(camera_layer_c1->conf_.device_unique_id_utf8_, camera.id_.c_str());

				camera_layer_c1->conf_.is_flip_horizontal_ = BST_CHECKED == btn_check_flip_horizontal_camera_.GetCheck();
				camera_layer_c1->conf_.is_flip_vertical_   = BST_CHECKED == btn_check_flip_vertical_camera_.GetCheck();

				// 这种叠加模式下不要旋转，否则变形厉害, 要么就定好一个角度，调整宽高，但不要动态旋转
				camera_layer_c1->conf_.rotate_degress_ = 0;

				layer_conf_wrappers_.push_back(camera_layer_c1);
			}

			// 第2层是屏幕
			auto screen_layer_c2 = std::make_shared<nt_pb_sdk::ScreenLayerConfigWrapper>(index++, true,
				0, 0, w, h);

			screen_layer_c2->conf_.reserve_ = nullptr;
			screen_layer_c2->conf_.clip_region_.x_ = left;
			screen_layer_c2->conf_.clip_region_.y_ = top;
			screen_layer_c2->conf_.clip_region_.width_  = w;
			screen_layer_c2->conf_.clip_region_.height_ = h;

			layer_conf_wrappers_.push_back(screen_layer_c2);


			// 第3层填充RGBA矩形, 增加半透明效果
			auto r = GetIntValueFromEdit(&edit_rgba_rect_layer_red_);
			r = ClipIntValue(r, 0, 255);

			auto g = GetIntValueFromEdit(&edit_rgba_rect_layer_green_);
			g = ClipIntValue(g, 0, 255);

			auto b = GetIntValueFromEdit(&edit_rgba_rect_layer_blue_);
			b = ClipIntValue(b, 0, 255);

			auto a = GetIntValueFromEdit(&edit_rgba_rect_layer_alpha_);
			a = ClipIntValue(a, 0, 255);

			auto rgba_layer_c3 = std::make_shared<nt_pb_sdk::RGBARectangleLayerConfigWrapper>(index++, a != 0,
				0, 0, w, h);

				rgba_layer_c3->conf_.red_	= r;
				rgba_layer_c3->conf_.green_ = g;
				rgba_layer_c3->conf_.blue_	= b;
				rgba_layer_c3->conf_.alpha_ = a;

			layer_conf_wrappers_.push_back(rgba_layer_c3);

			// 如果有图片，增加图片层
			if ( !image_layer_file_name_utf8_.empty()
				&& image_layer_width_ > 0 
				&& image_layer_height_ > 0)
			{
				auto image_layer_c4 = std::make_shared<nt_pb_sdk::ImageLayerConfigWrapper>(index++, true,
					image_layer_left_, image_layer_top_, 
					image_layer_width_, image_layer_height_ );

				strcpy_s(image_layer_c4->conf_.file_name_utf8_, image_layer_file_name_utf8_.c_str());
				
				// 图片层是可以增加背景的，有兴趣的话，可以打开看看效果
				/*image_layer_c4->conf_.is_setting_background_ = 1;

				image_layer_c4->conf_.bk_red_ = 255;
				image_layer_c4->conf_.bk_green_ = 255;
				image_layer_c4->conf_.bk_blue_ = 255;*/

				layer_conf_wrappers_.push_back(image_layer_c4);
			}

			SetLayersConfig();

			publisher_api_.SetFrameRate(publisher_handle_, GetIntValueFromEdit(&edit_frame_rate_));

		}
		else if (BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck())
		{
			auto left = GetIntValueFromEdit(&edit_clip_left_);
			auto top  = GetIntValueFromEdit(&edit_clip_top_);
			auto w = GetIntValueFromEdit(&edit_clip_width_);
			auto h = GetIntValueFromEdit(&edit_clip_height_);

			// 有一个是0， 就使用全屏
			if ( w == 0 || h == 0 )
			{
				left = 0;
				top = 0;
				GetScreenSize(w, h);
			}
			else
			{
				// 保证4字节对齐
				w = NT_ByteAlign(w, 2);
				h = NT_ByteAlign(h, 4);
			}

			auto index = 0;

			/*auto image_layer_c0 = std::make_shared<nt_pb_sdk::ImageLayerConfigWrapper>(index++, true,
				0, 0, w, h);

				strcpy_s(image_layer_c0->conf_.file_name_utf8_, "D:\\myxxyy\\testpng\\ggdwdd.png");

				image_layer_c0->conf_.is_setting_background_ = 1;

				image_layer_c0->conf_.bk_red_ = 255;
				image_layer_c0->conf_.bk_green_ = 255;
				image_layer_c0->conf_.bk_blue_ = 255;

				layer_conf_wrappers_.push_back(image_layer_c0);*/


			// 第0层是屏幕
			auto screen_layer_c0 = std::make_shared<nt_pb_sdk::ScreenLayerConfigWrapper>(index++, true,
				0, 0, w, h);

			screen_layer_c0->conf_.reserve_ = nullptr;
			screen_layer_c0->conf_.clip_region_.x_ = left;
			screen_layer_c0->conf_.clip_region_.y_ = top;
			screen_layer_c0->conf_.clip_region_.width_ = w;
			screen_layer_c0->conf_.clip_region_.height_ = h;

			layer_conf_wrappers_.push_back(screen_layer_c0);

			// 第1层是摄像头
			if ( CB_ERR != cur_sel_camera_index_ )
			{
				auto& camera = cameras_[cur_sel_camera_index_];

				auto c_l = GetIntValueFromEdit(&edit_camera_overlay_left_);
				auto c_t = GetIntValueFromEdit(&edit_camera_overlay_top_);

				auto c_w = GetIntValueFromEdit(&edit_camera_overlay_width_);
				auto c_h = GetIntValueFromEdit(&edit_camera_overlay_height_);

				if ( c_w == 0 )
				{
					c_w = w / 2;
				}

				if ( c_h == 0 )
				{
					c_h = h / 2;
				}

				if ( c_w >0 && c_h > 0 )
				{
					auto camera_layer_c1 = std::make_shared<nt_pb_sdk::CameraLayerConfigWrapperV2>(index++, true,
						c_l, c_t, c_w, c_h);

					strcpy_s(camera_layer_c1->conf_.device_unique_id_utf8_, camera.id_.c_str());

					camera_layer_c1->conf_.is_flip_horizontal_ = BST_CHECKED == btn_check_flip_horizontal_camera_.GetCheck();
					camera_layer_c1->conf_.is_flip_vertical_ = BST_CHECKED == btn_check_flip_vertical_camera_.GetCheck();
					camera_layer_c1->conf_.rotate_degress_ = GetCameraRotateDegress();

					layer_conf_wrappers_.push_back(camera_layer_c1);
				}	
			}
			
			SetLayersConfig();

			publisher_api_.SetFrameRate(publisher_handle_, GetIntValueFromEdit(&edit_frame_rate_));
		}
		else if (BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck())
		{
			if (CB_ERR != cur_sel_camera_index_
				&& CB_ERR != cur_sel_camera_resolutions_index_
				&& CB_ERR != cur_sel_camera_frame_rate_index_)
			{
				auto& camera = cameras_[cur_sel_camera_index_];
				auto& cap = camera.capabilities_[cur_sel_camera_resolutions_index_];

				auto index = 0;

				// 第0层是摄像头
				auto camera_layer_c0 = std::make_shared<nt_pb_sdk::CameraLayerConfigWrapperV2>(index++, true,
					0, 0, cap.width_, cap.height_);

				strcpy_s(camera_layer_c0->conf_.device_unique_id_utf8_, camera.id_.c_str());

				camera_layer_c0->conf_.is_flip_horizontal_ = BST_CHECKED == btn_check_flip_horizontal_camera_.GetCheck();
				camera_layer_c0->conf_.is_flip_vertical_   = BST_CHECKED == btn_check_flip_vertical_camera_.GetCheck();

				// 这种叠加模式下不要旋转，否则变形厉害, 要么就定好一个角度，调整宽高，但不要动态旋转
				camera_layer_c0->conf_.rotate_degress_ = 0;

				layer_conf_wrappers_.push_back(camera_layer_c0);

				// 第1层是屏幕
				auto screen_layer_c1 = std::make_shared<nt_pb_sdk::ScreenLayerConfigWrapper>(index++, true,
					0, 0, cap.width_ / 2, cap.height_/2);

				screen_layer_c1->conf_.reserve_ = nullptr;
				screen_layer_c1->conf_.clip_region_.x_ = 0;
				screen_layer_c1->conf_.clip_region_.y_ = 0;
				screen_layer_c1->conf_.clip_region_.width_ = cap.width_ / 2;
				screen_layer_c1->conf_.clip_region_.height_ = cap.height_ / 2;

				layer_conf_wrappers_.push_back(screen_layer_c1);

				SetLayersConfig();

				publisher_api_.SetFrameRate(publisher_handle_, cur_sel_camera_frame_rate_index_ + 1);
			}
		}
		else if ( BST_CHECKED == btn_check_desktop_input_.GetCheck() )
		{
			publisher_api_.SetScreenClip(publisher_handle_,
				GetIntValueFromEdit(&edit_clip_left_),
				GetIntValueFromEdit(&edit_clip_top_),
				GetIntValueFromEdit(&edit_clip_width_),
				GetIntValueFromEdit(&edit_clip_height_));

			publisher_api_.SetFrameRate(publisher_handle_, GetIntValueFromEdit(&edit_frame_rate_));
		}
		else if ( BST_CHECKED == btn_check_window_input_.GetCheck() )
		{
			if ( nullptr != cur_sel_capture_window_ )
			{
				publisher_api_.SetCaptureWindow(publisher_handle_, cur_sel_capture_window_);

				publisher_api_.SetFrameRate(publisher_handle_, GetIntValueFromEdit(&edit_frame_rate_));

				// 窗口层测试和演示代码++

				/*auto index = 0;

				auto rgba_layer_c0 = std::make_shared<nt_pb_sdk::RGBARectangleLayerConfigWrapper>(index++, true,
					0, 0, 1280, 720);

				rgba_layer_c0->conf_.red_ = 0;
				rgba_layer_c0->conf_.green_ = 255;
				rgba_layer_c0->conf_.blue_ = 0;
				rgba_layer_c0->conf_.alpha_ = 255;

				layer_conf_wrappers_.push_back(rgba_layer_c0);

				auto window_layer_c1 = std::make_shared<nt_pb_sdk::WindowLayerConfigWrapper>(index++, true,
					100, 100, 800, 600);

				window_layer_c1->conf_.window_ = cur_sel_capture_window_;

				layer_conf_wrappers_.push_back(window_layer_c1);*/

				// 窗口层测试和演示代码--

				SetLayersConfig();
			}
		}
		else if (BST_CHECKED == btn_check_camera_input_.GetCheck())
		{
			if (CB_ERR != cur_sel_camera_index_
				&& CB_ERR != cur_sel_camera_resolutions_index_
				&& CB_ERR != cur_sel_camera_frame_rate_index_)
			{
				auto& camera = cameras_[cur_sel_camera_index_];
				auto& cap = camera.capabilities_[cur_sel_camera_resolutions_index_];

				publisher_api_.SetVideoCaptureDeviceBaseParameter(publisher_handle_,
					camera.id_.c_str(), cap.width_, cap.height_);

				publisher_api_.SetFrameRate(publisher_handle_, cur_sel_camera_frame_rate_index_+ 1);

				publisher_api_.FlipVerticalCamera(publisher_handle_,
					BST_CHECKED == btn_check_flip_vertical_camera_.GetCheck());

				publisher_api_.FlipHorizontalCamera(publisher_handle_, 
					BST_CHECKED == btn_check_flip_horizontal_camera_.GetCheck());

				auto degress = GetCameraRotateDegress();
				publisher_api_.RotateCamera(publisher_handle_, degress);
			}
		}

		publisher_api_.EnableDXGIScreenCapturer(publisher_handle_, BST_CHECKED == btn_check_dxgi_screen_capturer_.GetCheck());

		publisher_api_.DisableAeroScreenCapturer(publisher_handle_, BST_CHECKED == btn_check_capturer_disable_aero_.GetCheck());


		if ( BST_CHECKED != btn_check_window_input_.GetCheck() )
		{
			publisher_api_.SetVideoBitRate(publisher_handle_, GetIntValueFromEdit(&edit_bit_rate_));
		}
		else
		{
			// 窗口的分辨率会变, 所以设置一组码率下去
			auto frame_rate = GetIntValueFromEdit(&edit_frame_rate_);
			SetBitrateGroup(publisher_handle_, frame_rate);
		}


		publisher_api_.SetVideoQuality(publisher_handle_, GetIntValueFromEdit(&edit_video_quality_));

		publisher_api_.SetVideoMaxBitRate(publisher_handle_, GetIntValueFromEdit(&edit_video_max_bitrate_));

		publisher_api_.SetVideoKeyFrameInterval(publisher_handle_, GetIntValueFromEdit(&edit_key_frame_));

		auto profile_sel = combox_h264_profile_.GetCurSel();
		if ( profile_sel != CB_ERR )
		{
			publisher_api_.SetVideoEncoderProfile(publisher_handle_, profile_sel + 1);
		}

		publisher_api_.SetVideoEncoderSpeed(publisher_handle_, GetIntValueFromEdit(&edit_video_encode_speed_));

		if (BST_CHECKED == btn_check_same_image_.GetCheck())
		{
			publisher_api_.SetVideoCompareSameImage(publisher_handle_, 1,
				GetIntValueFromEdit(&edit_max_video_frame_interval_));

			publisher_api_.SetVideoMaxKeyFrameInterval(publisher_handle_,
				GetIntValueFromEdit(&edit_max_video_key_frame_interval_));
		}
		else
		{
			publisher_api_.SetVideoCompareSameImage(publisher_handle_, 1, 0);

			publisher_api_.SetVideoMaxKeyFrameInterval(publisher_handle_, 0);
		}
	}

	// 合成测试代码, 下面的代码可以作为参考，测试摄像头层时，记着修改摄像头id
	//{
	//	auto index = 0;

	//	std::vector <std::shared_ptr<nt_layer_conf_wrapper_base> > conf_wrappers;

	//	// 第0层是RGBA矩形测试
	//	auto rgba_layer_c0 = std::make_shared<NT_RGBARectangleLayerConfigWrapper>(index++,
	//		0, 0, 640, 480);

	//	rgba_layer_c0->conf_.red_	= 120;
	//	rgba_layer_c0->conf_.green_ = 205;
	//	rgba_layer_c0->conf_.blue_	= 248;
	//	rgba_layer_c0->conf_.alpha_ = 255;

	//	conf_wrappers.push_back(rgba_layer_c0);

	//	// 第0层是摄像头测试
	//	/*auto camera_layer_c0 = std::make_shared<NT_CameraLayerConfigWrapper>(index++,
	//		0, 0, 640, 480);

	//		strcpy_s(camera_layer_c0->conf_.device_unique_id_utf8_, "\\\\?\\usb#vid_04f2&pid_b449&mi_00#7&29gaa8e6&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global");

	//		conf_wrappers.push_back(camera_layer_c0);*/
	//	

	//	// 第0层是屏幕测试
	//	/*auto screen_layer_c0 = std::make_shared<NT_ScreenLayerConfigWrapper>(index++,
	//		0, 0, 640, 480);

	//		screen_layer_c0->conf_.reserve_ = nullptr;
	//		screen_layer_c0->conf_.clip_region_.x_ = 0;
	//		screen_layer_c0->conf_.clip_region_.y_ = 0;
	//		screen_layer_c0->conf_.clip_region_.width_ = 640;
	//		screen_layer_c0->conf_.clip_region_.height_ = 480;

	//		conf_wrappers.push_back(screen_layer_c0);*/


	//	auto screen_layer_c1 = std::make_shared<NT_ScreenLayerConfigWrapper>(index++,
	//		200, 220, 400, 200);

	//	screen_layer_c1->conf_.reserve_ = nullptr;
	//	screen_layer_c1->conf_.clip_region_.x_ = 800;
	//	screen_layer_c1->conf_.clip_region_.y_ = 400;
	//	screen_layer_c1->conf_.clip_region_.width_ = 400;
	//	screen_layer_c1->conf_.clip_region_.height_ = 200;

	//	conf_wrappers.push_back(screen_layer_c1);

	//	// 摄像头层
	//	auto camera_layer_c1 = std::make_shared<NT_CameraLayerConfigWrapper>(index++,
	//		140, 0, 280, 210);

	//	strcpy_s(camera_layer_c1->conf_.device_unique_id_utf8_, "\\\\?\\usb#vid_04f2&pid_b449&mi_00#7&293bd9f6&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global");

	//	conf_wrappers.push_back(camera_layer_c1);

	//	// 摄像头层
	//	auto camera_layer_c2 = std::make_shared<NT_CameraLayerConfigWrapper>(index++,
	//		430, 0, 210, 160);

	//	strcpy_s(camera_layer_c2->conf_.device_unique_id_utf8_, "\\\\?\\usb#vid_046d&pid_082b&mi_00#7&2c5eeg1&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global");

	//	conf_wrappers.push_back(camera_layer_c2);

	//	// 摄像头层
	//	/*auto camera_layer_c3 = std::make_shared<NT_CameraLayerConfigWrapper>(index++,
	//		-530, -200, 640, 480);

	//		strcpy_s(camera_layer_c3->conf_.device_unique_id_utf8_, "\\\\?\\usb#vid_04f2&pid_b449&mi_00#7&127a28f6&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global");

	//		conf_wrappers.push_back(camera_layer_c3);*/


	//	auto rgba_layer_c1 = std::make_shared<NT_RGBARectangleLayerConfigWrapper>(index++,
	//		150, -10, 440, 420);

	//	rgba_layer_c1->conf_.red_	= 0;
	//	rgba_layer_c1->conf_.green_ = 80;
	//	rgba_layer_c1->conf_.blue_	= 159;
	//	rgba_layer_c1->conf_.alpha_ = 128;

	//	conf_wrappers.push_back(rgba_layer_c1);


	//	auto screen_layer_c2 = std::make_shared<NT_ScreenLayerConfigWrapper>(index++,
	//		0, 0, 640, 480);

	//	screen_layer_c2->conf_.reserve_ = nullptr;
	//	screen_layer_c2->conf_.clip_region_.x_ = 0;
	//	screen_layer_c2->conf_.clip_region_.y_ = 0;
	//	screen_layer_c2->conf_.clip_region_.width_ = 640;
	//	screen_layer_c2->conf_.clip_region_.height_ = 480;

	//	conf_wrappers.push_back(screen_layer_c2);

	//	auto camera_layer_c4 = std::make_shared<NT_CameraLayerConfigWrapper>(index++,
	//		0, 0, 640, 480);

	//	strcpy_s(camera_layer_c4->conf_.device_unique_id_utf8_, "\\\\?\\usb#vid_04f2&pid_b449&mi_00#7&87ba28f6&0&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\global");

	//	conf_wrappers.push_back(camera_layer_c4);

	//	{
	//		std::vector<const NT_PB_LayerBaseConfig* > confs;

	//		for (const auto& i : conf_wrappers)
	//		{
	//			ASSERT(i);
	//			confs.push_back(i->getBase());
	//		}

	//		publisher_api_.SetLayersConfig(publisher_handle_, 0, (const NT_PB_LayerBaseConfig**)confs.data(),
	//			confs.size(), 0, nullptr);
	//	}
	//	
	//}

	// 音频相关设置
	if ( BST_CHECKED == btn_check_auido_mic_input_.GetCheck() )
	{
		auto count = combox_auido_input_devices_.GetCount();
		if (count != CB_ERR && count > 0)
		{
			auto cur_sel = combox_auido_input_devices_.GetCurSel();
			if (cur_sel != CB_ERR)
			{
				publisher_api_.SetAuidoInputDeviceId(publisher_handle_, cur_sel);
			}
		}
	}

	if ( BST_CHECKED == btn_check_speex_encoder_.GetCheck())
	{
		publisher_api_.SetPublisherAudioCodecType(publisher_handle_, 2);

		publisher_api_.SetPublisherSpeexEncoderQuality(publisher_handle_, GetIntValueFromEdit(&edit_speex_quality_));
	}
	else
	{
		publisher_api_.SetPublisherAudioCodecType(publisher_handle_, 1);
	}

	if ( BST_CHECKED == btn_check_auido_mic_input_.GetCheck()
		|| BST_CHECKED == btn_check_auido_speaker_input_.GetCheck())
	{
		if (BST_CHECKED == btn_check_set_mute_.GetCheck())
		{
			publisher_api_.SetMute(publisher_handle_, 1);
		}
	}

	if ( BST_CHECKED == btn_check_echo_cancel_.GetCheck() )
	{
		publisher_api_.SetEchoCancellation(publisher_handle_, 1, GetIntValueFromEdit(&edit_echo_delay_));
	}
	else
	{
		publisher_api_.SetEchoCancellation(publisher_handle_, 0, 0);
	}

	if ( BST_CHECKED == btn_check_noise_suppression_.GetCheck() )
	{
		publisher_api_.SetNoiseSuppression(publisher_handle_, 1);
	}
	else
	{
		publisher_api_.SetNoiseSuppression(publisher_handle_, 0);
	}

	if ( BST_CHECKED == btn_check_agc_.GetCheck() )
	{
		publisher_api_.SetAGC(publisher_handle_, 1);
	}
	else
	{
		publisher_api_.SetAGC(publisher_handle_, 0);
	}

	if (BST_CHECKED == btn_check_vad_.GetCheck())
	{
		publisher_api_.SetVAD(publisher_handle_, 1);
	}
	else
	{
		publisher_api_.SetVAD(publisher_handle_, 0);
	}
}

void CSmartPublisherDemoDlg::FillCameraInfo()
{
	ASSERT(publisher_api_.GetVideoCaptureDeviceNumber != NULL);
	ASSERT(publisher_api_.GetVideoCaptureDeviceInfo != NULL);
	ASSERT(publisher_api_.GetVideoCaptureDeviceCapabilityNumber != NULL);
	ASSERT(publisher_api_.GetVideoCaptureDeviceCapability != NULL);

	NT_INT32 device_number = 0;
	if ( NT_ERC_OK != publisher_api_.GetVideoCaptureDeviceNumber(&device_number) )
	{
		return;
	}

	if (device_number < 1)
	{
		return;
	}

	for (auto i = 0; i < device_number; ++i)
	{
		CameraInfo info;

		char name[256] = { 0 };
		char id[1024] = { 0 };

		if ( NT_ERC_OK != publisher_api_.GetVideoCaptureDeviceInfo(i,
			name, sizeof(name) / sizeof(char),
			id, sizeof(id) / sizeof(char)) )
		{
			continue;
		}

		info.name_ = name;
		info.id_   = id;

		NT_INT32 capability_number = 0;
		if ( NT_ERC_OK != publisher_api_.GetVideoCaptureDeviceCapabilityNumber(
			id, &capability_number) )
		{
			continue;
		}

		auto is_failed = false;

		//capability_number = 1;

		for ( auto i = 0; i < capability_number; ++i )
		{
			NT_PB_VideoCaptureCapability capability = { 0 };

			if (NT_ERC_OK != publisher_api_.GetVideoCaptureDeviceCapability(
				id, i, &capability ) )
			{
				is_failed = true;
				break;
			}

			info.capabilities_.push_back(capability);
		}

		if ( !is_failed )
		{
			cameras_.push_back(info);
		}
	}
}

void CSmartPublisherDemoDlg::InitCameraControl()
{
	if ( cameras_.empty() )
	{
		return;
	}

	btn_check_camera_input_.EnableWindow(TRUE);
	
	cur_sel_camera_index_ = CB_ERR;
	combox_camera_list_.ResetContent();

	auto index = 0;
	for (const auto& i : cameras_)
	{
		++index;

		std::wstring w_name = ConvertToWString(i.name_);

		std::wostringstream ss;
		ss << w_name 
		   << L"-"
		   << index;

		combox_camera_list_.AddString(ss.str().c_str());
	}

	combox_camera_list_.SetCurSel(0);
	cur_sel_camera_index_ = combox_camera_list_.GetCurSel();

	ReFillCameraResolutionsCombox();
	ReFillCameraFrameRateCombox();
}

void CSmartPublisherDemoDlg::ReFillCameraResolutionsCombox()
{
	cur_sel_camera_resolutions_index_ = CB_ERR;
	combox_camera_resolutions_.ResetContent();

	if ( cur_sel_camera_index_ < 0
		|| cur_sel_camera_index_ >= static_cast<int>(cameras_.size()) )
	{
		return;
	}

	auto& camera = cameras_[cur_sel_camera_index_];

	for ( const auto& cap : camera.capabilities_ )
	{
		std::wostringstream ss;
		ss << cap.width_
			<< L" x "
			<< cap.height_;

		combox_camera_resolutions_.AddString(ss.str().c_str());
	}

	auto res_count = combox_camera_resolutions_.GetCount();
	if ( res_count != CB_ERR && res_count > 0 )
	{
		int index = 0;
		GetDefaultCameraCapabilityIndex(camera, index);

		if ( index < res_count )
		{
			combox_camera_resolutions_.SetCurSel(index);
			cur_sel_camera_resolutions_index_ = combox_camera_resolutions_.GetCurSel();
		}
	}
}

void CSmartPublisherDemoDlg::ReFillCameraFrameRateCombox()
{
	cur_sel_camera_frame_rate_index_ = CB_ERR;
	combox_camera_frame_rate_.ResetContent();

	if ( cur_sel_camera_index_ < 0
		|| cur_sel_camera_index_ >= static_cast<int>(cameras_.size()) )
	{
		return;
	}

	auto& camera = cameras_[cur_sel_camera_index_];

	if ( cur_sel_camera_resolutions_index_ < 0
		|| cur_sel_camera_resolutions_index_ >= static_cast<int>(camera.capabilities_.size()) )
	{
		return;
	}

	NT_PB_VideoCaptureCapability* pDefCapability = &camera.capabilities_[cur_sel_camera_resolutions_index_];

	for ( auto i = 0; i < pDefCapability->max_frame_rate_; ++i )
	{
		std::wostringstream ss;
		ss << (i + 1);
		combox_camera_frame_rate_.AddString(ss.str().c_str());
	}

	auto frame_count = combox_camera_frame_rate_.GetCount();
	if ( frame_count != CB_ERR && frame_count > 0 )
	{
		auto half = frame_count / 2;
		if ( half > 0 )
		{
			half -= 1;
		}

		combox_camera_frame_rate_.SetCurSel(half);
		cur_sel_camera_frame_rate_index_ = combox_camera_frame_rate_.GetCurSel();
	}
}

void CSmartPublisherDemoDlg::GetDefaultCameraCapabilityIndex(const CameraInfo& camera_info, int& index)
{
	index = 0;

	int i = 0;
	for (const auto& c : camera_info.capabilities_)
	{
		if (c.width_ == 640
			&& c.height_ == 480 )
		{
			index = i;
			return;
		}

		++i;
	}

	int area = 700 * 600;

	i = 0;
	for (const auto& c : camera_info.capabilities_)
	{
		if ( (c.width_ * c.height_) <= area )
		{
			index = i;
			return;
		}

		++i;
	}
}

void CSmartPublisherDemoDlg::OnCbnSelchangeComboCameras()
{
	auto sel = combox_camera_list_.GetCurSel();
	if (cur_sel_camera_index_ == sel)
	{
		return;
	}

	cur_sel_camera_index_ = sel;
	if (CB_ERR == cur_sel_camera_index_)
	{
		return;
	}

	ReFillCameraResolutionsCombox();
	ReFillCameraFrameRateCombox();
}

void CSmartPublisherDemoDlg::OnCbnSelchangeComboCameraResolutions()
{
	auto sel = combox_camera_resolutions_.GetCurSel();
	if ( cur_sel_camera_resolutions_index_ == sel )
	{
		return;
	}

	cur_sel_camera_resolutions_index_ = sel;
	if ( CB_ERR == cur_sel_camera_resolutions_index_ )
	{
		return;
	}

	ReFillCameraFrameRateCombox();
}


void CSmartPublisherDemoDlg::OnCbnSelchangeComboCameraFrameRate()
{
	cur_sel_camera_frame_rate_index_ = combox_camera_frame_rate_.GetCurSel();
}

void CSmartPublisherDemoDlg::OnBnClickedCheckFlipVerticalCamera()
{
	auto is_flip = BST_CHECKED == btn_check_flip_vertical_camera_.GetCheck();

	if ( publisher_handle_ != NULL )
	{
		if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
			|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck()
			|| BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck() )
		{
			for ( auto& i : layer_conf_wrappers_ )
			{
				if ( NT_PB_E_LAYER_TYPE_CAMERA == i->getBase()->type_ )
				{
					auto camera_layer_conf = std::dynamic_pointer_cast<nt_pb_sdk::CameraLayerConfigWrapperV2>(i);
					if ( camera_layer_conf )
					{
						camera_layer_conf->conf_.is_flip_vertical_ = is_flip ? 1 : 0;

						publisher_api_.UpdateLayerConfig(publisher_handle_, 0,
							camera_layer_conf->getBase(), 0, nullptr);
					}

					break;
				}
			}
		}
		else if ( BST_CHECKED == btn_check_camera_input_.GetCheck() )
		{
			publisher_api_.FlipVerticalCamera(publisher_handle_, is_flip? 1 : 0);
		}
	}

	CString btn_camera_preview_name;
	btn_camera_preview_.GetWindowTextW(btn_camera_preview_name);
	if ( btn_camera_preview_name != L"预览" )
	{
		publisher_api_.FlipVerticalCameraPreview(camera_preview_wnd_.m_hWnd, is_flip ? 1 : 0);
	}
}


void CSmartPublisherDemoDlg::OnBnClickedCheckFlipHorizontalcamera()
{
	auto is_flip = BST_CHECKED == btn_check_flip_horizontal_camera_.GetCheck();

	if ( publisher_handle_ != NULL )
	{
		if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
			|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck()
			|| BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck() )
		{
			for (auto& i : layer_conf_wrappers_)
			{
				if (NT_PB_E_LAYER_TYPE_CAMERA == i->getBase()->type_)
				{
					auto camera_layer_conf = std::dynamic_pointer_cast<nt_pb_sdk::CameraLayerConfigWrapperV2>(i);
					if (camera_layer_conf)
					{
						camera_layer_conf->conf_.is_flip_horizontal_ = is_flip ? 1 : 0;

						publisher_api_.UpdateLayerConfig(publisher_handle_, 0,
							camera_layer_conf->getBase(), 0, nullptr);
					}

					break;
				}
			}
		}
		else if ( BST_CHECKED == btn_check_camera_input_.GetCheck() )
		{
			publisher_api_.FlipHorizontalCamera(publisher_handle_, is_flip ? 1 : 0);
		}
	}

	CString btn_camera_preview_name;
	btn_camera_preview_.GetWindowTextW(btn_camera_preview_name);
	if ( btn_camera_preview_name != L"预览")
	{
		publisher_api_.FlipHorizontalCameraPreview(camera_preview_wnd_.m_hWnd, is_flip ? 1 : 0);
	}
}


void CSmartPublisherDemoDlg::OnBnClickedButtonRotateCamera()
{
	auto degress = GetCameraRotateDegress();
	degress += 90;
	degress %= 360;

	SetCameraRotateText(degress);

	if (publisher_handle_ != NULL)
	{
		if (BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
			|| BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck())
		{

		}
		else if (BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck())
		{
			for (auto& i : layer_conf_wrappers_)
			{
				if (NT_PB_E_LAYER_TYPE_CAMERA == i->getBase()->type_)
				{
					auto camera_layer_conf = std::dynamic_pointer_cast<nt_pb_sdk::CameraLayerConfigWrapperV2>(i);
					if ( camera_layer_conf )
					{
						camera_layer_conf->conf_.rotate_degress_ = degress;

						publisher_api_.UpdateLayerConfig(publisher_handle_, 0,
							camera_layer_conf->getBase(), 0, nullptr);
					}

					break;
				}
			}
		}
		else if (BST_CHECKED == btn_check_camera_input_.GetCheck())
		{
			publisher_api_.RotateCamera(publisher_handle_, degress);
		}
	}

	CString btn_camera_preview_name;
	btn_camera_preview_.GetWindowTextW(btn_camera_preview_name);
	if ( btn_camera_preview_name != L"预览" )
	{
		publisher_api_.RotateCameraPreview(camera_preview_wnd_.m_hWnd, degress);
	}
}


void CSmartPublisherDemoDlg::OnBnClickedCheckDesktopInput()
{
	if ( BST_CHECKED == btn_check_desktop_input_.GetCheck() )
	{
		btn_check_window_input_.SetCheck(BST_UNCHECKED);

		if ( BST_CHECKED == btn_check_camera_input_.GetCheck())
		{
			btn_check_camera_input_.SetCheck(BST_UNCHECKED);
			combox_camera_list_.EnableWindow(FALSE);
			combox_camera_resolutions_.EnableWindow(FALSE);
			combox_camera_frame_rate_.EnableWindow(FALSE);

			btn_camera_preview_.EnableWindow(FALSE);

			StopCameraPreview();
		}
	}	
}

void CSmartPublisherDemoDlg::OnBnClickedCheckWindowInput()
{
	if ( BST_CHECKED == btn_check_window_input_.GetCheck() )
	{
		btn_check_desktop_input_.SetCheck(BST_UNCHECKED);

		if (BST_CHECKED == btn_check_camera_input_.GetCheck())
		{
			btn_check_camera_input_.SetCheck(BST_UNCHECKED);

			combox_camera_list_.EnableWindow(FALSE);
			combox_camera_resolutions_.EnableWindow(FALSE);
			combox_camera_frame_rate_.EnableWindow(FALSE);

			btn_camera_preview_.EnableWindow(FALSE);

			StopCameraPreview();
		}

		RefreshWindowList();
	}
}

void CSmartPublisherDemoDlg::OnCbnDropdownComboWindowList()
{
	RefreshWindowList();
}

void CSmartPublisherDemoDlg::OnCbnSelchangeComboWindowList()
{
	auto old_sel_window = cur_sel_capture_window_;

	cur_sel_capture_window_ = nullptr;

	auto sel_index = combox_window_list_.GetCurSel();

	if ( CB_ERR != sel_index && sel_index >= 0 )
	{
		if ( sel_index < static_cast<int>(capture_windows_.size()) )
		{
			cur_sel_capture_window_ = capture_windows_[sel_index].hwnd_;
		}
	}

	if ( publisher_handle_ != nullptr )
	{
		if ( BST_CHECKED == btn_check_window_input_.GetCheck() )
		{
			if ( old_sel_window != cur_sel_capture_window_ )
			{
				if ( cur_sel_capture_window_ != nullptr )
				{
					publisher_api_.SetCaptureWindow(publisher_handle_, cur_sel_capture_window_);

					// 窗口层修改演示代码++

					/*for (auto& i : layer_conf_wrappers_)
					{
					if (NT_PB_E_LAYER_TYPE_WINDOW == i->getBase()->type_)
					{
					auto window_layer_conf = std::dynamic_pointer_cast<nt_pb_sdk::WindowLayerConfigWrapper>(i);
					if ( window_layer_conf )
					{
					window_layer_conf->conf_.window_ = cur_sel_capture_window_;

					publisher_api_.UpdateLayerConfig(publisher_handle_, 0,
					window_layer_conf->getBase(), 0, nullptr);
					}

					break;
					}
					}*/

					// 窗口层修改演示代码--

				}
				else
				{
					AfxMessageBox(_T("请下拉选择一个有效的采集窗口"));
				}
			}
		}
	}
}


void CSmartPublisherDemoDlg::OnBnClickedCheckcameraInput()
{
	if ( BST_CHECKED == btn_check_camera_input_.GetCheck() )
	{
		btn_check_desktop_input_.SetCheck(BST_UNCHECKED);
		btn_check_window_input_.SetCheck(BST_UNCHECKED);

		combox_camera_list_.EnableWindow(TRUE);
		combox_camera_resolutions_.EnableWindow(TRUE);
		combox_camera_frame_rate_.EnableWindow(TRUE);

		btn_camera_preview_.EnableWindow(TRUE);
	}
	else
	{
		combox_camera_list_.EnableWindow(FALSE);
		combox_camera_resolutions_.EnableWindow(FALSE);
		combox_camera_frame_rate_.EnableWindow(FALSE);

		btn_camera_preview_.EnableWindow(FALSE);

		StopCameraPreview();
	}
}


void CSmartPublisherDemoDlg::OnBnClickedButtonCameraPreview()
{
	CString name;
	btn_camera_preview_.GetWindowTextW(name);

	if (name == L"预览")
	{
		if (BST_CHECKED == btn_check_camera_input_.GetCheck())
		{
			if (CB_ERR != cur_sel_camera_index_ )
			{
				auto& camera = cameras_[cur_sel_camera_index_];
				
				/*publisher_api_.StartVideoCaptureDevicePreview(camera.id_.c_str(),
					camera_preview_wnd2_.m_hWnd);*/

				if ( NT_ERC_OK == publisher_api_.StartVideoCaptureDevicePreview(camera.id_.c_str(),
					camera_preview_wnd_.m_hWnd))
				{
					btn_camera_preview_.SetWindowTextW(L"停止预览");

					publisher_api_.FlipVerticalCameraPreview(camera_preview_wnd_.m_hWnd, 
						BST_CHECKED == btn_check_flip_vertical_camera_.GetCheck() ? 1 : 0);

					publisher_api_.FlipHorizontalCameraPreview(camera_preview_wnd_.m_hWnd,
						BST_CHECKED == btn_check_flip_horizontal_camera_.GetCheck() ? 1 : 0);

					publisher_api_.RotateCameraPreview(camera_preview_wnd_.m_hWnd, GetCameraRotateDegress());
				}
			}
		}
	}
	else
	{
		//publisher_api_.StopVideoCaptureDevicePreview(camera_preview_wnd2_.m_hWnd);

		StopCameraPreview();
	}
}


void CSmartPublisherDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	/*if (::IsWindow(btn_camera_preview_.m_hWnd))
	{
	CString text;
	btn_camera_preview_.GetWindowTextW(text);
	if (text == L"停止预览")
	{
	auto rect = GetCameraPreviewRect();
	camera_preview_wnd_.MoveWindow(rect);
	publisher_api_.VideoCaptureDevicePreviewWindowSizeChanged(camera_preview_wnd_.m_hWnd);
	}
	}*/
	
	
	// TODO: Add your message handler code here
}

CRect CSmartPublisherDemoDlg::GetCameraPreviewRect()
{
	CRect rect(0, 0, 0, 0);

	if (!::IsWindow(btn_camera_preview_.m_hWnd))
	{
		return rect;
	}

	CRect btn_rect(0, 0, 0, 0);
	btn_camera_preview_.GetWindowRect(&btn_rect);
	ScreenToClient(btn_rect);

	CRect clientRect(0, 0, 0, 0);
	if ( !::IsWindow(m_hWnd) )
		return rect;

	GetClientRect(&clientRect);

	if ( clientRect.right < (btn_rect.right + 30))
		return rect;


	rect.left   = btn_rect.right + 2;
	rect.bottom = btn_rect.bottom + 2;

	rect.right = rect.left + 140;
	rect.top = rect.bottom - static_cast<int>(140/1.333);

	if (rect.right > clientRect.right)
		rect.right = clientRect.right;

	return rect;
}

void CSmartPublisherDemoDlg::StopCameraPreview()
{
	CString btn_camera_preview_name;
	btn_camera_preview_.GetWindowTextW(btn_camera_preview_name);
	if (btn_camera_preview_name != L"预览")
	{
		publisher_api_.StopVideoCaptureDevicePreview(camera_preview_wnd_.m_hWnd);
		btn_camera_preview_.SetWindowTextW(L"预览");
	}
}

void CSmartPublisherDemoDlg::OnBnClickedButtonChooseScreenRegion()
{
	if ( region_choose_tool_handle_ == NULL )
	{
		if (publisher_handle_ == NULL)
		{
			ShowWindow(SW_HIDE);

			// 隐藏window比较慢，这里加个延时，如果你觉着不好，可以启动一个定时器，延时一段时间在调用截屏接口, 或者不影藏
			::Sleep(500);
			
			// 没用推送或者录像的情况是选择区域
			auto ret = publisher_api_.OpenScreenRegionChooseTool(&region_choose_tool_handle_,
				NT_PB_E_SCREEN_REGION_CHOOSE_MODE_CHOOSE, NULL,
				&NT_PB_SDKScreenRegionChooseHandle,
				GetSafeHwnd(),
				0, NULL);

			if (NT_ERC_OK == ret)
			{

			}
			else
			{
				// 失败就显示出来
				ShowWindow(SW_SHOW);
			}
		}
		else
		{
			// 推送或者录像的情况，只能移动区域
			NT_PB_RectRegion rect = { 0 };
			rect.x_ = GetIntValueFromEdit(&edit_clip_left_);
			rect.y_ = GetIntValueFromEdit(&edit_clip_top_);
			rect.width_ = GetIntValueFromEdit(&edit_clip_width_);
			rect.height_ = GetIntValueFromEdit(&edit_clip_height_);

			if (rect.width_ < 1 || rect.height_ < 1)
			{
				return;
			}

			ShowWindow(SW_HIDE);

			// 隐藏window比较慢，这里加个延时，如果你觉着不好，可以启动一个定时器，延时一段时间在调用截屏接口, 或者不影藏
			::Sleep(500);

			auto ret = publisher_api_.OpenScreenRegionChooseTool(&region_choose_tool_handle_,
				NT_PB_E_SCREEN_REGION_CHOOSE_MODE_MOVE, &rect,
				&NT_PB_SDKScreenRegionChooseHandle,
				GetSafeHwnd(),
				0, NULL);

			if (NT_ERC_OK == ret)
			{

			}
			else if (NT_ERC_PB_INVALID_SCREEN_RECTREGION == ret)
			{
				ShowWindow(SW_SHOW);
			}
			else
			{
				ShowWindow(SW_SHOW);
			}
		}
	}
	else
	{
		publisher_api_.CloseScreenRegionChooseTool(region_choose_tool_handle_);
		region_choose_tool_handle_ = NULL;

		ShowWindow(SW_SHOW);
	}
}

LRESULT CSmartPublisherDemoDlg::OnSDKScreenRegionChooseHandle(WPARAM wParam, LPARAM lParam)
{
	NT_UINT32 msg_id = (NT_UINT32)(wParam);
	
	if ( NT_PB_E_SCREEN_REGION_CHOOSE_MSG_CANCEL == msg_id )
	{
		if (region_choose_tool_handle_ != NULL)
		{
			publisher_api_.CloseScreenRegionChooseTool(region_choose_tool_handle_);
			region_choose_tool_handle_ = NULL;
		}

		ShowWindow(SW_SHOW);
	}
	else if ( NT_PB_E_SCREEN_REGION_CHOOSE_MSG_OK == msg_id )
	{
		if ( region_choose_tool_handle_ != NULL)
		{
			publisher_api_.CloseScreenRegionChooseTool(region_choose_tool_handle_);
			region_choose_tool_handle_ = NULL;
		}

		std::unique_ptr<NT_PB_RectRegion> region((NT_PB_RectRegion*)(lParam));
		if ( region )
		{
			SetIntValueToEdit(&edit_clip_left_, region->x_);
			SetIntValueToEdit(&edit_clip_top_, region->y_);
			SetIntValueToEdit(&edit_clip_width_, region->width_);
			SetIntValueToEdit(&edit_clip_height_, region->height_);

			// 如果是推送h或者录像的话， 直接移动位置
			if ( publisher_handle_ != NULL )
			{
				if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
					|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck() )
				{
					// 找到屏幕层，进行移动
					for (auto& i : layer_conf_wrappers_)
					{
						if ( NT_PB_E_LAYER_TYPE_SCREEN == i->getBase()->type_ )
						{
							auto screen_layer_conf = std::dynamic_pointer_cast<nt_pb_sdk::ScreenLayerConfigWrapper>(i);
							if ( screen_layer_conf )
							{
								screen_layer_conf->conf_.clip_region_.x_ = region->x_;
								screen_layer_conf->conf_.clip_region_.y_ = region->y_;

								publisher_api_.UpdateLayerConfig(publisher_handle_, 0,
									screen_layer_conf->getBase(), 0, nullptr);
							}

							break;
						}
					}
				}
				else if ( BST_CHECKED == btn_check_desktop_input_.GetCheck() )
				{
					publisher_api_.MoveScreenClipRegion(publisher_handle_, region->x_, region->y_);
				}
			}
		}

		ShowWindow(SW_SHOW);
	}
	else if (NT_PB_E_SCREEN_REGION_CHOOSE_MSG_MOVE == msg_id)
	{
		std::unique_ptr<NT_PB_RectRegion> region((NT_PB_RectRegion*)(lParam));
		if (region)
		{
			SetIntValueToEdit(&edit_clip_left_, region->x_);
			SetIntValueToEdit(&edit_clip_top_, region->y_);
			SetIntValueToEdit(&edit_clip_width_, region->width_);
			SetIntValueToEdit(&edit_clip_height_, region->height_);

			// 如果是推送或者录像的话， 直接移动位置
			if (publisher_handle_ != NULL)
			{
				if (BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck()
					|| BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck())
				{
					// 找到屏幕层，进行移动
					for (auto& i : layer_conf_wrappers_)
					{
						if (NT_PB_E_LAYER_TYPE_SCREEN == i->getBase()->type_)
						{
							auto screen_layer_conf = std::dynamic_pointer_cast<nt_pb_sdk::ScreenLayerConfigWrapper>(i);
							if (screen_layer_conf)
							{
								screen_layer_conf->conf_.clip_region_.x_ = region->x_;
								screen_layer_conf->conf_.clip_region_.y_ = region->y_;

								publisher_api_.UpdateLayerConfig(publisher_handle_, 0,
									screen_layer_conf->getBase(), 0, nullptr);
							}

							break;
						}
					}
				}
				else if (BST_CHECKED == btn_check_desktop_input_.GetCheck())
				{
					publisher_api_.MoveScreenClipRegion(publisher_handle_, region->x_, region->y_);
				}
			}
		}
	}

	return S_OK;
}


void CSmartPublisherDemoDlg::OnBnClickedCheckDesktopCameraSwitch()
{
	if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck() )
	{
		btn_check_camera_overlay_to_desktop_.SetCheck(BST_UNCHECKED);
		btn_check_desktop_overlay_to_camera_.SetCheck(BST_UNCHECKED);

		btn_check_desktop_input_.SetCheck(BST_CHECKED);
		btn_check_desktop_input_.EnableWindow(FALSE);

		btn_check_camera_input_.SetCheck(BST_CHECKED);
		btn_check_camera_input_.EnableWindow(FALSE);

		combox_camera_list_.EnableWindow(TRUE);
		combox_camera_resolutions_.EnableWindow(TRUE);
		combox_camera_frame_rate_.EnableWindow(TRUE);

		btn_camera_preview_.EnableWindow(TRUE);

		btn_check_window_input_.SetCheck(BST_UNCHECKED);
		btn_check_window_input_.EnableWindow(FALSE);
	}
	else
	{
		btn_check_desktop_input_.SetCheck(BST_UNCHECKED);
		btn_check_desktop_input_.EnableWindow(TRUE);

		btn_check_camera_input_.SetCheck(BST_UNCHECKED);
		btn_check_camera_input_.EnableWindow(TRUE);

		combox_camera_list_.EnableWindow(FALSE);
		combox_camera_resolutions_.EnableWindow(FALSE);
		combox_camera_frame_rate_.EnableWindow(FALSE);

		btn_camera_preview_.EnableWindow(FALSE);

		btn_check_window_input_.EnableWindow(TRUE);

		StopCameraPreview();
	}
}


void CSmartPublisherDemoDlg::OnBnClickedCheckCameraOverlayToDesktop()
{
	if ( BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck() )
	{
		btn_check_desktop_camera_switch_.SetCheck(BST_UNCHECKED);
		btn_check_desktop_overlay_to_camera_.SetCheck(BST_UNCHECKED);

		btn_check_desktop_input_.SetCheck(BST_CHECKED);
		btn_check_desktop_input_.EnableWindow(FALSE);

		btn_check_camera_input_.SetCheck(BST_CHECKED);
		btn_check_camera_input_.EnableWindow(FALSE);

		combox_camera_list_.EnableWindow(TRUE);
		combox_camera_resolutions_.EnableWindow(TRUE);
		combox_camera_frame_rate_.EnableWindow(TRUE);

		btn_camera_preview_.EnableWindow(TRUE);

		btn_check_window_input_.SetCheck(BST_UNCHECKED);
		btn_check_window_input_.EnableWindow(FALSE);
	}
	else
	{
		btn_check_desktop_input_.SetCheck(BST_UNCHECKED);
		btn_check_desktop_input_.EnableWindow(TRUE);

		btn_check_camera_input_.SetCheck(BST_UNCHECKED);
		btn_check_camera_input_.EnableWindow(TRUE);

		combox_camera_list_.EnableWindow(FALSE);
		combox_camera_resolutions_.EnableWindow(FALSE);
		combox_camera_frame_rate_.EnableWindow(FALSE);

		btn_camera_preview_.EnableWindow(FALSE);

		btn_check_window_input_.EnableWindow(TRUE);

		StopCameraPreview();
	}
}


void CSmartPublisherDemoDlg::OnBnClickedCheckDesktopOverlayToCamera()
{
	if (BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck())
	{
		btn_check_desktop_camera_switch_.SetCheck(BST_UNCHECKED);
		btn_check_camera_overlay_to_desktop_.SetCheck(BST_UNCHECKED);
		
		btn_check_desktop_input_.SetCheck(BST_CHECKED);
		btn_check_desktop_input_.EnableWindow(FALSE);

		btn_check_camera_input_.SetCheck(BST_CHECKED);
		btn_check_camera_input_.EnableWindow(FALSE);

		combox_camera_list_.EnableWindow(TRUE);
		combox_camera_resolutions_.EnableWindow(TRUE);
		combox_camera_frame_rate_.EnableWindow(TRUE);

		btn_camera_preview_.EnableWindow(TRUE);

		btn_check_window_input_.SetCheck(BST_UNCHECKED);
		btn_check_window_input_.EnableWindow(FALSE);
	}
	else
	{
		btn_check_desktop_input_.SetCheck(BST_UNCHECKED);
		btn_check_desktop_input_.EnableWindow(TRUE);

		btn_check_camera_input_.SetCheck(BST_UNCHECKED);
		btn_check_camera_input_.EnableWindow(TRUE);

		combox_camera_list_.EnableWindow(FALSE);
		combox_camera_resolutions_.EnableWindow(FALSE);
		combox_camera_frame_rate_.EnableWindow(FALSE);

		btn_camera_preview_.EnableWindow(FALSE);

		btn_check_window_input_.EnableWindow(TRUE);

		StopCameraPreview();
	}
}


void CSmartPublisherDemoDlg::OnBnClickedButtonDesktopCameraSwitch()
{
	if ( BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck() )
	{
		if ( publisher_handle_ != NULL )
		{
			CString str_btn_text;
			btn_desktop_camera_switch_.GetWindowTextW(str_btn_text);

			if ( str_btn_text == L"切换到摄像头" )
			{
				// 启用摄像头层
				publisher_api_.EnableLayer(publisher_handle_, 0, 1, 1);

				// 禁用桌面层
				if ( NT_ERC_OK == publisher_api_.EnableLayer(publisher_handle_, 0,
					2, 0) )
				{
					btn_desktop_camera_switch_.SetWindowTextW(L"切换到屏幕");
				}
			}
			else
			{
				// 禁用摄像头层
				publisher_api_.EnableLayer(publisher_handle_, 0, 1, 0);

				// 启用桌面层
				if (NT_ERC_OK == publisher_api_.EnableLayer(publisher_handle_, 0,
					2, 1))
				{
					btn_desktop_camera_switch_.SetWindowTextW(L"切换到摄像头");
				}
			}			
		}
	}
}


void CSmartPublisherDemoDlg::OnBnClickedButtonDisableCameraOverlay()
{
	if ( BST_CHECKED == btn_check_camera_overlay_to_desktop_.GetCheck() )
	{
		if ( publisher_handle_ != NULL )
		{
			CString str_btn_text;
			btn_disable_camera_overlay_.GetWindowTextW(str_btn_text);

			if ( str_btn_text == L"停止叠加摄像头" )
			{
				if (NT_ERC_OK == publisher_api_.EnableLayer(publisher_handle_, 0,
					1, 0))
				{
					btn_disable_camera_overlay_.SetWindowTextW(L"叠加摄像头");
				}
			}
			else
			{
				if (NT_ERC_OK == publisher_api_.EnableLayer(publisher_handle_, 0,
					1, 1))
				{
					btn_disable_camera_overlay_.SetWindowTextW(L"停止叠加摄像头");
				}
			}
		}
	}
}


void CSmartPublisherDemoDlg::OnBnClickedButtonDisableDesktopOverlay()
{
	if ( BST_CHECKED == btn_check_desktop_overlay_to_camera_.GetCheck() )
	{
		if ( publisher_handle_ != NULL )
		{
			CString str_btn_text;
			btn_disable_desktop_overlay_.GetWindowTextW(str_btn_text);

			if ( str_btn_text == L"停止叠加屏幕" )
			{
				if (NT_ERC_OK == publisher_api_.EnableLayer(publisher_handle_, 0,
					1, 0))
				{
					btn_disable_desktop_overlay_.SetWindowTextW(L"叠加屏幕");
				}
			}
			else
			{
				if (NT_ERC_OK == publisher_api_.EnableLayer(publisher_handle_, 0,
					1, 1))
				{
					btn_disable_desktop_overlay_.SetWindowTextW(L"停止叠加屏幕");
				}
			}
		}
	}
}

bool CSmartPublisherDemoDlg::SetLayersConfig()
{
	if ( layer_conf_wrappers_.empty() )
	{
		return false;
	}

	std::vector<const NT_PB_LayerBaseConfig* > confs;
	for ( const auto& i : layer_conf_wrappers_ )
	{
		ASSERT(i);
		confs.push_back(i->getBase());
	}

	ASSERT(publisher_handle_ != nullptr);
	if (NT_ERC_OK == publisher_api_.SetLayersConfig(publisher_handle_, 0, confs.data(),
		confs.size(), 0, nullptr))
	{
		return true;
	}

	return false;
}

void CSmartPublisherDemoDlg::OnBnClickedButtonSetRbgaRectLayer()
{
	if (BST_CHECKED == btn_check_desktop_camera_switch_.GetCheck())
	{
		if (publisher_handle_ != NULL && !layer_conf_wrappers_.empty())
		{
			auto r = GetIntValueFromEdit(&edit_rgba_rect_layer_red_);
			r = ClipIntValue(r, 0, 255);

			auto g = GetIntValueFromEdit(&edit_rgba_rect_layer_green_);
			g = ClipIntValue(g, 0, 255);

			auto b = GetIntValueFromEdit(&edit_rgba_rect_layer_blue_);
			b = ClipIntValue(b, 0, 255);

			auto a = GetIntValueFromEdit(&edit_rgba_rect_layer_alpha_);
			a = ClipIntValue(a, 0, 255);

			std::shared_ptr<nt_pb_sdk::layer_conf_wrapper_base> layer_conf;

			for (auto iter = layer_conf_wrappers_.rbegin(); iter != layer_conf_wrappers_.rend(); ++iter)
			{
				ASSERT((*iter));
				if (NT_PB_E_LAYER_TYPE_RGBA_RECTANGLE == (*iter)->getBase()->type_)
				{
					layer_conf = (*iter);
					break;
				}
			}

			if (!layer_conf)
			{
				return;
			}

			if (layer_conf->getBase()->index_ == layer_conf_wrappers_.front()->getBase()->index_)
			{
				return;
			}

			publisher_api_.EnableLayer(publisher_handle_, 0, layer_conf->getBase()->index_,
				a != 0 ? 1 : 0);

			auto rgba_conf = std::dynamic_pointer_cast<nt_pb_sdk::RGBARectangleLayerConfigWrapper>(layer_conf);
			if (rgba_conf)
			{
				rgba_conf->conf_.red_ = r;
				rgba_conf->conf_.green_ = g;
				rgba_conf->conf_.blue_ = b;
				rgba_conf->conf_.alpha_ = a;

				publisher_api_.UpdateLayerConfig(publisher_handle_, 0, rgba_conf->getBase(), 0, nullptr);
			}
		}
	}
}


void CSmartPublisherDemoDlg::OnBnClickedButtonSettingCaptureImagePath()
{
	std::wstring    sel_dir;

	BROWSEINFO      bi;
	::ZeroMemory(&bi, sizeof(BROWSEINFO));

	bi.pidlRoot = 0;
	bi.lpszTitle = _T("请选择一个保存截图的目录(注意：最好是空目录，防止覆盖掉老文件!!)，截图文件将会保存到这个目录");
	bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	bi.lpfn = NULL;

	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&bi);
	if (lpidlBrowse != NULL)
	{
		TCHAR  folder_path[MAX_PATH] = { 0 };
		if (::SHGetPathFromIDList(lpidlBrowse, folder_path))
		{
			sel_dir = folder_path;
		}
	}

	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}

	if (!sel_dir.empty())
	{
		capture_image_path_ = sel_dir;
	}
}


void CSmartPublisherDemoDlg::OnBnClickedButtonCaptureImage()
{
	if (capture_image_path_.empty())
	{
		AfxMessageBox(_T("请先设置保存截图文件的目录! 点击截图左边的按钮设置!"));
		return;
	}

	if (publisher_handle_ == NULL)
	{
		return;
	}

	std::wostringstream ss;
	ss << capture_image_path_;

	if (capture_image_path_.back() != L'\\')
	{
		ss << L"\\";
	}

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);

	ss << L"SmartPub-"
		<< std::setfill(L'0') << std::setw(4) << sysTime.wYear
		<< std::setfill(L'0') << std::setw(2) << sysTime.wMonth
		<< std::setfill(L'0') << std::setw(2) << sysTime.wDay
		<< L"-"
		<< std::setfill(L'0') << std::setw(2) << sysTime.wHour
		<< std::setfill(L'0') << std::setw(2) << sysTime.wMinute
		<< std::setfill(L'0') << std::setw(2) << sysTime.wSecond;

	ss << L"-" << std::setfill(L'0') << std::setw(3) << sysTime.wMilliseconds
		<< L".png";

	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;

	auto val_str = conv.to_bytes(ss.str());

	auto ret = publisher_api_.CaptureImage(publisher_handle_, val_str.c_str(), NULL, &NT_PB_SDKCaptureImageHandle);
	if (NT_ERC_OK == ret)
	{
		// 发送截图请求成功
	}
	else if (NT_ERC_PB_TOO_MANY_CAPTURE_IMAGE_REQUESTS == ret)
	{
		// 通知用户延时
		OutputDebugStringA("Too many capture image requests!!!\r\n");
	}
	else
	{
		// 其他失败
	}
}

LRESULT CSmartPublisherDemoDlg::OnSDKCaptureWindowInvalid(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)(lParam);

	std::ostringstream ss;
	ss << "OnSDKCaptureWindowInvalid hwnd=" << hwnd << " \r\n";

	OutputDebugStringA(ss.str().c_str());

	if ( hwnd == cur_sel_capture_window_ 
		|| nullptr == cur_sel_capture_window_ 
		|| !::IsWindow(cur_sel_capture_window_) )
	{
		AfxMessageBox(_T("采集窗口已经关闭或者无效了，请下拉选择新的采集窗口"));

		RefreshWindowList();
	}

	return S_OK;
}

int CSmartPublisherDemoDlg::GetCameraRotateDegress()
{
	int degress = 0;

	if ( !::IsWindow(edit_camera_rotate_degress_.m_hWnd))
		return degress;

	CString rotate_text;

	edit_camera_rotate_degress_.GetWindowTextW(rotate_text);

	if ( _T("0度") == rotate_text )
	{
		degress = 0;
	}
	else if (_T("90度") == rotate_text)
	{
		degress = 90;
	}
	else if (_T("180度") == rotate_text)
	{
		degress = 180;
	}
	else if (_T("270度") == rotate_text)
	{
		degress = 270;
	}

	return degress;
}

void CSmartPublisherDemoDlg::SetCameraRotateText(int degress)
{
	degress = degress % 360;

	if (!::IsWindow(edit_camera_rotate_degress_.m_hWnd))
		return;

	if ( 0 == degress )
	{
		edit_camera_rotate_degress_.SetWindowText(_T("0度"));
	}
	else if (90 == degress)
	{
		edit_camera_rotate_degress_.SetWindowText(_T("90度"));
	}
	else if (180 == degress)
	{
		edit_camera_rotate_degress_.SetWindowText(_T("180度"));
	}
	else if (270 == degress)
	{
		edit_camera_rotate_degress_.SetWindowText(_T("270度"));
	}
}

struct NT_PB_Demo_WindowsEnumerationParam
{
	NT_SmartPublisherSDKAPI* publisher_api_;
	std::vector<WindowInfo> window_info_list_;
};

BOOL CALLBACK NT_PB_Demo_WindowsEnumerationPro(HWND hwnd, LPARAM param)
{
	NT_PB_Demo_WindowsEnumerationParam* wp = reinterpret_cast<NT_PB_Demo_WindowsEnumerationParam*>(param);

	ASSERT(wp != nullptr);
	ASSERT(wp->publisher_api_ != nullptr);

	if (NT_ERC_OK == wp->publisher_api_->CheckCapturerWindow(hwnd))
	{
		const size_t buffer_size = 512;
		WCHAR text[buffer_size];
		text[0] = 0;

		if (::GetWindowText(hwnd, text, buffer_size))
		{
			wp->window_info_list_.emplace_back(hwnd, text);
		}
	}

	return TRUE;
}

void CSmartPublisherDemoDlg::RefreshWindowList()
{
	combox_window_list_.ResetContent();
	capture_windows_.clear();

	NT_PB_Demo_WindowsEnumerationParam param;
	param.publisher_api_ = &publisher_api_;

	param.window_info_list_.emplace_back(nullptr, L"请下拉选择采集窗口");

	if (EnumWindows(&NT_PB_Demo_WindowsEnumerationPro, (LPARAM)&param))
	{
		capture_windows_.swap(param.window_info_list_);

		for (const auto& i : capture_windows_)
		{
			combox_window_list_.AddString(i.text_.c_str());
		}
	}

	auto is_exist = false;
	auto sel_index = 0;

	for ( auto& i : capture_windows_ )
	{
		if (cur_sel_capture_window_ == i.hwnd_)
		{
			is_exist = true;
			break;;
		}

		sel_index++;
	}

	if ( is_exist )
	{
		if (sel_index < combox_window_list_.GetCount())
		{
			combox_window_list_.SetCurSel(sel_index);
		}
	}
	else
	{
		cur_sel_capture_window_ = nullptr;
		if (combox_window_list_.GetCount() > 0)
		{
			combox_window_list_.SetCurSel(0);
		}
	}
}

void CSmartPublisherDemoDlg::FillBitrateControlDefValue()
{
	int kbit_rate = 0;
	int max_kbit_rate = 0;

	CalDefBitrate(kbit_rate, max_kbit_rate);

	if (BST_CHECKED == btn_check_video_bitrate_.GetCheck())
	{
		edit_bit_rate_.SetWindowTextW(GetIntString(kbit_rate).c_str());
		edit_video_max_bitrate_.SetWindowTextW(GetIntString(max_kbit_rate).c_str());
	}
	else
	{
		edit_bit_rate_.SetWindowTextW(_T("0"));
		edit_video_max_bitrate_.SetWindowTextW(GetIntString(max_kbit_rate).c_str());
	}

	edit_video_quality_.SetWindowTextW(GetIntString(DEF_VIDEO_QUALITY).c_str());

	combox_h264_profile_.SetCurSel(0);

	edit_video_encode_speed_.SetWindowTextW(_T("3"));
}



void CSmartPublisherDemoDlg::OnBnClickedMfclink1() {
	// TODO: 在此添加控件通知处理程序代码
}
