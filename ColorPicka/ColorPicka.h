﻿#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"	


class CColorPickaApp : public CWinApp
{
// Enums and Structure ==================================================================================

// ======================================================================================================


// Variables ============================================================================================

// ======================================================================================================


// Functions ============================================================================================
private:
	bool InitOptions();
	void SaveOptions();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CColorPickaApp();

	virtual BOOL InitInstance();

// ======================================================================================================
};

extern CColorPickaApp theApp;
