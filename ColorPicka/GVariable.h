#pragma once

// Key Hooker (for Spacebar Capture)
extern HHOOK g_hKeyboard;
// Main Dialog HWND
extern HWND g_hwndMain;
// Hooking Key Code
extern int g_nHookKey;

// Option - Magnifier Const
constexpr LPCTSTR STR_APP_MAG = _T("Magnifier");
constexpr LPCTSTR STR_KEY_MAG_INCWIN = _T("IncludeWindow");
constexpr LPCTSTR STR_KEY_MAG_DRAWCROSS = _T("DrawCross");
// Option - List Const
constexpr LPCTSTR STR_APP_LIST = _T("List");
constexpr LPCTSTR STR_KEY_LIST_SAVELOG = _T("LogSave");
constexpr LPCTSTR STR_KEY_LIST_USE = _T("Use");
constexpr LPCTSTR STR_KEY_LIST_LOGDATA = _T("LogData");
constexpr LPCTSTR STR_KEY_LIST_LOGMAX = _T("LogItemMax");
// Option - System Const
constexpr LPCTSTR STR_APP_SYS = _T("System");
constexpr LPCTSTR STR_KEY_SYS_MINI = _T("MiniMode");

// Option Structure
struct stOpMagnifier
{
	bool bIsIncludeWindow;	// Color Search include Window
	bool bIsDrawCross;		// Draw Crossline

	stOpMagnifier()
		: bIsIncludeWindow(false)
		, bIsDrawCross(true) 
	{}
};
struct stOpColorList
{
	bool bIsSaveLog;		// Save Color Log in List
	bool bIsUseList;		// Use List
	int nLogMax;			// Log Max Count

	stOpColorList()
		: bIsSaveLog(false)
		, bIsUseList(true)
		, nLogMax(17)
	{}
};
struct stOpSystem
{
	bool bIsMiniMode;		// Mini Mode

	stOpSystem()
		: bIsMiniMode(false)
	{}
};
struct stOptions
{
	stOpMagnifier stOpMag;
	stOpColorList stOpList;
	stOpSystem stOpSys;
};
extern stOptions g_stOptions;