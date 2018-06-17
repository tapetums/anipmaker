#pragma once

//---------------------------------------------------------------------------//
//
// Global.hpp
//  Copyright (C) 2018 tapetums
//
//---------------------------------------------------------------------------//

#include <string>
#include <vector>

#include "webp/encode.h"
#include "webp/mux.h"

//---------------------------------------------------------------------------//
// Structures
//---------------------------------------------------------------------------//

struct ImageData
{
    std::wstring filename;
    int32_t      duration;
    int32_t      frame_number;
};

//---------------------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------------------//

extern std::vector<ImageData> g_images;

extern bool g_abort;

struct Config
{
    WebPAnimEncoderOptions options;
    WebPConfig             config;
    int                    frame_skip;
};

extern Config g_config;

//---------------------------------------------------------------------------//

// Global.hpp