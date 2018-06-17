#pragma once

//---------------------------------------------------------------------------//
//
// SettingWnd.cpp
//  Copyright (C) 2018 tapetums
//
//---------------------------------------------------------------------------//

#include "SettingWnd.hpp"

#include "resource.h"

#include "Global.hpp"

//---------------------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------------------//

extern LPCWSTR   APP_NAME;
extern LPCWSTR   VERSION;
extern HINSTANCE g_hInst;

//---------------------------------------------------------------------------//
// ctor
//---------------------------------------------------------------------------//

SettingWnd::SettingWnd
(
    HWND parent
)
{
    // フォントの生成
    fontM.Create(14, L"Meiryo UI", FW_REGULAR);

    // ウィンドウの生成
    Create(WS_SYSMENU, 0, parent, nullptr);
    SetText(APP_NAME);
    Resize(610, 640);
    ToCenter();
    Show();

    // アイコンの設定
    const auto icon = ::LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON));
    SetWindowIcon(icon);
}

//---------------------------------------------------------------------------//
// Window Procedure
//---------------------------------------------------------------------------//

LRESULT CALLBACK SettingWnd::WndProc
(
    HWND hwnd, UINT msg, WPARAM wp, LPARAM lp
)
{
    switch ( msg )
    {
        case WM_CREATE:  { return OnCreate(LPCREATESTRUCT(lp)); }
        case WM_DESTROY: { return OnDestroy(); }
        case WM_SIZE:    { return OnSize(LOWORD(lp), HIWORD(lp)); }
        case WM_PAINT:   { return OnPaint(hwnd); }
        case WM_NOTIFY:  { return OnNotify(LPNMHDR(lp)); }
        case WM_COMMAND: { return OnCommand(LOWORD(wp), HIWORD(wp), HWND(lp)); }
        case WM_HSCROLL: { return OnHScroll(LOWORD(wp), HIWORD(wp), HWND(lp)); }
        default:         { return super::WndProc(hwnd, msg, wp, lp); }
    }
}

//---------------------------------------------------------------------------//
// Message Handlers
//---------------------------------------------------------------------------//

LRESULT CALLBACK SettingWnd::OnCreate
(
    CREATESTRUCT* cs
)
{
    UNREFERENCED_PARAMETER(cs);

    label_loop_count. Create(0, 0, m_hwnd, LABEL_LOOP_COUNT);
    edit_loop_count.  Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_LOOP_COUNT);
    updown_loop_count.Create(UDS_AUTOBUDDY, 0, m_hwnd, UPDOWN_LOOP_COUNT);

    button_minimize.Create(BS_AUTOCHECKBOX, 0, m_hwnd, BUTTON_MINIMIZE);

    label_kmin. Create(0, 0, m_hwnd, LABEL_KMIN);
    edit_kmin.  Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_KMIN);
    updown_kmin.Create(UDS_AUTOBUDDY, 0, m_hwnd, UPDOWN_KMIN);

    label_kmax. Create(0, 0, m_hwnd, LABEL_KMAX);
    edit_kmax.  Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_KMAX);
    updown_kmax.Create(UDS_AUTOBUDDY, 0, m_hwnd, UPDOWN_KMAX);

    button_lossy.   Create(BS_AUTORADIOBUTTON, 0, m_hwnd, BUTTON_LOSSY);
    button_lossless.Create(BS_AUTORADIOBUTTON, 0, m_hwnd, BUTTON_LOSSLESS);

    label_quality.Create(0, 0, m_hwnd, LABEL_QUALITY);
    track_quality.Create(TBS_TOOLTIPS, 0, m_hwnd, TRACK_QUALITY);

    combobox_method.Create(CBS_DROPDOWNLIST, 0, m_hwnd, COMBOBOX_METHOD);

    combobox_image_hint.Create(CBS_DROPDOWNLIST, 0, m_hwnd, COMBOBOX_IMAGE_HINT);

    label_target_size.Create(0, 0, m_hwnd, LABEL_TARGET_SIZE);
    edit_target_size. Create(ES_NUMBER | ES_RIGHT, WS_EX_CLIENTEDGE, m_hwnd, EDIT_TARGET_SIZE);
    label_byte.       Create(0, 0, m_hwnd, LABEL_BYTE);

    label_target_PSNR.Create(0, 0, m_hwnd, LABEL_TARGET_PSNR);
    edit_target_PSNR. Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_TARGET_PSNR);
    label_dB.         Create(0, 0, m_hwnd, LABEL_DB);

    label_segments. Create(0, 0, m_hwnd, LABEL_SEGMENTS);
    edit_segments.  Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_SEGMENTS);
    updown_segments.Create(UDS_AUTOBUDDY, 0, m_hwnd, UPDOWN_SEGMENTS);

    label_SNS_strength.Create(0, 0, m_hwnd, LABEL_SNS_STRENGTH);
    track_SNS_strength.Create(TBS_TOOLTIPS, 0, m_hwnd, TRACK_SNS_STRENGTH);

    label_filter_strength.Create(0, 0, m_hwnd, LABEL_FILTER_STRENGTH);
    track_filter_strength.Create(TBS_TOOLTIPS, 0, m_hwnd, TRACK_FILTER_STRENGTH);

    label_filter_sharpness.Create(0, 0, m_hwnd, LABEL_FILTER_SHARPNESS);
    track_filter_sharpness.Create(TBS_TOOLTIPS, 0, m_hwnd, TRACK_FILTER_SHARPNESS);

    combobox_filter_type.Create(CBS_DROPDOWNLIST, 0, m_hwnd, COMBOBOX_FILTER_TYPE);

    button_autofilter.Create(BS_AUTOCHECKBOX, 0, m_hwnd, BUTTON_AUTO_FILTER);

    button_alpha_compression.Create(BS_AUTOCHECKBOX, 0, m_hwnd, BUTTON_ALPHA_COMPRESSION);
    combobox_alpha_filtering.Create(CBS_DROPDOWNLIST, 0, m_hwnd, COMBOBOX_ALPHA_FILTERING);

    label_alpha_quality.Create(0, 0, m_hwnd, LABEL_ALPHA_QUALITY);
    track_alpha_quality.Create(TBS_TOOLTIPS, 0, m_hwnd, TRACK_ALPHA_QUALITY);

    label_pass. Create(0, 0, m_hwnd, LABEL_PASS);
    edit_pass.  Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_PASS);
    updown_pass.Create(UDS_AUTOBUDDY, 0, m_hwnd, UPDOWN_PASS);

    combobox_preprocessing.Create(CBS_DROPDOWNLIST, 0, m_hwnd, COMBOBOX_PREPROCESSING);

    label_partitions. Create(0, 0, m_hwnd, LABEL_PARTITIONS);
    edit_partitions.  Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_PARTITIONS);
    updown_partitions.Create(UDS_AUTOBUDDY, 0, m_hwnd, UPDOWN_PARTITIONS);

    label_partition_limit.Create(0, 0, m_hwnd, LABEL_PARTITION_LIMIT);
    track_partition_limit.Create(TBS_TOOLTIPS, 0, m_hwnd, TRACK_PARTITION_LIMIT);

    button_emulate_jpeg_size.Create(BS_AUTOCHECKBOX, 0, m_hwnd, BUTTON_EMULATE_JPEG_SIZE);
    button_thread_level.     Create(BS_AUTOCHECKBOX, 0, m_hwnd, BUTTON_THREAD_LEVEL);
    button_low_memory.       Create(BS_AUTOCHECKBOX, 0, m_hwnd, BUTTON_LOW_MEMORY);

    label_near_lossless. Create(0, 0, m_hwnd, LABEL_NEAR_LOSSLESS);
    track_near_lossless.Create(TBS_TOOLTIPS, 0, m_hwnd, TRACK_NEAR_LOSSLESS);

    button_exact.Create(BS_AUTOCHECKBOX, 0, m_hwnd, BUTTON_EXACT);

    label_frame_skip. Create(0, 0, m_hwnd, LABEL_FRAME_SKIP);
    edit_frame_skip.  Create(ES_NUMBER | ES_CENTER, WS_EX_CLIENTEDGE, m_hwnd, EDIT_FRAME_SKIP);
    updown_frame_skip.Create(UDS_AUTOBUDDY, 0, m_hwnd, UPDOWN_FRAME_SKIP);

    label_loop_count.SetFont(fontM);
    edit_loop_count. SetFont(fontM);
    button_minimize. SetFont(fontM);
    label_kmin.      SetFont(fontM);
    edit_kmin.       SetFont(fontM);
    label_kmax.      SetFont(fontM);
    edit_kmax.       SetFont(fontM);

    button_lossy.            SetFont(fontM);
    button_lossless.         SetFont(fontM);
    label_quality.           SetFont(fontM);
    combobox_method.         SetFont(fontM);
    combobox_image_hint.     SetFont(fontM);
    label_target_size.       SetFont(fontM);
    edit_target_size.        SetFont(fontM);
    label_byte.              SetFont(fontM);
    label_target_PSNR.       SetFont(fontM);
    edit_target_PSNR.        SetFont(fontM);
    label_dB.                SetFont(fontM);
    label_segments.          SetFont(fontM);
    edit_segments.           SetFont(fontM);
    label_SNS_strength.      SetFont(fontM);
    label_filter_strength.   SetFont(fontM);
    label_filter_sharpness.  SetFont(fontM);
    combobox_filter_type.    SetFont(fontM);
    button_autofilter.       SetFont(fontM);
    button_alpha_compression.SetFont(fontM);
    combobox_alpha_filtering.SetFont(fontM);
    label_alpha_quality.     SetFont(fontM);
    label_pass.              SetFont(fontM);
    edit_pass.               SetFont(fontM);
    combobox_preprocessing.  SetFont(fontM);
    label_partitions.        SetFont(fontM);
    edit_partitions.         SetFont(fontM);
    label_partition_limit.   SetFont(fontM);
    button_emulate_jpeg_size.SetFont(fontM);
    button_thread_level.     SetFont(fontM);
    button_low_memory.       SetFont(fontM);
    label_near_lossless.     SetFont(fontM);
    button_exact.            SetFont(fontM);

    label_frame_skip.SetFont(fontM);
    edit_frame_skip. SetFont(fontM);

    label_loop_count.SetText(L"loop count");
    button_minimize. SetText(L"minimize");
    label_kmin.      SetText(L"k min");
    label_kmax.      SetText(L"k max");

    button_lossy.            SetText(L"lossy");
    button_lossless.         SetText(L"lossless");
    label_quality.           SetText(L"quality");
    label_target_size.       SetText(L"target size");
    label_byte.              SetText(L"byte");
    label_target_PSNR.       SetText(L"PSNR");
    label_dB.                SetText(L"dB");
    label_segments.          SetText(L"segments");
    label_SNS_strength.      SetText(L"SNS strength");
    label_filter_strength.   SetText(L"filter strength");
    label_filter_sharpness.  SetText(L"filter sharpness");
    button_autofilter.       SetText(L"auto filter");
    button_alpha_compression.SetText(L"alpha compression");
    label_alpha_quality.     SetText(L"alpha quality");
    label_pass.              SetText(L"pass");
    label_partitions.        SetText(L"partitions");
    label_partition_limit.   SetText(L"partition limit");
    button_emulate_jpeg_size.SetText(L"emulate jpeg size");
    button_thread_level.     SetText(L"thread level");
    button_low_memory.       SetText(L"low memory");
    label_near_lossless.     SetText(L"near lossless");
    button_exact.            SetText(L"exact");

    label_frame_skip.SetText(L"frame skip");

    updown_loop_count.range(0, 999);
    updown_loop_count.position(g_config.options.anim_params.loop_count);

    updown_kmin.range(-1, 99);
    updown_kmin.position(g_config.options.kmin);

    updown_kmax.range(-1, 99);
    updown_kmax.position(g_config.options.kmax);

    if ( g_config.options.minimize_size )
    {
        button_minimize.Check();
    }

    if ( g_config.config.lossless )
    {
        button_lossless.Check();
    }
    else
    {
        button_lossy.Check();
    }

    if ( g_config.config.autofilter )
    {
        button_autofilter.Check();
    }
    if ( g_config.config.alpha_compression )
    {
        button_alpha_compression.Check();
    }
    if ( g_config.config.emulate_jpeg_size )
    {
        button_emulate_jpeg_size.Check();
    }
    if ( g_config.config.thread_level )
    {
        button_thread_level.Check();
    }
    if ( g_config.config.low_memory )
    {
        button_low_memory.Check();
    }
    if ( g_config.config.exact )
    {
        button_exact.Check();
    }

    track_quality.range(0, 100);
    track_quality.position(UINT(g_config.config.quality));

    track_SNS_strength.range(0, 100);
    track_SNS_strength.position(UINT(g_config.config.sns_strength));

    track_filter_strength.range(0, 100);
    track_filter_strength.position(UINT(g_config.config.filter_strength));

    track_filter_sharpness.range(0, 7);
    track_filter_sharpness.position(UINT(g_config.config.filter_sharpness));

    track_alpha_quality.range(0, 100);
    track_alpha_quality.position(UINT(g_config.config.alpha_quality));

    track_partition_limit.range(0, 100);
    track_partition_limit.position(UINT(g_config.config.partition_limit));

    track_near_lossless.range(0, 100);
    track_near_lossless.position(UINT(g_config.config.near_lossless));

    updown_segments.range(1, 4);
    updown_segments.position(g_config.config.segments);

    updown_pass.range(1, 10);
    updown_pass.position(g_config.config.pass);

    updown_partitions.range(0, 3);
    updown_partitions.position(g_config.config.partitions);

    updown_frame_skip.range(0, 10);
    updown_frame_skip.position(g_config.frame_skip);

    combobox_method.AddString(L"method: 0");
    combobox_method.AddString(L"method: 1");
    combobox_method.AddString(L"method: 2");
    combobox_method.AddString(L"method: 3");
    combobox_method.AddString(L"method: 4");
    combobox_method.AddString(L"method: 5");
    combobox_method.AddString(L"method: 6");
    combobox_method.Select(g_config.config.method);

    combobox_image_hint.AddString(L"image hint: DEFAULT");
    combobox_image_hint.AddString(L"image hint: PICTURE");
    combobox_image_hint.AddString(L"image hint: PHOTO");
    combobox_image_hint.AddString(L"image hint: GRAPH");
    combobox_image_hint.Select(g_config.config.image_hint);

    combobox_filter_type.AddString(L"filtering type: simple");
    combobox_filter_type.AddString(L"filtering type: strong");
    combobox_filter_type.Select(g_config.config.filter_type);

    combobox_alpha_filtering.AddString(L"alpha filtering: none");
    combobox_alpha_filtering.AddString(L"alpha filtering: fast");
    combobox_alpha_filtering.AddString(L"alpha filtering: best");
    combobox_alpha_filtering.Select(g_config.config.alpha_filtering);

    combobox_preprocessing.AddString(L"preprocessing: none");
    combobox_preprocessing.AddString(L"preprocessing: segment-smooth");
    combobox_preprocessing.AddString(L"preprocessing: pseudo-random dithering");
    combobox_preprocessing.Select(g_config.config.preprocessing);

    wchar_t buf [16];

    ::StringCchPrintfW(buf, 16, L"%d", g_config.config.target_size);
    edit_target_size.SetText(buf);

    ::StringCchPrintfW(buf, 16, L"%d", g_config.config.target_PSNR);
    edit_target_PSNR.SetText(buf);

    ::StringCchPrintfW(buf, 16, L"%d", updown_loop_count.position());
    edit_loop_count.SetText(buf);

    ::StringCchPrintfW(buf, 16, L"%d", updown_kmin.position());
    edit_kmin.SetText(buf);

    ::StringCchPrintfW(buf, 16, L"%d", updown_kmax.position());
    edit_kmax.SetText(buf);

    ::StringCchPrintfW(buf, 16, L"%d", updown_segments.position());
    edit_segments.SetText(buf);

    ::StringCchPrintfW(buf, 16, L"%d", updown_pass.position());
    edit_pass.SetText(buf);

    ::StringCchPrintfW(buf, 16, L"%d", updown_partitions.position());
    edit_partitions.SetText(buf);

    ::StringCchPrintfW(buf, 16, L"%d", updown_frame_skip.position());
    edit_frame_skip.SetText(buf);

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK SettingWnd::OnDestroy()
{
    g_config.options.minimize_size    = button_minimize.IsChecked();
    g_config.config.autofilter        = button_autofilter.IsChecked();
    g_config.config.alpha_compression = button_alpha_compression.IsChecked();
    g_config.config.emulate_jpeg_size = button_emulate_jpeg_size.IsChecked();
    g_config.config.thread_level      = button_thread_level.IsChecked();
    g_config.config.low_memory        = button_low_memory.IsChecked();
    g_config.config.exact             = button_exact.IsChecked();

    wchar_t buf [16];

    edit_target_size.GetText(buf, 16);
    g_config.config.target_size = ::StrToIntW(buf);

    edit_target_PSNR.GetText(buf, 16);
    g_config.config.target_PSNR = float(::StrToIntW(buf));

    is_alive = false;

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK SettingWnd::OnSize
(
    INT16 cx, INT16 cy
)
{
    (cx); (cy);

    label_loop_count. Bounds(16 +   0, 16, 68, 16);
    edit_loop_count.  Bounds(16 +  68, 16, 32, 16);
    updown_loop_count.Bounds(16 + 104, 15, 16, 20);
    button_minimize.  Bounds(16 + 144, 16, 68, 16);
    label_kmin.       Bounds(16 + 228, 16, 40, 16);
    edit_kmin.        Bounds(16 + 268, 16, 32, 16);
    updown_kmin.      Bounds(16 + 304, 15, 16, 20);
    label_kmax.       Bounds(16 + 336, 16, 40, 16);
    edit_kmax.        Bounds(16 + 376, 16, 32, 16);
    updown_kmax.      Bounds(16 + 412, 15, 16, 20);

    button_lossy.   Bounds(16 +   0, 68,  64,  16);
    button_lossless.Bounds(16 +  64, 68,  64,  16);
    label_quality.  Bounds(16 + 144, 68,  44,  16);
    track_quality.  Bounds(16 + 188, 68, 200,  24);
    combobox_method.Bounds(16 + 404, 68,  96, 128);

    combobox_image_hint.Bounds(16 + 0, 104, 192, 128);

    label_target_size.Bounds(16 +   0, 142, 64, 16);
    edit_target_size. Bounds(16 +  64, 140, 96, 16);
    label_byte.       Bounds(16 + 168, 142, 32, 16);
    label_target_PSNR.Bounds(16 + 216, 142, 32, 16);
    edit_target_PSNR. Bounds(16 + 252, 140, 32, 16);
    label_dB.         Bounds(16 + 292, 142, 32, 16);

    label_segments.        Bounds(16 +   0, 176,  64,  16);
    edit_segments.         Bounds(16 +  64, 176,  32,  16);
    updown_segments.       Bounds(16 + 100, 175,  16,  20);
    label_SNS_strength.    Bounds(16 +   0, 212,  88,  16);
    track_SNS_strength.    Bounds(16 +  88, 212, 200,  24);
    label_filter_strength. Bounds(16 +   0, 248,  88,  16);
    track_filter_strength. Bounds(16 +  88, 248, 200,  24);
    label_filter_sharpness.Bounds(16 +   0, 284,  88,  16);
    track_filter_sharpness.Bounds(16 +  88, 284, 200,  24);
    combobox_filter_type.  Bounds(16 +   0, 320, 144, 128);
    button_autofilter.     Bounds(16 + 160, 322,  72,  16);

    button_alpha_compression.Bounds(16 +   0, 356, 120,  24);
    combobox_alpha_filtering.Bounds(16 + 136, 356, 144, 128);
    label_alpha_quality.     Bounds(16 + 296, 356,  80,  16);
    track_alpha_quality.     Bounds(16 + 376, 356, 200,  24);

    label_pass. Bounds(16 +  0, 392, 32, 16);
    edit_pass.  Bounds(16 + 32, 392, 32, 16);
    updown_pass.Bounds(16 + 68, 391, 16, 20);

    combobox_preprocessing.Bounds(16 + 0, 428, 256, 128);

    label_partitions.     Bounds(16 +   0, 464,  64, 16);
    edit_partitions.      Bounds(16 +  64, 464,  32, 16);
    updown_partitions.    Bounds(16 + 100, 463,  16, 20);
    label_partition_limit.Bounds(16 + 132, 464,  80, 16);
    track_partition_limit.Bounds(16 + 212, 464, 200, 24);

    button_emulate_jpeg_size.Bounds(16 +   0, 500, 112, 16);
    button_thread_level.     Bounds(16 + 136, 500,  88, 16);
    button_low_memory.       Bounds(16 + 240, 500,  88, 16);

    label_near_lossless.Bounds(16 +  0, 536,  80, 16);
    track_near_lossless.Bounds(16 + 80, 536, 200, 24);

    button_exact.Bounds(16 + 0, 570, 48, 16);

    label_frame_skip. Bounds(16 +   0, 606, 64, 16);
    edit_frame_skip.  Bounds(16 +  64, 606, 48, 16);
    updown_frame_skip.Bounds(16 + 116, 606, 16, 20);

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK SettingWnd::OnPaint
(
    HWND hwnd
)
{
    PAINTSTRUCT ps;
    const auto hdc = ::BeginPaint(hwnd, &ps);

    ::FillRect(hdc, &ps.rcPaint, GetSysColorBrush(COLOR_BTNFACE));

    ::MoveToEx(hdc, ps.rcPaint.left  + 16, 52, nullptr);
    ::LineTo  (hdc, ps.rcPaint.right - 16, 52);

    wchar_t buf [64];
    ::StringCchPrintfW(buf, 64, L"%s v%s", APP_NAME, VERSION);

    RECT rc
    {
        ps.rcPaint.right  - 128,
        ps.rcPaint.bottom -  28,
        ps.rcPaint.right  -  14,
        ps.rcPaint.bottom -  14,
    };

    const auto obj = ::SelectObject(hdc, HFONT(fontM));
    ::SetBkMode(hdc,TRANSPARENT);
    ::DrawTextW(hdc, buf, ::lstrlenW(buf), &rc, DT_RIGHT | DT_VCENTER);
    ::SelectObject(hdc, obj);

    ::EndPaint(hwnd, &ps);

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK SettingWnd::OnNotify
(
    const NMHDR* nmhdr
)
{
    const auto nmud = LPNMUPDOWN(nmhdr);
    if ( nmud->hdr.code != UDN_DELTAPOS )
    {
        return 0;
    }

    const auto position = nmud->iPos + nmud->iDelta;

    wchar_t buf [16];
    ::StringCchPrintfW(buf, 16, L"%d", position);

    switch ( nmhdr->idFrom )
    {
        case CTRL::UPDOWN_LOOP_COUNT:
        {
            if ( 0 <= position && position <= 999 )
            {
                g_config.options.anim_params.loop_count = position;
                edit_loop_count.SetText(buf);
            }
            break;
        }
        case CTRL::UPDOWN_SEGMENTS:
        {
            if ( 1 <= position && position <= 4 )
            {
                g_config.config.segments = position;
                edit_segments.SetText(buf);
            }
            break;
        }
        case CTRL::UPDOWN_PASS:
        {
            if ( 1 <= position && position <= 10 )
            {
                g_config.config.pass = position;
                edit_pass.SetText(buf);
            }
            break;
        }
        case CTRL::UPDOWN_PARTITIONS:
        {
            if ( 0 <= position && position <= 3 )
            {
                g_config.config.partitions = position;
                edit_partitions.SetText(buf);
            }
            break;
        }
        case CTRL::UPDOWN_FRAME_SKIP:
        {
            if ( 0 <= position && position <= 10 )
            {
                g_config.frame_skip = position;
                edit_frame_skip.SetText(buf);
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

LRESULT CALLBACK SettingWnd::OnCommand
(
    UINT16 id, UINT16 code, HWND ctrl
)
{
    UNREFERENCED_PARAMETER(code);
    UNREFERENCED_PARAMETER(ctrl);

    switch ( id )
    {
        case CTRL::BUTTON_LOSSY:
        case CTRL::BUTTON_LOSSLESS:
        {
            g_config.config.lossless = button_lossy.IsChecked() ? 0 : 1;
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
        case CTRL::COMBOBOX_IMAGE_HINT:
        {
            if ( code == CBN_SELCHANGE )
            {
                g_config.config.image_hint = WebPImageHint(combobox_image_hint.SelectedIndex());
            }
            break;
        }
        case CTRL::COMBOBOX_FILTER_TYPE:
        {
            if ( code == CBN_SELCHANGE )
            {
                g_config.config.filter_type = combobox_filter_type.SelectedIndex();
            }
            break;
        }
        case CTRL::COMBOBOX_ALPHA_FILTERING:
        {
            if ( code == CBN_SELCHANGE )
            {
                g_config.config.alpha_filtering = combobox_alpha_filtering.SelectedIndex();
            }
            break;
        }
        case CTRL::COMBOBOX_PREPROCESSING:
        {
            if ( code == CBN_SELCHANGE )
            {
                g_config.config.preprocessing = combobox_preprocessing.SelectedIndex();
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

LRESULT CALLBACK SettingWnd::OnHScroll
(
    WORD request, WORD position, HWND ctrl
)
{
    if ( request != SB_THUMBPOSITION && request != SB_THUMBTRACK )
    {
        return 0;
    }

    if ( ctrl == track_quality.handle() )
    {
        g_config.config.quality = float(position);
    }
    else if ( ctrl == track_SNS_strength.handle() )
    {
        g_config.config.sns_strength = position;
    }
    else if ( ctrl == track_filter_strength.handle() )
    {
        g_config.config.filter_strength = position;
    }
    else if ( ctrl == track_filter_sharpness.handle() )
    {
        g_config.config.filter_sharpness = position;
    }
    else if ( ctrl == track_alpha_quality.handle() )
    {
        g_config.config.alpha_quality = position;
    }
    else if ( ctrl == track_partition_limit.handle() )
    {
        g_config.config.partition_limit = position;
    }
    else if ( ctrl == track_near_lossless.handle() )
    {
        g_config.config.near_lossless = position;
    }

    return 0;
}

//---------------------------------------------------------------------------//

// SettingWnd.cpp