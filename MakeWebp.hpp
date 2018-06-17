#pragma once

//---------------------------------------------------------------------------//
//
// MakeWebp.hpp
//  Copyright (C) 2018 tapetums
//
//---------------------------------------------------------------------------//

#include <Bitmap.hpp>

#include "Global.hpp"

//---------------------------------------------------------------------------//

bool decode_picture(const std::wstring& filename, tapetums::Bitmap* bitmap);
bool make_animated_webp(HWND hwnd, const std::vector<ImageData> images, const std::wstring filename);

//---------------------------------------------------------------------------//

// MakeWebp.hpp