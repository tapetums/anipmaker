//---------------------------------------------------------------------------//
//
// PreviewWnd.cpp
//  Copyright (C) 2018 tapetums
//
//---------------------------------------------------------------------------//

#include "PreviewWnd.hpp"

#include "Global.hpp"
#include "MainWnd.hpp"
#include "MakeWebp.hpp"

//---------------------------------------------------------------------------//
// Window Procedure
//---------------------------------------------------------------------------//

LRESULT CALLBACK PreviewWnd::WndProc
(
    HWND hwnd, UINT msg, WPARAM wp, LPARAM lp
)
{
    if ( msg == WM_SELCTED_INDEX_CHANGED ) { return OnDecode(SIZE_T(lp)); }

    switch ( msg )
    {
        case WM_PAINT: { return OnPaint(hwnd); }
        default:       { return super::WndProc(hwnd, msg, wp, lp); }
    }
}

//---------------------------------------------------------------------------//
// Internal Methods
//---------------------------------------------------------------------------//

LRESULT CALLBACK PreviewWnd::OnPaint
(
    HWND hwnd
)
{
    PAINTSTRUCT ps;
    const auto hdc = ::BeginPaint(hwnd, &ps);

    // 背景を塗りつぶす
    ::FillRect(hdc, &ps.rcPaint, GetStockBrush(BLACK_BRUSH));

    if ( bitmap.size() )
    {
        const auto sw = ps.rcPaint.right  - ps.rcPaint.left;
        const auto sh = ps.rcPaint.bottom - ps.rcPaint.top;
        const auto iw = bitmap.width();
        const auto ih = bitmap.height();

        // 縮尺を決める
        INT32 x, y, w, h;
        if ( sw * ih < sh * iw )
        {
            w = sw;
            h = ih * sw / iw;
            x = 0;
            y = (sh - h) / 2;
        }
        else
        {
            w = iw * sh / ih;
            h = sh;
            x = (sw - w) / 2;
            y = 0;
        }

        // 画像を描画する
        ::SetStretchBltMode(hdc, HALFTONE);
        ::SetBrushOrgEx(hdc, 0, 0, nullptr);
        ::StretchDIBits
        (
            hdc, x, y, w, h,
            0, 0, iw, ih, bitmap.pbits(), bitmap.info(),
            DIB_RGB_COLORS, SRCCOPY
        );
    }

    ::EndPaint(hwnd, &ps);

    return 0;
}

//---------------------------------------------------------------------------//

LRESULT CALLBACK PreviewWnd::OnDecode
(
    SIZE_T selected_index
)
{
    bitmap.Dispose();

    if ( selected_index < g_images.size() )
    {
        const auto& filename = g_images[selected_index].filename;

        decode_picture(filename, &bitmap);
    }

    Refresh();

    return 0;
}

//---------------------------------------------------------------------------//

// PreviewWnd.cpp