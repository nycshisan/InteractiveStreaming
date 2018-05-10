/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/
// SmartPlayer.cpp : 定义应用程序的类行为。
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


// CSmartPlayerApp 构造

CSmartPlayerApp::CSmartPlayerApp() {
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSmartPlayerApp 对象

CSmartPlayerApp theApp;


// CSmartPlayerApp 初始化

BOOL CSmartPlayerApp::InitInstance() {
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	std::wstring role = __wargv[1];

	if (role == L"player") {
		CSmartPlayerDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == -1) {
			TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
			TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
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

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

