/*
* Author:YIENIJAN
* Copyright (C) 2015-2018 YIENIJAN. All rights reserved.
*
*/
// SmartPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include <string>
#include "smart_player_sdk.h"
#include "nt_render_wnd.h"
#include "nt_wrapper_render_wnd.h"


// CSmartPlayerDlg �Ի���
class CSmartPlayerDlg : public CDialogEx {
	// ����
public:
	CSmartPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

											// �Ի�������
	enum { IDD = IDD_SMARTPLAYER_DIALOG };

	enum { IDT_TIMER_SP_SDK_POS_DRIVER = 311, };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


														// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnSDKEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSDKVideoSize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSDKStartPlay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSDKRGB32Image(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheckMute();
	afx_msg void OnBnClickedButtonFullScreen();

private:
	bool InitCommonSDKParam();
	void StopPlayback();

private:
	CEdit edit_url;

	CButton btn_play_;

	CButton btn_full_screen_;

	nt_wrapper_render_wnd wrapper_render_wnd_;

	SmartPlayerSDKAPI player_api_;
	NT_HANDLE player_handle_;

	int width_;
	int height_;

	bool is_gdi_render_;
	bool is_playing_;

	CButton btn_mute_;

	CString   base_title_;
	NT_UINT32 connection_status_;
	NT_UINT32 buffer_status_;
	NT_INT32  buffer_percent_;
	NT_INT32  download_speed_;
};
