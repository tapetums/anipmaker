#pragma once

//---------------------------------------------------------------------------//
//
// PreviewWnd.hpp
//  Copyright (C) 2018 tapetums
//
//---------------------------------------------------------------------------//

#include <UWnd.hpp>
#include <Bitmap.hpp>

//---------------------------------------------------------------------------//
// Classes
//---------------------------------------------------------------------------//

class PreviewWnd : public tapetums::UWnd
{
    using super = UWnd;

public:
    enum { WM_SELCTED_INDEX_CHANGED = WM_APP + 1 };

private:
    tapetums::Bitmap bitmap;

public:
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;

private:
    LRESULT CALLBACK OnPaint (HWND hwnd);
    LRESULT CALLBACK OnDecode(SIZE_T selected_index);
};

//---------------------------------------------------------------------------//

// PreviewWnd.hpp