#pragma once

//---------------------------------------------------------------------------//
//
// MainWnd.hpp
//  Copyright (C) 2016-2018 tapetums
//
//---------------------------------------------------------------------------//

#include <string>
#include <thread>
#include <vector>

#include <wrl/client.h>
#include <shobjidl.h>

#include <UWnd.hpp>
#include <CtrlWnd.hpp>
#include <Font.hpp>

#include "PreviewWnd.hpp"

//---------------------------------------------------------------------------//
// Classes
//---------------------------------------------------------------------------//

class MainWnd : public tapetums::UWnd
{
    using super = UWnd;

    enum CTRL : INT16
    {
        LIST_WINDOW, PREVIEW_WINDOW, SEEKBAR,
        BUTTON_LOSSY, BUTTON_LOSSLESS,
        LABEL_QUALITY, TRACK_QUALITY,
        EDIT_DURATION, LABEL_MS,
        COMBOBOX_METHOD, BUTTON_ADVANCED,
        LABEL_FRAME_SKIP, EDIT_FRAME_SKIP, UPDOWN_FRAME_SKIP,
        BUTTON_DELETE, BUTTON_ADD, BUTTON_CREATE,
    };

private:
    Microsoft::WRL::ComPtr<ITaskbarList3> taskbarlist;

    tapetums::Font        fontM;
    tapetums::Font        fontE;
    tapetums::ListWnd     list_window;
    PreviewWnd            preview_window;
    tapetums::TrackbarWnd seekbar;
    tapetums::BtnWnd      button_lossy;
    tapetums::BtnWnd      button_lossless;
    tapetums::LabelWnd    label_quality;
    tapetums::TrackbarWnd track_quality;
    tapetums::ComboBox    combobox_method;
    tapetums::BtnWnd      button_advanced;
    tapetums::EditWnd     edit_duration;
    tapetums::LabelWnd    label_ms;
    tapetums::LabelWnd    label_frame_skip;
    tapetums::LabelWnd    edit_frame_skip;
    tapetums::UpDownWnd   updown_frame_skip;
    tapetums::BtnWnd      button_delete;
    tapetums::BtnWnd      button_add;
    tapetums::BtnWnd      button_create;

    std::thread write_thread;

public:
    enum { WM_PROCESS = WM_APP + 1, };

public:
    MainWnd();

public:
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;

private:
    LRESULT CALLBACK OnCreate       (CREATESTRUCT* cs);
    LRESULT CALLBACK OnDestroy      ();
    LRESULT CALLBACK OnSize         (INT16 cx, INT16 cy);
    LRESULT CALLBACK OnPaint        (HWND hwnd);
    LRESULT CALLBACK OnClose        ();
    LRESULT CALLBACK OnGetMinMaxInfo(MINMAXINFO* mmi);
    LRESULT CALLBACK OnNotify       (const NMHDR* nmhdr);
    LRESULT CALLBACK OnCommand      (UINT16 id, UINT16 code, HWND ctrl);
    LRESULT CALLBACK OnDropFiles    (HDROP hDrop);
    LRESULT CALLBACK OnSeek         (WORD request, WORD position, HWND ctrl);
    LRESULT CALLBACK OnProcess      (SIZE_T numer, SIZE_T denom);

private:
    void OnDurationChanged    ();
    void OnAddFiles           ();
    void OnDeleteFiles        ();
    void OnSelectUp           ();
    void OnSelectDown         ();
    void OnSelectAll          ();
    void OnDeselect           ();
    void OnCreateButtonClicked();
    void ShowSettingWindow    (HWND hwnd);
    void AddFilesToList       (const std::vector<std::wstring> &filenames);
    void CreateWebP           ();
    void Abort                ();
};

//---------------------------------------------------------------------------//

// MainWnd.hpp