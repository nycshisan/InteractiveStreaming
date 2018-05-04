/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/


// SmartPublisherDemoDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

#include "nt_layer_conf_wrapper.h"

class CameraInfo
{
public:
	std::string name_;
	std::string id_;
	std::vector<NT_PB_VideoCaptureCapability> capabilities_;
};

struct WindowInfo
{
	WindowInfo(HWND hwnd, const std::wstring& text)
		:hwnd_(hwnd), text_(text)
	{	
	}

	WindowInfo(WindowInfo&& r)
		:hwnd_(r.hwnd_),
		text_(std::move(r.text_))
	{
	}

	HWND hwnd_;
	std::wstring text_;
};


// CSmartPublisherDemoDlg dialog
class CSmartPublisherDemoDlg : public CDialogEx
{
// Construction
public:
	CSmartPublisherDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SMARTPUBLISHERDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedButtonPublish();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheckVideoBitrate();
	afx_msg void OnBnClickedCheckVideoSameIamge();
	afx_msg void OnBnClickedButtonBitrateControlDefValue();
	afx_msg void OnBnClickedButtonBitrateControlDefValue2();
	afx_msg void OnBnClickedButtonBitrateControlDefValue3();
	afx_msg void OnBnClickedCheckSetMute();
	afx_msg void OnBnClickedCheckAudioMicInput();
	afx_msg void OnBnClickedCheckAuidoSpeakerInput();
	afx_msg void OnBnClickedButtonChooseRecDir();
	afx_msg void OnBnClickedButtonRec();
	afx_msg void OnBnClickedButtonStopRec();
	afx_msg void OnCbnSelchangeComboCameras();
	afx_msg void OnCbnSelchangeComboCameraResolutions();
	afx_msg void OnCbnSelchangeComboCameraFrameRate();
	afx_msg void OnBnClickedCheckFlipVerticalCamera();
	afx_msg void OnBnClickedCheckFlipHorizontalcamera();
	afx_msg void OnBnClickedButtonRotateCamera();
	afx_msg void OnBnClickedCheckDesktopInput(); 
	afx_msg void OnBnClickedCheckWindowInput();
	afx_msg void OnCbnDropdownComboWindowList();
	afx_msg void OnCbnSelchangeComboWindowList();
	afx_msg void OnBnClickedCheckcameraInput();
	afx_msg void OnBnClickedButtonCameraPreview();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonChooseScreenRegion();
	afx_msg LRESULT OnSDKScreenRegionChooseHandle(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheckDesktopCameraSwitch();
	afx_msg void OnBnClickedCheckCameraOverlayToDesktop();
	afx_msg void OnBnClickedCheckDesktopOverlayToCamera();
	afx_msg void OnBnClickedButtonDesktopCameraSwitch();
	afx_msg void OnBnClickedButtonDisableCameraOverlay();
	afx_msg void OnBnClickedButtonDisableDesktopOverlay();
	afx_msg void OnBnClickedButtonSetRbgaRectLayer();
	afx_msg void OnBnClickedButtonAddImageWatermark();
	afx_msg void OnBnClickedButtonDisableImageWatermark();
	afx_msg void OnBnClickedButtonSettingCaptureImagePath();
	afx_msg void OnBnClickedButtonCaptureImage();
	afx_msg LRESULT OnSDKCaptureWindowInvalid(WPARAM wParam, LPARAM lParam);

private:
	enum 
	{
		DEF_FRAME_RATE = 5,
		DEF_KEY_FRAME = 10,
		DEF_VIDEO_QUALITY = 10,
	};

private:
	void SetIntValueToEdit(CEdit* pEdit, int val);
	int GetIntValueFromEdit(CEdit* pEdit);
	int ClipIntValue(int val, int min, int max);
	std::string GetStringValueFromEdit(CEdit* pEdit);
	std::wstring GetIntString(int val);
	std::wstring ConvertToWString(const std::string& val);

	int  CalMaxKBitRate(int frame_rate, int w, int h);
	int  CalBitRate(int frame_rate, int w, int h);
	void CalDefBitrate(int& kbit_rate, int& max_kbit_rate);
	void FillBitrateGroupItem(NT_HANDLE publisher_handle, int frame_rate, int w, int h);
	void SetBitrateGroup(NT_HANDLE publisher_handle, int frame_rate);
	bool IsCanCaptureSpeaker();
	void DisableAuidoInputControl();
	void EnableAuidoInputControl();
	bool OpenPublisherHandle();
	void SetCommonOptionToPublisherSDK();
	void FillCameraInfo();
	void InitCameraControl();
	void GetDefaultCameraCapabilityIndex(const CameraInfo& camera_info, int& index);
	void ReFillCameraResolutionsCombox();
	void ReFillCameraFrameRateCombox();
	CRect GetCameraPreviewRect();
	void StopCameraPreview();
	bool SetLayersConfig();
	int  GetCameraRotateDegress();
	void SetCameraRotateText(int deress);
	void RefreshWindowList();
	void FillBitrateControlDefValue();

private:
	CEdit edit_pb_qq1;
	CEdit edit_pb_qq2_;

	CButton btn_check_desktop_camera_switch_; 
	CButton btn_check_camera_overlay_to_desktop_;
	CButton btn_check_desktop_overlay_to_camera_;
	CButton btn_desktop_camera_switch_;
	CButton btn_add_image_watermark_;
	CButton btn_disable_image_watermark_;

	CButton btn_disable_camera_overlay_;
	CButton btn_disable_desktop_overlay_;

	CEdit edit_rgba_rect_layer_red_;
	CEdit edit_rgba_rect_layer_green_;
	CEdit edit_rgba_rect_layer_blue_;
	CEdit edit_rgba_rect_layer_alpha_;

	CEdit edit_camera_overlay_left_;
	CEdit edit_camera_overlay_top_;
	CEdit edit_camera_overlay_width_;
	CEdit edit_camera_overlay_height_;

	// 桌面设置相关
	CButton btn_check_desktop_input_;
	CEdit   edit_frame_rate_;
	CButton btn_choose_screen_region_;
	CButton btn_check_dxgi_screen_capturer_;
	CButton btn_check_capturer_disable_aero_;

	CEdit edit_clip_left_;
	CEdit edit_clip_top_;
	CEdit edit_clip_width_;
	CEdit edit_clip_height_;

	// 采集窗口
	CButton btn_check_window_input_;
	CComboBox combox_window_list_;

	// 摄像头相关
	CButton btn_check_camera_input_;
	CComboBox combox_camera_list_;
	CComboBox combox_camera_resolutions_;
	CComboBox combox_camera_frame_rate_;
	CButton btn_check_flip_vertical_camera_;
	CButton btn_check_flip_horizontal_camera_;
	CButton btn_rotate_camera_;
	CEdit edit_camera_rotate_degress_;
	CButton btn_camera_preview_;
	CWnd camera_preview_wnd_;
	//CWnd camera_preview_wnd2_;


	CEdit edit_bit_rate_;
	CEdit edit_key_frame_;

	CButton btn_check_video_bitrate_;
	CEdit edit_video_quality_;
	CEdit edit_video_max_bitrate_;
	CComboBox combox_h264_profile_;
	CEdit edit_video_encode_speed_;
	CButton btn_check_same_image_;
	CEdit   edit_max_video_frame_interval_;
	CEdit   edit_max_video_key_frame_interval_;

	// 音频相关设置
	CButton btn_check_auido_mic_input_;
	CButton btn_check_auido_speaker_input_;
	CComboBox combox_auido_input_devices_;
	CButton btn_check_set_mute_;
	CButton btn_check_echo_cancel_;
	CEdit   edit_echo_delay_;
	CButton btn_check_speex_encoder_;
	CEdit   edit_speex_quality_;
	CButton btn_check_noise_suppression_;
	CButton btn_check_agc_;
	CButton btn_check_vad_;

	CEdit edit_publish_url1_;
	CEdit edit_publish_url2_;
	CEdit edit_publish_url3_;
	CEdit edit_gen_url_;

	CButton btn_publish_;
	CButton btn_stop_publish_;

	CButton btn_rec_;
	CButton btn_stop_rec_;

	CEdit	edit_rec_dir_;
	CEdit	edit_file_name_prefix_;
	CEdit   edit_rec_max_file_size_;
	CButton btn_check_rec_append_date_;
	CButton btn_check_append_time_;

	CEdit edit_note_;

private:
	std::vector<CameraInfo> cameras_;
	int cur_sel_camera_index_;
	int cur_sel_camera_resolutions_index_;
	int cur_sel_camera_frame_rate_index_;

private:
	std::vector<WindowInfo> capture_windows_;
	HWND cur_sel_capture_window_;
	
private:
	std::vector<std::shared_ptr<nt_pb_sdk::layer_conf_wrapper_base> > layer_conf_wrappers_;

	// 图片层相关参数
	std::string image_layer_file_name_utf8_;
	int image_layer_left_;
	int image_layer_top_;
	int image_layer_width_;
	int image_layer_height_;

private:
	// 截图
	std::wstring capture_image_path_;

private:
	NT_SmartPublisherSDKAPI publisher_api_;
	NT_HANDLE publisher_handle_;
	int publisher_handle_count_;

	NT_HANDLE region_choose_tool_handle_;
public:
	afx_msg void OnBnClickedMfclink1();
};
