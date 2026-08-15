#pragma once

#include <android/native_window.h>
#include <cstdint>
#include <cstring>

// ---------------------------------------------------------------------------
// SAndroidFramebuffer — wraps ANativeWindow_lock / unlockAndPost.
//
// Supports WINDOW_FORMAT_RGB_565 (4), WINDOW_FORMAT_RGBA_8888 (1),
// and WINDOW_FORMAT_RGBX_8888 (2).  All drawing is done in 32-bit ARGB
// internally; pixels are packed at write time.
// ---------------------------------------------------------------------------

struct SAndroidFramebuffer
{
    ANativeWindow_Buffer _buf;
    ANativeWindow*       _window;
    bool                 _locked;

    SAndroidFramebuffer()
        : _window(nullptr)
        , _locked(false)
        , _bClip(false)
        , _clipX(0), _clipY(0), _clipW(0), _clipH(0)
    {
        std::memset(&_buf, 0, sizeof(_buf));
    }

    ~SAndroidFramebuffer()
    {
        if (_locked && _window)
            ANativeWindow_unlockAndPost(_window);
    }

    /// Lock the window surface for CPU drawing. Returns false on failure.
    bool Lock(ANativeWindow* w)
    {
        if (!w) return false;
        _window = w;
        if (ANativeWindow_lock(w, &_buf, nullptr) != 0)
        {
            _window = nullptr;
            return false;
        }
        _locked = true;
        return true;
    }

    /// Post the buffer and release the lock.
    void UnlockAndPost()
    {
        if (!_locked || !_window) return;
        ANativeWindow_unlockAndPost(_window);
        _locked = false;
        _window = nullptr;
        std::memset(&_buf, 0, sizeof(_buf));
    }

    int32_t Width()  const { return _buf.width; }
    int32_t Height() const { return _buf.height; }
    int32_t Stride() const { return _buf.stride; }
    int32_t Format() const { return _buf.format; }

    // -----------------------------------------------------------------------
    // Clip rect — all drawing is restricted to this rectangle when set.
    // Use ClearClipRect() to remove the restriction.
    // -----------------------------------------------------------------------
    void SetClipRect(int x, int y, int w, int h)
    {
        _bClip = true;
        _clipX = x; _clipY = y; _clipW = w; _clipH = h;
    }
    void ClearClipRect() { _bClip = false; }
    bool HasClipRect() const { return _bClip; }

    /// Returns true if the point (x,y) is outside the current clip rect.
    bool IsOutsideClip(int x, int y) const
    {
        if (!_bClip) return false;
        return x < _clipX || y < _clipY || x >= _clipX + _clipW || y >= _clipY + _clipH;
    }

    // -----------------------------------------------------------------------
    // Clear the entire framebuffer to a solid ARGB colour.
    // -----------------------------------------------------------------------
    void Clear(uint32_t argb)
    {
        if (!_locked) return;

        if (_buf.format == WINDOW_FORMAT_RGB_565)
        {
            uint16_t* px = (uint16_t*)_buf.bits;
            uint16_t c16 = Pack565(argb);
            for (int32_t y = 0; y < _buf.height; ++y)
                for (int32_t x = 0; x < _buf.width; ++x)
                    px[y * _buf.stride + x] = c16;
        }
        else
        {
            uint32_t* px = (uint32_t*)_buf.bits;
            uint32_t c32 = Pack8888(argb);
            for (int32_t y = 0; y < _buf.height; ++y)
                for (int32_t x = 0; x < _buf.width; ++x)
                    px[y * _buf.stride + x] = c32;
        }
    }

    // -----------------------------------------------------------------------
    // Fill a rectangle (clipped to framebuffer) with a solid ARGB colour.
    // -----------------------------------------------------------------------
    void FillRect(int x, int y, int w, int h, uint32_t argb)
    {
        if (!_locked || w <= 0 || h <= 0) return;

        // Clip against framebuffer
        if (x < 0) { w += x; x = 0; }
        if (y < 0) { h += y; y = 0; }
        if (x + w > _buf.width)  w = _buf.width  - x;
        if (y + h > _buf.height) h = _buf.height - y;
        if (w <= 0 || h <= 0) return;

        // Clip against scissor/clip rect
        if (_bClip)
        {
            if (x < _clipX) { w -= (_clipX - x); x = _clipX; }
            if (y < _clipY) { h -= (_clipY - y); y = _clipY; }
            if (x + w > _clipX + _clipW) w = _clipX + _clipW - x;
            if (y + h > _clipY + _clipH) h = _clipY + _clipH - y;
            if (w <= 0 || h <= 0) return;
        }

        if (_buf.format == WINDOW_FORMAT_RGB_565)
        {
            uint16_t* px = (uint16_t*)_buf.bits;
            uint16_t c16 = Pack565(argb);
            for (int32_t row = y; row < y + h; ++row)
                for (int32_t col = x; col < x + w; ++col)
                    px[row * _buf.stride + col] = c16;
        }
        else
        {
            uint32_t* px = (uint32_t*)_buf.bits;
            uint32_t c32 = Pack8888(argb);
            for (int32_t row = y; row < y + h; ++row)
                for (int32_t col = x; col < x + w; ++col)
                    px[row * _buf.stride + col] = c32;
        }
    }

    // -----------------------------------------------------------------------
    // Alpha-blend a single ARGB pixel into the framebuffer.
    // Reads the current pixel, blends, writes back.
    // -----------------------------------------------------------------------
    void BlendPixel(int x, int y, uint32_t argb)
    {
        if (!_locked) return;
        if (x < 0 || y < 0 || x >= _buf.width || y >= _buf.height) return;
        if (_bClip && (x < _clipX || y < _clipY || x >= _clipX + _clipW || y >= _clipY + _clipH)) return;

        uint8_t sr = (argb >> 16) & 0xFF;
        uint8_t sg = (argb >>  8) & 0xFF;
        uint8_t sb = (argb >>  0) & 0xFF;
        uint8_t sa = (argb >> 24) & 0xFF;

        if (sa == 0) return;          // fully transparent
        if (sa == 255)                 // fully opaque — overwrite
        {
            SetPixelOpaque(x, y, argb);
            return;
        }

        // Read existing pixel, blend, write back
        uint32_t existing = ReadPixelARGB(x, y);
        uint8_t dr = (existing >> 16) & 0xFF;
        uint8_t dg = (existing >>  8) & 0xFF;
        uint8_t db = (existing >>  0) & 0xFF;
        uint8_t da = (existing >> 24) & 0xFF;

        // Result alpha: sa over da
        uint32_t ra = sa + ((da * (255 - sa)) / 255);
        if (ra == 0) { SetPixelOpaque(x, y, 0); return; }

        uint32_t rr = (sr * sa + dr * da * (255 - sa) / 255) / ra;
        uint32_t rg = (sg * sa + dg * da * (255 - sa) / 255) / ra;
        uint32_t rb = (sb * sa + db * da * (255 - sa) / 255) / ra;

        SetPixelOpaque(x, y, (ra << 24) | (rr << 16) | (rg << 8) | rb);
    }

private:
    // ----- Clip rect ---------------------------------------------------
    bool _bClip;
    int  _clipX, _clipY, _clipW, _clipH;

    // -------------------------------------------------------------------
    // Read a pixel as 32-bit ARGB (converting from the native format).
    // -------------------------------------------------------------------
    uint32_t ReadPixelARGB(int x, int y)
    {
        if (_buf.format == WINDOW_FORMAT_RGB_565)
        {
            uint16_t* px = (uint16_t*)_buf.bits;
            uint16_t c = px[y * _buf.stride + x];
            uint8_t r = (c >> 11) & 0x1F;  r = (r << 3) | (r >> 2);
            uint8_t g = (c >>  5) & 0x3F;  g = (g << 2) | (g >> 4);
            uint8_t b = (c >>  0) & 0x1F;  b = (b << 3) | (b >> 2);
            return 0xFF000000 | (r << 16) | (g << 8) | b;
        }
        else
        {
            uint32_t* px = (uint32_t*)_buf.bits;
            uint32_t c = px[y * _buf.stride + x];
            if (_buf.format == WINDOW_FORMAT_RGBX_8888)
                return c | 0xFF000000;  // force alpha to opaque
            return c; // RGBA_8888
        }
    }

    // -------------------------------------------------------------------
    // Write an opaque ARGB pixel (no blending).
    // -------------------------------------------------------------------
    void SetPixelOpaque(int x, int y, uint32_t argb)
    {
        if (_buf.format == WINDOW_FORMAT_RGB_565)
        {
            uint16_t* px = (uint16_t*)_buf.bits;
            px[y * _buf.stride + x] = Pack565(argb);
        }
        else
        {
            uint32_t* px = (uint32_t*)_buf.bits;
            uint32_t c = Pack8888(argb);
            if (_buf.format == WINDOW_FORMAT_RGBX_8888)
                c |= 0xFF000000;
            px[y * _buf.stride + x] = c;
        }
    }

    static inline uint16_t Pack565(uint32_t argb)
    {
        uint8_t r = (argb >> 16) & 0xFF;
        uint8_t g = (argb >>  8) & 0xFF;
        uint8_t b = (argb >>  0) & 0xFF;
        return (uint16_t)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
    }

    static inline uint32_t Pack8888(uint32_t argb)
    {
        // ARGB → RGBA (little-endian; Android uses RGBA in memory)
        uint8_t a = (argb >> 24) & 0xFF;
        uint8_t r = (argb >> 16) & 0xFF;
        uint8_t g = (argb >>  8) & 0xFF;
        uint8_t b = (argb >>  0) & 0xFF;
        return (r << 0) | (g << 8) | (b << 16) | (a << 24);
    }
};
