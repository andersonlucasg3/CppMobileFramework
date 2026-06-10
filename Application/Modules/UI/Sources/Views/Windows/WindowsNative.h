#pragma once
#include <Windows.h>

class CNativeInstance
{
public:
    CNativeInstance(HWND h) : _hWnd(h) {}
    ~CNativeInstance() = default;
    HWND GetHWND() const { return _hWnd; }
    void* Native() const { return (void*)_hWnd; }
    void SetHWND(HWND h) { _hWnd = h; }
private:
    HWND _hWnd;
};

inline HWND ViewHWND(const CUIView* V) { return ((CNativeInstance*)V->GetNativePointer())->GetHWND(); }