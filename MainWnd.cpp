//---------------------------------------------------------------------------//
//
// MainWnd.cpp
//  Copyright (C) 2016-2018 tapetums
//
//---------------------------------------------------------------------------//

#include "MainWnd.hpp"

#include <strsafe.h>
#include <shobjidl.h>

#ifndef COM_PTR
  #define COM_PTR
  #include <wrl/client.h>
  template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#endif

#include "resource.h"

#include "Global.hpp"
#include "SettingWnd.hpp"
#include "MakeWebp.hpp"

//---------------------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------------------//

extern LPCWSTR   APP_NAME;
extern HINSTANCE g_hInst;

//---------------------------------------------------------------------------//
// Utility Functions
//---------------------------------------------------------------------------//

#if defined(UNICODE) || defined(_UNICODE)
#else
  #include <Transcode.hpp>
#endif

void get_disp_info
(
    const NMLVDISPINFO* disp_info
)
{
    if ( disp_info->item.mask & LVIF_TEXT )
    {
        const auto& image = g_images[disp_info->item.iItem];

        const auto text = disp_info->item.pszText;
        switch ( disp_info->item.iSubItem )
        {
            case 0:
            {
              #if defined(UNICODE) || defined(_UNICODE)
                ::StringCchCopy(text, 256, image.filename.c_str());
              #else
                char filename [MAX_PATH];
                tapetums::toMBCS(image.filename.c_str(), filename, MAX_PATH);
                ::StringCchCopy(text, MAX_PATH, filename);
              #endif
                break;
            }
            case 1:
            {
                ::StringCchPrintf(text, 256, TEXT("%d"), image.duration);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

//---------------------------------------------------------------------------//

HRESULT show_open_dialog
(
    std::vector<std::wstring>& filenames
)
{
    HRESULT hr;
    ComPtr<IFileOpenDialog> dialog;

    hr = ::CoCreateInstance
    (
        CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(dialog.GetAddressOf())
    );
    if ( FAILED(hr) ) { return hr; }

    DWORD options;

    hr = dialog->GetOptions(&options);
    if ( FAILED(hr) ) { return hr; }

    hr = dialog->SetOptions(options | FOS_ALLOWMULTISELECT);
    if ( FAILED(hr) ) { return hr; }

    constexpr COMDLG_FILTERSPEC file_types[6] =
    {
        { L"All files",   L"*.*" },
        { L"Bitmap file", L"*.bmp" },
        { L"GIF file",    L"*.gif" },
        { L"JPEG file",   L"*.jpg;*.jpeg" },
        { L"PNG file",    L"*.png" },
        { L"WEBP file",   L"*.webp" },
    };

    hr = dialog->SetFileTypes(6, file_types);
    if ( FAILED(hr) ) { return hr; }

    hr = dialog->Show(nullptr);
    if ( FAILED(hr) ) { return hr; }

    ComPtr<IShellItemArray> items;

    hr = dialog->GetResults(items.GetAddressOf());
    if ( FAILED(hr) ) { return hr; }

    DWORD count;
    hr = items->GetCount(&count);
    if ( FAILED(hr) ) { return hr; }

    for ( DWORD index = 0; index < count; ++index )
    {
        ComPtr<IShellItem> item;

        hr = items->GetItemAt(index, item.GetAddressOf());;
        if ( FAILED(hr) ) { continue; }

        LPWSTR path;

        hr = item->GetDisplayName(SIGDN_FILESYSPATH, &path);
        if ( FAILED(hr) ) { continue; }

        filenames.push_back(path);

        ::CoTaskMemFree(path);
    }

    return S_OK;
}

//---------------------------------------------------------------------------//

HRESULT show_save_dialog
(
    std::wstring& filename
)
{
    HRESULT hr;
    ComPtr<IFileDialog> dialog;

    hr = ::CoCreateInstance
    (
        CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(dialog.GetAddressOf())
    );
    if ( FAILED(hr) ) { return hr; }

    constexpr COMDLG_FILTERSPEC file_types[2] =
    {
        { L"WEBP file", L"*.webp" },
        { L"All files", L"*.*" },
    };

    hr = dialog->SetFileTypes(2, file_types);
    if ( FAILED(hr) ) { return hr; }

    hr = dialog->SetDefaultExtension(L"webp");
    if ( FAILED(hr) ) { return hr; }

    hr = dialog->Show(nullptr);
    if ( FAILED(hr) ) { return hr; }

    ComPtr<IShellItem> item;

    hr = dialog->GetResult(item.GetAddressOf());
    if ( FAILED(hr) ) { return hr; }

    LPWSTR path;
    hr = item->GetDisplayName(SIGDN_FILESYSPATH, &path);
    if ( FAILED(hr) ) { return hr; }

    filename = path;

    ::CoTaskMemFree(path);

    return S_OK;
}

//---------------------------------------------------------------------------//
// ctor
//---------------------------------------------------------------------------//

MainWnd::MainWnd()
{
    // フォントの生成
    fontM.Create(14, L"Meiryo UI", FW_REGULAR);
    fontE.Create(14, L"Segoe UI Emoji", FW_REGULAR);

    // ウィンドウの生成
    Create(WS_OVERLAPPEDWINDOW, WS_EX_ACCEPTFILES, nullptr, nullptr);
    SetText(APP_NAME);
    Resize(640, 480);
    ToCenter();
    Show();

    // アイコンの設定
    const auto icon = ::LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON));
    SetWindowIcon(icon);
}

//---------------------------------------------------------------------------//
// Window Procedure
//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::WndProc
(
    HWND hwnd, UINT msg, WPARAM wp, LPARAM lp
)
{
    if ( msg == WM_PROCESS ) { return OnProcess(SIZE_T(wp), SIZE_T(lp)); }

    switch ( msg )
    {
        case WM_CREATE:        { return OnCreate(LPCREATESTRUCT(lp)); }
        case WM_DESTROY:       { return OnDestroy(); }
        case WM_SIZE:          { return OnSize(LOWORD(lp), HIWORD(lp)); }
        case WM_PAINT:         { return OnPaint(hwnd); }
        case WM_CLOSE:         { return OnClose(); }
        case WM_GETMINMAXINFO: { return OnGetMinMaxInfo(LPMINMAXINFO(lp)); }
        case WM_NOTIFY:        { return OnNotify(LPNMHDR(lp)); }
        case WM_COMMAND:       { return OnCommand(LOWORD(wp), HIWORD(wp), HWND(lp)); }
        case WM_DROPFILES:     { return OnDropFiles(HDROP(wp)); }
        case WM_HSCROLL:       { return OnSeek(LOWORD(wp), HIWORD(wp), HWND(lp)); }
        default:               { return super::WndProc(hwnd, msg, wp, lp); }
    }
}

//---------------------------------------------------------------------------//
// Message Handlers
//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnCreate
(
    CREATESTRUCT* cs
)
{
    UNREFERENCED_PARAMETER(cs);

    const auto hr = ::CoCreateInstance
    (
        CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(taskbarlist.GetAddressOf())
    );
    if ( FAILED(hr) )
    {
        Destroy(); return 0;
    }

    list_window.      Create(LVS_OWNERDATA | LVS_REPORT | LVS_SHOWSELALWAYS, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, m_hwnd, LIST_WINDOW);
    preview_window.   Create(WS_CHILD | WS_VISIBLE, 0, m_hwnd, HMENU(PREVIEW_WINDOW));
    seekbar.          Create(0, 0, m_hwnd, SEEKBAR);
    button_lossy.     Create(BS_AUTORADIOBUTTON, 0, m_hwnd, BUTTON_LOSSY);
    button_lossless.  Create(BS_AUTORADIOBUTTON, 0, m_hwnd, BUTTON_LOSSLESS);
    label_quality.    Create(0, 0, m_hwnd, LABEL_QUALITY);
    track_quality.    Create(TBS_TOOLTIPS, 0, m_hwnd, TRACK_QUALITY);
    edit_duration.    Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_DURATION);
    combobox_method.  Create(CBS_DROPDOWNLIST, 0, m_hwnd, COMBOBOX_METHOD);
    button_advanced.  Create(0, 0, m_hwnd, BUTTON_ADVANCED);
    label_ms.         Create(0, 0, m_hwnd, LABEL_MS);
    label_frame_skip. Create(0, 0, m_hwnd, LABEL_FRAME_SKIP);
    edit_frame_skip.  Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_FRAME_SKIP);
    updown_frame_skip.Create(UDS_AUTOBUDDY, 0, m_hwnd, UPDOWN_FRAME_SKIP);
    button_delete.    Create(0, 0, m_hwnd, BUTTON_DELETE);
    button_add.       Create(0, 0, m_hwnd, BUTTON_ADD);
    button_create.    Create(0, 0, m_hwnd, BUTTON_CREATE);

    list_window.     SetFont(fontM);
    button_lossy.    SetFont(fontM);
    button_lossless. SetFont(fontM);
    label_quality.   SetFont(fontM);
    combobox_method. SetFont(fontM);
    button_advanced. SetFont(fontE);
    edit_duration.   SetFont(fontM);
    label_ms.        SetFont(fontM);
    label_frame_skip.SetFont(fontM);
    edit_frame_skip. SetFont(fontM);
    button_delete.   SetFont(fontM);
    button_add.      SetFont(fontM);
    button_create.   SetFont(fontM);

    button_lossy.    SetText(L"lossy");
    button_lossless. SetText(L"lossless");
    label_quality.   SetText(L"quality :");
    edit_duration.   SetText(L"33");
    button_advanced. SetText(L"Advanced ⚙");
    label_ms.        SetText(L"ms");
    label_frame_skip.SetText(L"frame skip :");
    button_delete.   SetText(L"Delete");
    button_add.      SetText(L"Add");
    button_create.   SetText(L"Create");

    list_window.InsertColumn(L"file", 224, 0, LVCFMT_CENTER);
    list_window.InsertColumn(L"ms", 64, 1, LVCFMT_CENTER);
    ListView_SetItemCountEx(list_window, 0, LVSICF_NOINVALIDATEALL);

    seekbar.range(0, 0);
    seekbar.position(0);

    track_quality.range(0, 100);
    track_quality.position(UINT(g_config.config.quality));

    combobox_method.AddString(L"method: 0");
    combobox_method.AddString(L"method: 1");
    combobox_method.AddString(L"method: 2");
    combobox_method.AddString(L"method: 3");
    combobox_method.AddString(L"method: 4");
    combobox_method.AddString(L"method: 5");
    combobox_method.AddString(L"method: 6");
    combobox_method.Select(g_config.config.method);

    edit_duration.Send(EM_SETLIMITTEXT, 8, 0);

    updown_frame_skip.range(0, 10);
    updown_frame_skip.position(g_config.frame_skip);

    wchar_t buf [16];
    ::StringCchPrintfW(buf, 16, L"%d", updown_frame_skip.position());
    edit_frame_skip.SetText(buf);

    if ( g_config.config.lossless )
    {
        button_lossless.Check();
    }
    else
    {
        button_lossy.Check();
    }

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnDestroy()
{
    taskbarlist = nullptr;

    ::PostQuitMessage(0);

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnSize
(
    INT16 cx, INT16 cy
)
{
    list_window.Bounds(0, 0, 320, cy - 8);

    preview_window.Bounds(320,        0, cx - 320, cy - 228);
    seekbar.       Bounds(320, cy - 228, cx - 320,       24);

    button_lossy.   Bounds((cx - 320 - 136) / 2 + 320 + 16, cy - 200, 64, 16);
    button_lossless.Bounds((cx - 320 - 136) / 2 + 320 + 80, cy - 200, 72, 16);

    label_quality.Bounds((cx - 320 - 304) / 2 + 320 + 16, cy - 168,  64, 16);
    track_quality.Bounds((cx - 320 - 304) / 2 + 320 + 64, cy - 168, 240, 24);

    combobox_method.Bounds((cx - 320 - 200) / 2 + 320,       cy - 128, 96, 24);
    button_advanced.Bounds((cx - 320 - 200) / 2 + 320 + 144, cy - 128, 80, 24);

    edit_duration.    Bounds((cx - 320 - 290) / 2 + 320 +   8, cy - 84, 64, 16);
    label_ms.         Bounds((cx - 320 - 290) / 2 + 320 +  88, cy - 84, 16, 16);
    label_frame_skip. Bounds((cx - 320 - 290) / 2 + 320 + 140, cy - 84, 76, 16);
    edit_frame_skip.  Bounds((cx - 320 - 290) / 2 + 320 + 216, cy - 84, 48, 16);
    updown_frame_skip.Bounds((cx - 320 - 290) / 2 + 320 + 268, cy - 84, 16, 20);

    button_delete.Bounds(320 + 16,                  cy - 48, 80, 32);
    button_add   .Bounds((cx - 320 - 80) / 2 + 320, cy - 48, 80, 32);
    button_create.Bounds(cx - 80 - 16,              cy - 48, 80, 32);

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnPaint
(
    HWND hwnd
)
{
    PAINTSTRUCT ps;
    const auto hdc = ::BeginPaint(hwnd, &ps);

    ::FillRect(hdc, &ps.rcPaint, GetSysColorBrush(COLOR_BTNFACE));

    ::EndPaint(hwnd, &ps);

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnClose()
{
    if ( write_thread.joinable() )
    {
        const auto answer = ::MessageBoxW
        (
            m_hwnd, L"Do you want to quit?", APP_NAME, MB_YESNO | MB_DEFBUTTON2
        );
        if ( answer != IDYES )
        {
            return 0;
        }

        button_create.SetText(L"...");

        g_abort = true;
        if ( write_thread.joinable() )
        {
            write_thread.join();
        }
    }

    Destroy();
    
    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnGetMinMaxInfo
(
    MINMAXINFO* mmi
)
{
    mmi->ptMinTrackSize.x = 640;
    mmi->ptMinTrackSize.y = 480;

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnNotify
(
    const NMHDR* nmhdr
)
{
    if ( nmhdr->idFrom == CTRL::LIST_WINDOW )
    {
        const auto nmlv = LPNMLISTVIEW(nmhdr);
        if ( nmlv->hdr.code == LVN_GETDISPINFO )
        {
            get_disp_info((NMLVDISPINFO*)(nmhdr));
        }
        if ( nmlv->hdr.code == LVN_ITEMCHANGED && (nmlv->uNewState & LVIS_FOCUSED) )
        {
            const auto selected_index = nmlv->iItem;

            preview_window.Post
            (
                PreviewWnd::WM_SELCTED_INDEX_CHANGED, 0, LPARAM(selected_index)
            );

            if ( selected_index < int(g_images.size()) )
            {
                seekbar.position(selected_index);

                const auto& item = g_images[selected_index];

                wchar_t buf [16];
                ::StringCchPrintfW(buf, 16, L"%d", item.duration);
                edit_duration.SetText(buf);
            }
        }
    }
    else if ( nmhdr->idFrom == CTRL::UPDOWN_FRAME_SKIP )
    {
        const auto nmud = LPNMUPDOWN(nmhdr);
        if ( nmud->hdr.code == UDN_DELTAPOS )
        {
            const auto position = nmud->iPos + nmud->iDelta;
            if ( 0 <= position && position <= 10 )
            {
                wchar_t buf [16];
                ::StringCchPrintfW(buf, 16, L"%d", position);
                edit_frame_skip.SetText(buf);

                g_config.frame_skip = position;
            }
        }
    }

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnCommand
(
    UINT16 id, UINT16 code, HWND ctrl
)
{
    UNREFERENCED_PARAMETER(ctrl);

    switch ( id )
    {
        case CTRL::EDIT_DURATION:
        {
            if ( code == EN_UPDATE ) { OnDurationChanged(); }
            break;
        }
        case ID_ACCEL_INSERT:
        case ID_ACCEL_OPEN:
        case CTRL::BUTTON_ADD:
        {
            OnAddFiles();
            break;
        }
        case ID_ACCEL_DEL:
        case CTRL::BUTTON_DELETE:
        {
            OnDeleteFiles();
            break;
        }
        case ID_ACCEL_UP:
        {
            OnSelectUp();
            break;
        }
        case ID_ACCEL_DOWN:
        {
            OnSelectDown();
            break;
        }
        case ID_ACCEL_SELECT_ALL:
        {
            OnSelectAll();
            break;
        }
        case ID_ACCEL_DESELECT:
        {
            OnDeselect();
            break;
        }
        case ID_ACCEL_ENTER:
        case CTRL::BUTTON_CREATE:
        {
            OnCreateButtonClicked();
            break;
        }
        case ID_ACCEL_ESC:
        case ID_ACCEL_ABORT:
        {
            if ( write_thread.joinable() ) { Abort(); }
            break;
        }
        case ID_ACCEL_QUIT:
        {
            Close();
            break;
        }
        case CTRL::BUTTON_LOSSY:
        case CTRL::BUTTON_LOSSLESS:
        {
            g_config.config.lossless = button_lossy.IsChecked() ? 0 : 1;
            break;
        }
        case CTRL::BUTTON_ADVANCED:
        {
            ShowSettingWindow(m_hwnd);
            break;
        }
        case CTRL::COMBOBOX_METHOD:
        {
            if ( code == CBN_SELCHANGE )
            {
                g_config.config.method = combobox_method.SelectedIndex();
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnDropFiles
(
    HDROP hDrop
)
{
    wchar_t filename [MAX_PATH];
    std::vector<std::wstring> filenames;

    const auto count = ::DragQueryFileW(hDrop, UINT(-1), nullptr, 0);
    for ( UINT i = 0; i < count; ++i )
    {
        ::DragQueryFileW(hDrop, i, filename, MAX_PATH);

        filenames.push_back(filename);
    }

    ::DragFinish(hDrop);

    AddFilesToList(filenames);
 
    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnSeek
(
    WORD request, WORD position, HWND ctrl
)
{
    if ( request != SB_THUMBPOSITION && request != SB_THUMBTRACK )
    {
        return 0;
    }

    if ( ctrl == seekbar.handle() )
    {
        preview_window.Post
        (
            PreviewWnd::WM_SELCTED_INDEX_CHANGED, 0, LPARAM(position)
        );
    }
    else if ( ctrl == track_quality.handle() )
    {
        g_config.config.quality = float(track_quality.position());
    }

    return 0;
}

//---------------------------------------------------------------------------//
// Internal Methods
//---------------------------------------------------------------------------//

LRESULT CALLBACK MainWnd::OnProcess
(
    SIZE_T numer, SIZE_T denom
)
{
    wchar_t buf [256];
    ::StringCchPrintfW(buf, 256, L"%s (%zu/%zu)", APP_NAME, numer, denom);
    SetText(buf);

    taskbarlist->SetProgressValue(m_hwnd, numer, denom);

    if ( numer >= denom )
    {
        if ( write_thread.joinable() )
        {
            write_thread.join();
        }

        g_abort = false;
        taskbarlist->SetProgressState(m_hwnd, TBPFLAG::TBPF_PAUSED);
        button_create.SetText(L"Create");

        MessageBoxW(m_hwnd, L"Completed", APP_NAME, MB_OK);
    }

    return 0;
}

//---------------------------------------------------------------------------//

void MainWnd::OnDurationChanged()
{
    wchar_t buf [16];
    edit_duration.GetText(buf, 16);
    const auto duration = ::StrToIntW(buf);

    auto index = ListView_GetNextItem
    (
        list_window, -1, LVNI_ALL | LVNI_SELECTED
    );
    while ( index != -1 )
    {
        g_images[index].duration = duration;

        index = ListView_GetNextItem
        (
            list_window, index, LVNI_ALL | LVNI_SELECTED
        );
    }

    list_window.Refresh();
}

//---------------------------------------------------------------------------//

void MainWnd::OnAddFiles()
{
    std::vector<std::wstring> filenames;

    const auto hr = show_open_dialog(filenames);
    if ( FAILED(hr) ) { return; }

    AddFilesToList(filenames);
}

//---------------------------------------------------------------------------//

void MainWnd::OnDeleteFiles()
{
    const auto count = ListView_GetSelectedCount(list_window);
    for ( UINT n = 0; n < count; ++n )
    {
        const auto index = ListView_GetNextItem
        (
            list_window, -1, LVNI_ALL | LVNI_SELECTED
        );
        
        if ( index >= int(g_images.size()) )
        {
            break;
        }

        g_images.erase(g_images.begin() + index);

        ListView_SetItemCountEx(list_window, g_images.size(), LVSICF_NOINVALIDATEALL);
    }

    seekbar.range(0, UINT(g_images.size()) - 1);

    list_window.Refresh();

    preview_window.Post
    (
        PreviewWnd::WM_SELCTED_INDEX_CHANGED, 0, LPARAM(list_window.SelectedIndex())
    );
}

//---------------------------------------------------------------------------//

void MainWnd::OnSelectUp()
{
    const auto index = ListView_GetNextItem
    (
        list_window, -1, LVNI_ALL | LVNI_SELECTED
    );

    list_window.ClearSelect();

    if ( index < 1 )
    {
        list_window.Select(INT32(g_images.size() - 1));
    }
    else
    {
        list_window.Select(index - 1);
    }
}

//---------------------------------------------------------------------------//

void MainWnd::OnSelectDown()
{
    const auto index = ListView_GetNextItem
    (
        list_window, -1, LVNI_ALL | LVNI_SELECTED
    );

    list_window.ClearSelect();

    if ( index < int(g_images.size()) - 1 )
    {
        list_window.Select(index + 1);
    }
    else
    {
        list_window.Select(0);
    }
}

//---------------------------------------------------------------------------//

void MainWnd::OnSelectAll()
{
    list_window.Select(-1);
    list_window.Refresh();

    preview_window.Post
    (
        PreviewWnd::WM_SELCTED_INDEX_CHANGED, 0, LPARAM(list_window.SelectedIndex())
    );
}

//---------------------------------------------------------------------------//

void MainWnd::OnDeselect()
{
    list_window.ClearSelect();
    list_window.Refresh();

    preview_window.Post
    (
        PreviewWnd::WM_SELCTED_INDEX_CHANGED, 0, LPARAM(list_window.SelectedIndex())
    );
}

//---------------------------------------------------------------------------//

void MainWnd::OnCreateButtonClicked()
{
    if ( write_thread.joinable() )
    {
        Abort();
    }
    else
    {
        CreateWebP();
    }
}

//---------------------------------------------------------------------------//

void MainWnd::ShowSettingWindow
(
    HWND hwnd
)
{
    ::EnableWindow(hwnd, FALSE);

    SettingWnd setting_wnd(hwnd);
    setting_wnd.is_alive = true;

    MSG msg { };
    while ( setting_wnd.is_alive )
    {
        ::GetMessage(&msg, nullptr, 0, 0);
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    if ( g_config.config.lossless )
    {
        button_lossy.Uncheck();
        button_lossless.Check();
    }
    else
    {
        button_lossy.Check();
        button_lossless.Uncheck();
    }

    track_quality.position(INT32(g_config.config.quality));

    combobox_method.Select(g_config.config.method);

    updown_frame_skip.position(INT32(g_config.frame_skip));

    wchar_t buf [16];
    ::StringCchPrintfW(buf, 16, L"%d", updown_frame_skip.position());
    edit_frame_skip.SetText(buf);

    ::EnableWindow(hwnd, TRUE);
    ::BringWindowToTop(hwnd);
}

//---------------------------------------------------------------------------//

void MainWnd::AddFilesToList
(
    const std::vector<std::wstring>& filenames
)
{
    wchar_t buf [16];
    edit_duration.GetText(buf, 16);
    const auto duration = ::StrToIntW(buf);

    for ( const auto& filename : filenames )
    {
        g_images.push_back({ filename, duration, 0 });
    }

    ListView_SetItemCountEx(list_window, g_images.size(), LVSICF_NOINVALIDATEALL);

    seekbar.range(0, UINT(g_images.size()) - 1);

    list_window.Refresh();
}

//---------------------------------------------------------------------------//

void MainWnd::CreateWebP()
{
    if ( g_images.size() == 0 )
    {
        return;
    }

    std::wstring filename;
    const auto hr = show_save_dialog(filename);
    if ( FAILED(hr) )
    {
        return;
    }

    button_create.SetText(L"Abort");
    taskbarlist->SetProgressState(m_hwnd, TBPFLAG::TBPF_NORMAL);
    g_abort = false;

    write_thread = std::thread
    (
        make_animated_webp, m_hwnd, g_images, filename.c_str()
    );
}

//---------------------------------------------------------------------------//

void MainWnd::Abort()
{
    const auto answer = ::MessageBoxW
    (
        m_hwnd, L"Do you want to abort?", APP_NAME, MB_YESNO | MB_DEFBUTTON2
    );
    if ( answer != IDYES )
    {
        return;
    }

    button_create.SetText(L"...");

    g_abort = true;
    if ( write_thread.joinable() )
    {
        write_thread.join();
    }

    g_abort = false;
    taskbarlist->SetProgressState(m_hwnd, TBPFLAG::TBPF_PAUSED);
    button_create.SetText(L"Create");

    SetText(APP_NAME);
    MessageBoxW(m_hwnd, L"Aborted", APP_NAME, MB_OK);
}

//---------------------------------------------------------------------------//

// MainWnd.cpp