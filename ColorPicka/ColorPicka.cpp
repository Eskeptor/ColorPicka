﻿#include "pch.h"
#include "framework.h"
#include "ColorPicka.h"
#include "ColorPickaDlg.h"
#include "CppUtil/CppUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CColorPickaApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CColorPickaApp::CColorPickaApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


CColorPickaApp theApp;


BOOL CColorPickaApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("ColorPicka"));

	InitOptions();

	CColorPickaDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}
	SaveOptions();

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}


/**
Initialize Option
@access		private
@param
@return		true / false
*/
bool CColorPickaApp::InitOptions()
{
	CString strIniFileName = _T("");
	strIniFileName.Format(_T("%s%s.ini"), CppUtil::GetExePath().GetString(), CppUtil::GetExeName(false).GetString());

	if (CppUtil::FileCheck(strIniFileName))
	{
		// Magnifier
		{
			g_stOptions.stOpMag.bIsDrawCross = CppUtil::INIReadInt(STR_APP_MAG, STR_KEY_MAG_DRAWCROSS, strIniFileName) == TRUE ? true : false;
			g_stOptions.stOpMag.bIsIncludeWindow = CppUtil::INIReadInt(STR_APP_MAG, STR_KEY_MAG_INCWIN, strIniFileName) == TRUE ? true : false;
		}

		// List
		{
			g_stOptions.stOpList.bIsSaveLog = CppUtil::INIReadInt(STR_APP_LIST, STR_KEY_LIST_SAVELOG, strIniFileName) == TRUE ? true : false;
			g_stOptions.stOpList.bIsUseList = CppUtil::INIReadInt(STR_APP_LIST, STR_KEY_LIST_USE, strIniFileName) == TRUE ? true : false;
			g_stOptions.stOpList.nLogMax = CppUtil::INIReadInt(STR_APP_LIST, STR_KEY_LIST_LOGMAX, strIniFileName);
		}

		// System
		{
			g_stOptions.stOpSys.bIsMiniMode = CppUtil::INIReadInt(STR_APP_SYS, STR_KEY_SYS_MINI, strIniFileName) == TRUE ? true : false;
		}
	}

	return true;
}


/**
Save Option
@access		private
@param
@return
*/
void CColorPickaApp::SaveOptions()
{
	CString strIniFileName = _T("");
	strIniFileName.Format(_T("%s%s.ini"), CppUtil::GetExePath().GetString(), CppUtil::GetExeName(false).GetString());

	// Magnifier
	{
		CppUtil::INIWriteString(STR_APP_MAG, STR_KEY_MAG_DRAWCROSS, strIniFileName, g_stOptions.stOpMag.bIsDrawCross);
		CppUtil::INIWriteString(STR_APP_MAG, STR_KEY_MAG_INCWIN, strIniFileName, g_stOptions.stOpMag.bIsIncludeWindow);
	}

	// List
	{
		CppUtil::INIWriteString(STR_APP_LIST, STR_KEY_LIST_SAVELOG, strIniFileName, g_stOptions.stOpList.bIsSaveLog);
		CppUtil::INIWriteString(STR_APP_LIST, STR_KEY_LIST_USE, strIniFileName, g_stOptions.stOpList.bIsUseList);
		CppUtil::INIWriteString(STR_APP_LIST, STR_KEY_LIST_LOGMAX, strIniFileName, g_stOptions.stOpList.nLogMax);
	}

	// System
	{
		CppUtil::INIWriteString(STR_APP_SYS, STR_KEY_SYS_MINI, strIniFileName, g_stOptions.stOpSys.bIsMiniMode);
	}
}