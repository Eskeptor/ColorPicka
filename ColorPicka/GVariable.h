#pragma once

// Key Hooker (for Spacebar Capture)
extern HHOOK g_hKeyboard;
// Main Dialog HWND
extern HWND g_hwndMain;
// Hooking Key Code
extern int g_nHookKey;

// Magnifier Const
constexpr LPCTSTR STR_APP_MAG = _T("Magnifier");
constexpr LPCTSTR STR_KEY_MAG_INCWIN = _T("IncludeWindow");
constexpr LPCTSTR STR_KEY_MAG_DRAWCROSS = _T("DrawCross");
// List Const
constexpr LPCTSTR STR_APP_LIST = _T("List");
constexpr LPCTSTR STR_KEY_LIST_SAVELOG = _T("LogSave");
constexpr LPCTSTR STR_KEY_LIST_USE = _T("Use");
constexpr LPCTSTR STR_KEY_LIST_LOGDATA = _T("LogData");
constexpr LPCTSTR STR_KEY_LIST_LOGMAX = _T("LogItemMax");

// Option Structure
struct stOpMagnifier
{
	bool bIsIncludeWindow;
	bool bIsDrawCross;

	stOpMagnifier()
		: bIsIncludeWindow(false)
		, bIsDrawCross(true) 
	{}
};
struct stOpColorList
{
	bool bIsSaveLog;
	bool bIsUseList;
	int nLogMax;

	stOpColorList()
		: bIsSaveLog(false)
		, bIsUseList(true)
		, nLogMax(17)
	{}
};
struct stOpSystem
{
	
};
struct stOptions
{
	stOpMagnifier stOpMag;
	stOpColorList stOpList;
};
extern stOptions g_stOptions;