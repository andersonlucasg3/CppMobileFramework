#pragma once
#include <Windows.h>

#include "Graphics/Rects.h"

using namespace UI::Graphics;

class CUIView; // fwd

class CNativeInstance
{
public:
    CNativeInstance(HWND h) : _hWnd(h), _hBgBrush(nullptr), _hFont(nullptr), _frame(0,0,0,0), _bgColorRef(RGB(128,128,128)) {}
    ~CNativeInstance()
    {
        if (_hBgBrush) { DeleteObject(_hBgBrush); _hBgBrush = nullptr; }
        if (_hFont)    { DeleteObject(_hFont);    _hFont    = nullptr; }
    }

    HWND   GetHWND()   const { return _hWnd; }
    void*  Native()    const { return (void*)_hWnd; }
    void   SetHWND(HWND h)   { _hWnd = h; }

    HBRUSH GetBgBrush() const { return _hBgBrush; }
    void   SetBgBrush(HBRUSH b)
    {
        if (_hBgBrush) DeleteObject(_hBgBrush);
        _hBgBrush = b;
    }

    COLORREF GetBgColorRef() const { return _bgColorRef; }
    void     SetBgColorRef(COLORREF cr) { _bgColorRef = cr; }

    HFONT  GetFont() const { return _hFont; }
    void   SetFont(HFONT f)
    {
        if (_hFont) DeleteObject(_hFont);
        _hFont = f;
    }

    SRectF GetFrame() const    { return _frame; }
    void   SetFrame(SRectF f)  { _frame = f; }

private:
    HWND    _hWnd;
    HBRUSH  _hBgBrush;
    HFONT   _hFont;
    SRectF  _frame;
    COLORREF _bgColorRef;
};

inline HWND ViewHWND(const CUIView* V) { return (HWND)V->GetNativePointer(); }

/// Store a CUIView* back-pointer in an HWND's userdata.
inline void   ViewSetUserData(HWND h, CUIView* v) { SetWindowLongPtrA(h, GWLP_USERDATA, (LONG_PTR)v); }
inline CUIView* ViewGetUserData(HWND h)            { return (CUIView*)GetWindowLongPtrA(h, GWLP_USERDATA); }

// ---------------------------------------------------------------------------
// Per-HWND brush storage (external-linkage map, accessible from all TUs)
// ---------------------------------------------------------------------------
void SetHWNDBrushColor(HWND h, COLORREF cr);
HBRUSH GetHWNDBrush(HWND h);
void RemoveHWNDBrush(HWND h);
