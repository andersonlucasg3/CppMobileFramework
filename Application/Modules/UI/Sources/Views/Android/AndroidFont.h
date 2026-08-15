#pragma once

#include "Views/UILabel.h"        // ETextAlignment
#include <cstdint>
#include <cstdio>

struct stbtt_fontinfo;
struct SAndroidFramebuffer;

// ---------------------------------------------------------------------------
// SAndroidFont — loads Roboto-Regular.ttf once and provides measure + render.
//
// Usage:
//   SAndroidFont::Shared().Initialize();  // once, from the main thread
//   SAndroidFont::Shared().SetPixelSize(56.0f);
//   SAndroidFont::Shared().RenderString(fb, "Hello", x, y, w, 0xFFFFFFFF, ETextAlignment::Center);
// ---------------------------------------------------------------------------

struct SAndroidFont
{
    stbtt_fontinfo* info      = nullptr;
    unsigned char*  fontData  = nullptr;
    size_t          dataSize  = 0;
    float           pixelSize = 56.0f;
    float           scale     = 1.0f;
    int             ascent    = 0;
    int             descent   = 0;
    int             lineGap   = 0;

    static SAndroidFont& Shared()
    {
        static SAndroidFont instance;
        return instance;
    }

    /// Load the font file. Tries Roboto-Regular.ttf first, then falls back
    /// to any readable *.ttf in /system/fonts/. Returns true on success.
    bool Initialize();

    /// Recompute scale / metrics for a given pixel height.
    void SetPixelSize(float px);

    /// Measure a UTF-8 string. outW and outH are the bounding width and
    /// height in pixels at the current pixel size.
    void MeasureString(const char* text, int* outW, int* outH);

    /// Render a UTF-8 string into a framebuffer.
    ///   fb      : locked framebuffer
    ///   text    : null-terminated UTF-8
    ///   x, y    : top-left of the container rectangle (y = top of container)
    ///   containerW : width of the container (for alignment)
    ///   color   : ARGB text colour
    ///   align   : horizontal alignment within [x, x+containerW)
    void RenderString(SAndroidFramebuffer& fb, const char* text,
                      int x, int y, int containerW,
                      uint32_t color, ETextAlignment align);

    ~SAndroidFont();

private:
    bool TryLoadFile(const char* path);
};
