//---------------------------------------------------------------------------//
//
// MakeAnimatedWebp.cpp
//  Copyright (C) 2018 tapetums
//
//---------------------------------------------------------------------------//

#include "MakeWebp.hpp"

#include <memory>

#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/types.h"
#include "webp/mux.h"

#if INTPTR_MAX == INT64_MAX
  #if defined(_DEBUG) || defined(DEBUG)
    #pragma comment(lib, "lib/x64/libwebp_debug.lib")
    #pragma comment(lib, "lib/x64/libwebpmux_debug.lib")
  #else
    #pragma comment(lib, "lib/x64/libwebp.lib")
    #pragma comment(lib, "lib/x64/libwebpmux.lib")
  #endif
#elif INTPTR_MAX == INT32_MAX
  #if defined(_DEBUG) || defined(DEBUG)
    #pragma comment(lib, "lib/x86/libwebp_debug.lib")
    #pragma comment(lib, "lib/x86/libwebpmux_debug.lib")
  #else
    #pragma comment(lib, "lib/x86/libwebp.lib")
    #pragma comment(lib, "lib/x86/libwebpmux.lib")
  #endif
#else
  #error Unknown architecture
#endif

#include <File.hpp>
#include <WIC.hpp>

#include "MainWnd.hpp"

//---------------------------------------------------------------------------//
// RAII Classes
//---------------------------------------------------------------------------//

struct WebPPictureGuard
{
    WebPPicture& picture;

    WebPPictureGuard
    (
        WebPPicture& picture, int32_t width, int32_t height
    )
    : picture(picture)
    {
        const auto init = WebPPictureInit(&picture);
        if ( ! init ) { throw; }

        picture.width    = width;
        picture.height   = height;
        picture.use_argb = 1;
    }

    ~WebPPictureGuard()
    {
        WebPPictureFree(&picture);
    }
};

//---------------------------------------------------------------------------//
// Utility Functions
//---------------------------------------------------------------------------//

std::vector<uint8_t> read_data
(
    const std::wstring& filename
)
{
    std::vector<uint8_t> data;

    using tapetums::File;

    File file;

    const auto result = file.Open
    (
        filename.c_str(),
        File::ACCESS::READ, File::SHARE::READ, File::OPEN::EXISTING
    );
    if ( result )
    {
        data.resize(size_t(file.size()));
        file.Read(data.data(), data.size());
    }

    return data;
}

//---------------------------------------------------------------------------//

bool decode_picture
(
    const std::wstring& filename, tapetums::Bitmap* bitmap
)
{
    if ( bitmap == nullptr ) { return false; }

    bitmap->Dispose();

    const auto hr = tapetums::WIC::Load(filename.c_str(), bitmap);
    if ( SUCCEEDED(hr) )
    {
        return true;
    }

    const auto data = read_data(filename);
    if ( data.size() == 0 )
    {
        return false;
    }

    int width, height;
    const auto webp = WebPDecodeBGRA
    (
        data.data(), data.size(), &width, &height
    );
    if ( webp == nullptr )
    {
        return false;
    }

    bitmap->Create(width, -height, 32, 0);
    ::memcpy(bitmap->pbits(), webp, bitmap->size());

    WebPFree(webp);

    return true;
}

//---------------------------------------------------------------------------//

void determine_size
(
    const std::wstring& filename, int32_t* width, int32_t* height
)
{
    tapetums::Bitmap bitmap;

    decode_picture(filename, &bitmap);

    *width  = bitmap.width();
    *height = bitmap.height();
}

//---------------------------------------------------------------------------//

bool make_animated_webp
(
    HWND hwnd, const std::vector<ImageData> images, const std::wstring filename
)
{
    int32_t width, height;
    determine_size(images[0].filename, &width, &height);

    WebPAnimEncoderOptions options;
    {
        const auto init = WebPAnimEncoderOptionsInit(&options);
        if ( ! init ) { return false; }

        options = g_config.options;
    }

    const auto encoder = WebPAnimEncoderNew(width, height, &options);
    if ( encoder == nullptr ) { return false; }

    WebPConfig config;
    {
        const auto init = WebPConfigInit(&config);
        if ( ! init ) { return false; }

        config = g_config.config;

        const auto valid = WebPValidateConfig(&config);
        if ( ! valid ) { return false; }
    }

    const auto frame_skip = g_config.frame_skip;

    size_t index { 0 };
    const auto size { images.size() };

    int32_t skip { 0 };
    int32_t timestamp_ms { 0 };

    for ( const auto& image : images )
    {
        if ( g_abort ) { break; }

        ::PostMessage(hwnd, MainWnd::WM_PROCESS, WPARAM(index), LPARAM(size));

        if ( skip > 0 )
        {
            timestamp_ms += image.duration;
            ++index;

            --skip;

            continue;
        }

        tapetums::Bitmap bitmap;

        const auto decoded = decode_picture(image.filename, &bitmap);
        if ( ! decoded ) { continue; }

        WebPPicture picture;
        WebPPictureGuard guard(picture, width, height);

        const auto imported = WebPPictureImportBGRA
        (
            &picture, bitmap.pbits(), bitmap.stride()
        );
        if ( ! imported ) { continue; }

        const auto added = WebPAnimEncoderAdd
        (
            encoder, &picture, timestamp_ms, &config
        );
        if ( ! added ) { continue; }

        timestamp_ms += image.duration;
        ++index;

        if ( skip <= 0 )
        {
            skip = frame_skip;
        }
    }

    ::PostMessage(hwnd, MainWnd::WM_PROCESS, WPARAM(index), LPARAM(size));
    WebPAnimEncoderAdd(encoder, nullptr, timestamp_ms, nullptr);

    using tapetums::File;

    File file;
    {
        const auto opened = file.Open
        (
            filename.c_str(),
            File::ACCESS::WRITE, File::SHARE::WRITE, File::OPEN::OR_CREATE
        );
        if ( ! opened ) { return false; }
    }

    WebPData webp_data;
    {
        const auto assembled = WebPAnimEncoderAssemble(encoder, &webp_data);
        WebPAnimEncoderDelete(encoder);
        if ( ! assembled ) { return false; }

        file.Write(webp_data.bytes, webp_data.size);

        WebPDataClear(&webp_data);
    }

    return true;
}

//---------------------------------------------------------------------------//

// MakeWebp.cpp