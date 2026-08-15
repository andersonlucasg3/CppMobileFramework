#define STB_TRUETYPE_IMPLEMENTATION
#include "Graphics/ThirdParty/stb_truetype.h"

#include "AndroidFont.h"
#include "AndroidFramebuffer.h"

#include "Logger/Logger.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>

// ---------------------------------------------------------------------------
// Destructor (defined here because stbtt_fontinfo is complete).
// ---------------------------------------------------------------------------
SAndroidFont::~SAndroidFont()
{
    delete info;
    delete[] fontData;
    info = nullptr;
    fontData = nullptr;
}

// ---------------------------------------------------------------------------
// Simple UTF-8 → codepoint decoder.
// Advances *p past one codepoint and returns the next byte position.
// Stores the codepoint in *cp (or -1 on error/EOF).
// ---------------------------------------------------------------------------
static const char* DecodeUTF8(const char* p, int* cp)
{
    *cp = -1;
    if (!p || !*p) return p;

    unsigned char c = (unsigned char)*p;
    int len;
    int codepoint;

    if (c < 0x80)
    {
        codepoint = c;
        len = 1;
    }
    else if ((c & 0xE0) == 0xC0)
    {
        codepoint = c & 0x1F;
        len = 2;
    }
    else if ((c & 0xF0) == 0xE0)
    {
        codepoint = c & 0x0F;
        len = 3;
    }
    else if ((c & 0xF8) == 0xF0)
    {
        codepoint = c & 0x07;
        len = 4;
    }
    else
    {
        // Invalid lead byte; skip it
        return p + 1;
    }

    for (int i = 1; i < len; ++i)
    {
        unsigned char trail = (unsigned char)p[i];
        if ((trail & 0xC0) != 0x80)
        {
            // Invalid trail byte; return what we have so far
            len = i;
            break;
        }
        codepoint = (codepoint << 6) | (trail & 0x3F);
    }

    *cp = codepoint;
    return p + len;
}

// ---------------------------------------------------------------------------
// Font file loading
// ---------------------------------------------------------------------------

bool SAndroidFont::TryLoadFile(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (sz <= 0)
    {
        fclose(f);
        return false;
    }

    delete[] fontData;
    fontData = new unsigned char[sz];
    dataSize = (size_t)sz;

    size_t read = fread(fontData, 1, dataSize, f);
    fclose(f);

    if (read != dataSize)
    {
        delete[] fontData;
        fontData = nullptr;
        dataSize = 0;
        return false;
    }

    delete info;
    info = new stbtt_fontinfo;

    int offset = stbtt_GetFontOffsetForIndex(fontData, 0);
    if (offset < 0 || !stbtt_InitFont(info, fontData, offset))
    {
        GLogger.Error("SAndroidFont: stbtt_InitFont failed for {}", path);
        delete info;
        info = nullptr;
        return false;
    }

    GLogger.Log("SAndroidFont: loaded {} ({} bytes)", path, (int)dataSize);
    return true;
}

bool SAndroidFont::Initialize()
{
    if (info) return true; // already loaded

    // Try Roboto-Regular first
    static const char* kFontPaths[] = {
        "/system/fonts/Roboto-Regular.ttf",
        "/system/fonts/DroidSans.ttf",
        "/system/fonts/NotoSans-Regular.ttf",
        nullptr
    };

    for (int i = 0; kFontPaths[i]; ++i)
    {
        if (TryLoadFile(kFontPaths[i]))
        {
            SetPixelSize(pixelSize);
            return true;
        }
    }

    // Fallback: scan /system/fonts for any .ttf
    DIR* d = opendir("/system/fonts");
    if (d)
    {
        struct dirent* ent;
        while ((ent = readdir(d)) != nullptr)
        {
            const char* name = ent->d_name;
            size_t len = strlen(name);
            if (len > 4 &&
                    (name[len-4] == '.' || name[len-4] == '.') &&
                    (name[len-3] == 't' || name[len-3] == 'T') &&
                    (name[len-2] == 't' || name[len-2] == 'T') &&
                    (name[len-1] == 'f' || name[len-1] == 'F'))
            {
                char fullPath[512];
                snprintf(fullPath, sizeof(fullPath), "/system/fonts/%s", name);
                if (TryLoadFile(fullPath))
                {
                    closedir(d);
                    SetPixelSize(pixelSize);
                    return true;
                }
            }
        }
        closedir(d);
    }

    GLogger.Error("SAndroidFont: no TTF font found in /system/fonts/");
    return false;
}

// ---------------------------------------------------------------------------
// Scale
// ---------------------------------------------------------------------------

void SAndroidFont::SetPixelSize(float px)
{
    pixelSize = px;
    if (!info) return;

    scale = stbtt_ScaleForPixelHeight(info, px);
    stbtt_GetFontVMetrics(info, &ascent, &descent, &lineGap);
}

// ---------------------------------------------------------------------------
// Measure
// ---------------------------------------------------------------------------

void SAndroidFont::MeasureString(const char* text, int* outW, int* outH)
{
    *outW = 0;
    *outH = 0;

    if (!info || !text || !text[0]) return;

    int totalW = 0;
    int maxAscent  = (int)(ascent  * scale);
    int maxDescent = (int)(-descent * scale);

    const char* p = text;
    while (*p)
    {
        int codepoint;
        p = DecodeUTF8(p, &codepoint);
        if (codepoint <= 0) break;

        int ax, lsb;
        stbtt_GetCodepointHMetrics(info, codepoint, &ax, &lsb);
        totalW += (int)(ax * scale);

        int kern = 0;
        int nextCodepoint;
        const char* next = DecodeUTF8(p, &nextCodepoint);
        if (nextCodepoint > 0)
            kern = stbtt_GetCodepointKernAdvance(info, codepoint, nextCodepoint);
        totalW += (int)(kern * scale);
    }

    *outW = totalW;
    *outH = maxAscent + maxDescent;
}

// ---------------------------------------------------------------------------
// Render
// ---------------------------------------------------------------------------

void SAndroidFont::RenderString(SAndroidFramebuffer& fb, const char* text,
                                 int x, int y, int containerW,
                                 uint32_t color, ETextAlignment align)
{
    if (!info || !text || !text[0]) return;

    // Measure the total width for alignment
    int totalW = 0;
    {
        const char* mp = text;
        while (*mp)
        {
            int cp;
            const char* next = DecodeUTF8(mp, &cp);
            if (cp <= 0) break;
            int ax, lsb;
            stbtt_GetCodepointHMetrics(info, cp, &ax, &lsb);
            totalW += (int)(ax * scale);
            int ncp;
            const char* nn = DecodeUTF8(next, &ncp);
            if (ncp > 0)
                totalW += (int)(stbtt_GetCodepointKernAdvance(info, cp, ncp) * scale);
            mp = next;
        }
    }

    // Baseline Y: centre text vertically in the container.
    // The font metrics give us ascent (above baseline) and descent (below).
    int baselineY = y + (int)(ascent * scale);

    // Starting X based on alignment
    float xpos = (float)x;
    switch (align)
    {
    case ETextAlignment::Center:
        xpos = (float)(x + (containerW - totalW) / 2);
        break;
    case ETextAlignment::Right:
        xpos = (float)(x + containerW - totalW);
        break;
    default:
        break;
    }

    const char* p = text;
    while (*p)
    {
        int codepoint;
        const char* next = DecodeUTF8(p, &codepoint);
        if (codepoint <= 0) break;

        int glyphIndex = stbtt_FindGlyphIndex(info, codepoint);

        int x0, y0, x1, y1;
        stbtt_GetGlyphBitmapBox(info, glyphIndex, scale, scale, &x0, &y0, &x1, &y1);

        int gw = x1 - x0;
        int gh = y1 - y0;

        if (gw > 0 && gh > 0)
        {
            // Render glyph to a temporary buffer
            unsigned char* gbuf = new unsigned char[gw * gh];
            stbtt_MakeGlyphBitmap(info, gbuf, gw, gh, gw, scale, scale, glyphIndex);

            // Blend glyph pixels into the framebuffer
            int dstX = (int)xpos + x0;
            int dstY = baselineY + y0;

            for (int row = 0; row < gh; ++row)
            {
                for (int col = 0; col < gw; ++col)
                {
                    uint8_t coverage = gbuf[row * gw + col];
                    if (coverage == 0) continue;

                    uint8_t alpha = (uint8_t)(((uint32_t)coverage * (color >> 24)) / 255);
                    if (alpha == 0) continue;

                    uint32_t glyphColor = (alpha << 24) | (color & 0x00FFFFFF);
                    fb.BlendPixel(dstX + col, dstY + row, glyphColor);
                }
            }

            delete[] gbuf;
        }

        // Advance
        int ax, lsb;
        stbtt_GetCodepointHMetrics(info, codepoint, &ax, &lsb);
        xpos += ax * scale;

        // Kerning
        int nextCodepoint;
        const char* nn = DecodeUTF8(next, &nextCodepoint);
        if (nextCodepoint > 0)
            xpos += stbtt_GetCodepointKernAdvance(info, codepoint, nextCodepoint) * scale;

        p = next;
    }
}
