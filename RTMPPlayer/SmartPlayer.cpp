/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/
// SmartPlayer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SmartPlayer.h"
#include "SmartPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmartPlayerApp

BEGIN_MESSAGE_MAP(CSmartPlayerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSmartPlayerApp ����

CSmartPlayerApp::CSmartPlayerApp() {
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSmartPlayerApp ����

CSmartPlayerApp theApp;


// CSmartPlayerApp ��ʼ��

BOOL CSmartPlayerApp::InitInstance() {
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	std::wstring role = __wargv[1];

	if (role == L"player") {
		CSmartPlayerDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == -1) {
			TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
			TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
		}
	} else {
		// Simply run a audio-only player in the console
		SmartPlayerSDKAPI api;
		NT_HANDLE hPlayer;
		GetSmartPlayerSDKAPI(&api);
		api.Init(0, NULL);
		api.Open(&hPlayer, NULL, 0, NULL);

		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		api.SetBuffer(hPlayer, 100);
		api.SetFastStartup(hPlayer, 1);
		api.SetReportDownloadSpeed(hPlayer, 1, 1);
		CString wURL = __wargv[2];
		auto url = conv.to_bytes(wURL);
		if (NT_ERC_OK != api.SetURL(hPlayer, url.c_str())) {
			return false;
		}
		api.SetLowLatencyMode(hPlayer, 1);

		api.StartPlay(hPlayer);

		WaitMessage();

		api.StopPlay(hPlayer);
		api.Close(hPlayer);
		api.UnInit();
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

