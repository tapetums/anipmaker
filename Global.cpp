//---------------------------------------------------------------------------//
//
// Global.cpp
//  Copyright (C) 2018 tapetums
//
//---------------------------------------------------------------------------//

#include "Global.hpp"

//---------------------------------------------------------------------------//
// Global Variables
//---------------------------------------------------------------------------//

std::vector<ImageData> g_images;

bool g_abort { true };

Config g_config
{
    {
        // options
        {
            0xFFFFFFFF,    // bgcolor
            0,             // loop_count
        },
        true,              // minimize_size
        -1,                // kmin
        -1,                // kmax
        false,             // allow_mixed
        false,             // verbose
        0x00000000,        // Padding for later use.
        0x00000001,        // Padding for later use.
        0x00000002,        // Padding for later use.
        0x00000003,        // Padding for later use.
    },
    {
        // config
        0,                 // lossless
        75.0f,             // quality
        4,                 // method
        WEBP_HINT_DEFAULT, // image_hint
        0,                 // target_size
        0.0f,              // target_PSNR
        4,                 // segments
        50,                // sns_strength
        60,                // filter_strength
        0,                 // filter_sharpness
        1,                 // filter_type
        0,                 // autofilter
        1,                 // alpha_compression
        1,                 // alpha_filtering
        100,               // alpha_quality
        1,                 // pass
        false,             // show_compressed
        0,                 // preprocessing
        0,                 // partitions
        0,                 // partition_limit
        false,             // emulate_jpeg_size
        1,                 // thread_level
        0,                 // low_memory
        100,               // near_lossless
        0,                 // exact
        0,                 // use_delta_palette
        0,                 // use_sharp_yuv
        0x00000000,        // padding for later use
        0x00000001,        // padding for later use
    },
    0,                     // frame_skip
};

//---------------------------------------------------------------------------//

// Global.cpp