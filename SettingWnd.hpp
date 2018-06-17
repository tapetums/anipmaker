#pragma once

//---------------------------------------------------------------------------//
//
// SettingWnd.hpp
//  Copyright (C) 2018 tapetums
//
//---------------------------------------------------------------------------//

#include <UWnd.hpp>
#include <CtrlWnd.hpp>
#include <Font.hpp>

//---------------------------------------------------------------------------//
// Classes
//---------------------------------------------------------------------------//

class SettingWnd : public tapetums::UWnd
{
    using super = UWnd;

    enum CTRL : INT16
    {
        LABEL_LOOP_COUNT, EDIT_LOOP_COUNT, UPDOWN_LOOP_COUNT,
        BUTTON_MINIMIZE,
        LABEL_KMIN, EDIT_KMIN, UPDOWN_KMIN,
        LABEL_KMAX, EDIT_KMAX, UPDOWN_KMAX,

        BUTTON_LOSSY, BUTTON_LOSSLESS,
        LABEL_QUALITY, TRACK_QUALITY,
        COMBOBOX_METHOD,
        COMBOBOX_IMAGE_HINT,
        LABEL_TARGET_SIZE, EDIT_TARGET_SIZE, LABEL_BYTE,
        LABEL_TARGET_PSNR, EDIT_TARGET_PSNR, LABEL_DB,
        LABEL_SEGMENTS, EDIT_SEGMENTS, UPDOWN_SEGMENTS,
        LABEL_SNS_STRENGTH, TRACK_SNS_STRENGTH,
        LABEL_FILTER_STRENGTH, TRACK_FILTER_STRENGTH,
        LABEL_FILTER_SHARPNESS, TRACK_FILTER_SHARPNESS,
        COMBOBOX_FILTER_TYPE,
        BUTTON_AUTO_FILTER,
        BUTTON_ALPHA_COMPRESSION, COMBOBOX_ALPHA_FILTERING,
        LABEL_ALPHA_QUALITY, TRACK_ALPHA_QUALITY,
        LABEL_PASS, EDIT_PASS, UPDOWN_PASS,
        COMBOBOX_PREPROCESSING,
        LABEL_PARTITIONS, EDIT_PARTITIONS, UPDOWN_PARTITIONS,
        LABEL_PARTITION_LIMIT, TRACK_PARTITION_LIMIT,
        BUTTON_EMULATE_JPEG_SIZE,
        BUTTON_THREAD_LEVEL,
        BUTTON_LOW_MEMORY,
        LABEL_NEAR_LOSSLESS, TRACK_NEAR_LOSSLESS,
        BUTTON_EXACT,

        LABEL_FRAME_SKIP, EDIT_FRAME_SKIP, UPDOWN_FRAME_SKIP,
    };

private:
    tapetums::Font fontM;

    tapetums::LabelWnd    label_loop_count;
    tapetums::LabelWnd    edit_loop_count;
    tapetums::UpDownWnd   updown_loop_count;
    tapetums::BtnWnd      button_minimize;
    tapetums::LabelWnd    label_kmin;
    tapetums::LabelWnd    edit_kmin;
    tapetums::UpDownWnd   updown_kmin;
    tapetums::LabelWnd    label_kmax;
    tapetums::LabelWnd    edit_kmax;
    tapetums::UpDownWnd   updown_kmax;

    tapetums::BtnWnd      button_lossy;
    tapetums::BtnWnd      button_lossless;
    tapetums::LabelWnd    label_quality;
    tapetums::TrackbarWnd track_quality;
    tapetums::ComboBox    combobox_method;
    tapetums::ComboBox    combobox_image_hint;
    tapetums::LabelWnd    label_target_size;
    tapetums::EditWnd     edit_target_size;
    tapetums::LabelWnd    label_byte;
    tapetums::LabelWnd    label_target_PSNR;
    tapetums::EditWnd     edit_target_PSNR;
    tapetums::LabelWnd    label_dB;
    tapetums::LabelWnd    label_segments;
    tapetums::LabelWnd    edit_segments;
    tapetums::UpDownWnd   updown_segments;
    tapetums::LabelWnd    label_SNS_strength;
    tapetums::TrackbarWnd track_SNS_strength;
    tapetums::LabelWnd    label_filter_strength;
    tapetums::TrackbarWnd track_filter_strength;
    tapetums::LabelWnd    label_filter_sharpness;
    tapetums::TrackbarWnd track_filter_sharpness;
    tapetums::ComboBox    combobox_filter_type;
    tapetums::BtnWnd      button_autofilter;
    tapetums::BtnWnd      button_alpha_compression;
    tapetums::ComboBox    combobox_alpha_filtering;
    tapetums::LabelWnd    label_alpha_quality;
    tapetums::TrackbarWnd track_alpha_quality;
    tapetums::LabelWnd    label_pass;
    tapetums::LabelWnd    edit_pass;
    tapetums::UpDownWnd   updown_pass;
    tapetums::ComboBox    combobox_preprocessing;
    tapetums::LabelWnd    label_partitions;
    tapetums::LabelWnd    edit_partitions;
    tapetums::UpDownWnd   updown_partitions;
    tapetums::LabelWnd    label_partition_limit;
    tapetums::TrackbarWnd track_partition_limit;
    tapetums::BtnWnd      button_emulate_jpeg_size;
    tapetums::BtnWnd      button_thread_level;
    tapetums::BtnWnd      button_low_memory;
    tapetums::LabelWnd    label_near_lossless;
    tapetums::TrackbarWnd track_near_lossless;
    tapetums::BtnWnd      button_exact;

    tapetums::LabelWnd    label_frame_skip;
    tapetums::LabelWnd    edit_frame_skip;
    tapetums::UpDownWnd   updown_frame_skip;

public:
    bool is_alive;

public:
    SettingWnd(HWND parent);

public:
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;

private:
    LRESULT CALLBACK OnCreate (CREATESTRUCT* cs);
    LRESULT CALLBACK OnDestroy();
    LRESULT CALLBACK OnPaint  (HWND hwnd);
    LRESULT CALLBACK OnSize   (INT16 cx, INT16 cy);
    LRESULT CALLBACK OnNotify (const NMHDR* nmhdr);
    LRESULT CALLBACK OnCommand(UINT16 id, UINT16 code, HWND ctrl);
    LRESULT CALLBACK OnHScroll(WORD request, WORD position, HWND ctrl);
};

//---------------------------------------------------------------------------//

// SettingWnd.hpp