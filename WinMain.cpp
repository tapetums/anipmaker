//---------------------------------------------------------------------------//
//
// WinMain.cpp
//  Copyright (C) 2016-2018 tapetums
//
//---------------------------------------------------------------------------//

#include <windows.h>
#include <tchar.h>

#include "resource.h"

#include "MainWnd.hpp"

//---------------------------------------------------------------------------//

#if defined(_DEBUG) || defined(DEBUG)
  #define _CRTDBG_MAP_ALLOC
  #include <crtdbg.h>
  #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

//---------------------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------------------//

LPCWSTR   APP_NAME { L"anipmaker" };
LPCWSTR   VERSION  { L"0.1.0.0" };
HINSTANCE g_hInst  { nullptr };

//---------------------------------------------------------------------------//
// Application Entry Point
//---------------------------------------------------------------------------//

INT32 APIENTRY _tWinMain
(
    HINSTANCE hInstance, HINSTANCE, LPTSTR, INT32
)
{
    // メモリリーク検出
  #if defined(_DEBUG) || defined(DEBUG)
    ::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
  #endif

    // グローバル変数に情報を記憶
    g_hInst = hInstance;

    // COM の 初期化
    ::CoInitialize(nullptr);

    // ウィンドウの生成
    MainWnd wnd;

    // アクセラレータの読み込み
    const auto accelerator = ::LoadAccelerators
    (
        g_hInst, MAKEINTRESOURCE(IDR_ACCELERATOR)
    );
    if ( accelerator == nullptr )
    {
        return -1;
    }

    // メッセージループ
    MSG msg { };
    while ( ::GetMessage(&msg, nullptr, 0, 0) > 0 )
    {
        if ( ! TranslateAccelerator(wnd, accelerator, &msg) )
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    // アクセラレータの破棄
    ::DestroyAcceleratorTable(accelerator);

    // COM の 終了処理
    ::CoUninitialize();

    return static_cast<INT32>(msg.wParam);
}

//---------------------------------------------------------------------------//

// WinMain.cpp