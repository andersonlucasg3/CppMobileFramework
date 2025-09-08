#include "String.h"

#include "Defines/Types.h"

#include "Templates/Array.h"

#include "SmartPointer/MakeAndCasts.h"

#include "Memory/Memory.h"

#include <stdlib.h>
#include <string_view>
#include <cassert>
#include <cwchar>

TSharedPtr<char> CString::_empty = MakeShareable(new char[0]);

void CString::CopyStr(const char* CStr, SizeT Len)
{
    if (CStr == nullptr || Len == 0)
    {
        _bufferPtr = _empty;
        _length = 0;

        return;
    }

    const SizeT NewLength = Len + 1;
    char* NewBuffer = new char[NewLength];
    GMemory.Copy(CStr, NewBuffer, sizeof(char) * Len);
    NewBuffer[Len] = '\0';

    _bufferPtr = MakeShareable<char, SArrayDeleter<char>>(NewBuffer);
    _length = Len;
}

SizeT CString::StrLen(const char* Buffer) const
{
    if (Buffer == nullptr) return 0;

    return std::strlen(Buffer);
}

#if PLATFORM_WINDOWS

void CString::CopyStr(const wchar_t* CWStr, SizeT Len)
{
    if (CWStr == nullptr || Len == 0)
    {
        _bufferPtr = _empty;
        _length = 0;
    }

    char* NewBuffer = new char[Len + 1];

    UInt64 Index = 0;
    UInt64 Remaining = Len;
    while (Index + Remaining < Len)
    {
        SizeT ConvertedSize = 0;
        char ConvertedChars[1024];
        errno_t Err = wcstombs_s(&ConvertedSize, ConvertedChars, CWStr + Index, 1024);

        if (Err == 0)
        {
            GMemory.Copy(ConvertedChars, NewBuffer + Index, ConvertedSize * sizeof(char));

            Index += ConvertedSize;
            Remaining -= ConvertedSize;
        }
        else
        {
            throw "Failed to convert wchar string to char string";
        }
    }
    
    NewBuffer[Len] = '\0';

    _bufferPtr = MakeShareable<char, SArrayDeleter<char>>(NewBuffer);
    _length = Len;
}

SizeT CString::StrLen(const wchar_t* Buffer) const
{
    if (Buffer == nullptr) return 0;

    return std::wcslen(Buffer);
}

#endif // PLATFORM_WINDOWS

bool CString::Contains(const char* CStr, SizeT Len) const
{
    for (UInt64 Index = 0; Index <= _length - Len; ++Index)
    {
        if (GMemory.Equal(_bufferPtr + Index, Len, CStr, Len))
        {
            return true;
        }
    }

    return false;
}

const char* CString::Empty() 
{
    return _empty.Raw();
}

CString::CString()
:   _length(0)
,   _bufferPtr(_empty)
{
    //
}

CString::CString(const CString& Other)
:   _length(Other._length)
,   _bufferPtr(Other._bufferPtr)
{
    
}

CString::CString(const char* CStr)
:   CString()
{
    CopyStr(CStr, StrLen(CStr));
}

CString::CString(const char* CStr, SizeT Len) 
:   CString()
{
    CopyStr(CStr, Len);
}

#if PLATFORM_WINDOWS
CString::CString(const wchar_t* CWStr)
:   CString()
{
    CopyStr(CWStr, StrLen(CWStr));
}

CString::CString(const wchar_t* CWStr, SizeT Len)
:   CString()
{
    CopyStr(CWStr, Len);
}
#endif

SizeT CString::Len() const
{
    return _length;
}

bool CString::IsEmpty() const
{
    return _length == 0;
}

bool CString::Contains(const char* CStr) const
{
    return Contains(CStr, StrLen(CStr));
}

bool CString::Contains(const CString& Other) const
{
    return Contains(*Other, Other._length);
}

Int64 CString::LastIndexOf(const char Char) const
{
    for (UInt64 Index = _length - 1; Index >= 0; --Index)
    {
        if (_bufferPtr[Index] == Char)
        {
            return Index;
        }
    }

    return -1;
}

CString CString::SubString(UInt64 Start) const
{
    return SubString(Start, _length - 1);
}

CString CString::SubString(UInt64 Start, UInt64 End) const
{
    const SizeT NewLength = End - Start;
    return CString(_bufferPtr + Start, NewLength);
}

CString CString::Replace(const char OldChar, const char NewChar) const
{
    CString NewStr(_bufferPtr.Raw(), _length);

    for (UInt64 Index = 0; Index < NewStr._length; ++Index)
    {
        if (NewStr._bufferPtr[Index] == OldChar)
        {
            NewStr._bufferPtr[Index] = NewChar;
        }
    }

    return NewStr;
}

CString CString::Replace(const char* OldCStr, const char* NewCStr) const
{
    CString Builder = "";

    SizeT OldStrLen = StrLen(OldCStr);

    UInt64 LastCopyed = 0;
    for (UInt64 Index = 0; Index <= _length - OldStrLen; ++Index)
    {
        if (GMemory.Equal(_bufferPtr + Index, OldStrLen, OldCStr, OldStrLen))
        {
            if (Index > LastCopyed)
            {
                Builder += SubString(LastCopyed, Index);
            }

            Builder += NewCStr;
            LastCopyed = Index + OldStrLen;
            Index = LastCopyed;
        }
    }

    if (LastCopyed < _length)
    {
        Builder += SubString(LastCopyed, _length);
    }

    return Builder;
}

CString CString::Replace(const CString& OldStr, const CString& NewStr) const
{
    return Replace(OldStr._bufferPtr.Raw(), NewStr._bufferPtr.Raw());
}

TArray<CString> CString::Split(const char Separator) const
{
    TArray<CString> Results;
    Int64 LastFoundSeparator = 0;
    for (UInt64 Index = 0; Index < _length; Index++)
    {
        if (_bufferPtr[Index] == Separator)
        {
            Results.Add(SubString(LastFoundSeparator, Index));
            LastFoundSeparator = Index + 1;
        }
    }

    if (LastFoundSeparator != 0)
    {
        Results.Add(SubString(LastFoundSeparator, _length));
    }

    return Results;
}

bool CString::StartsWith(const CString& Other) const
{
    if (Other._length > _length) return false;

    return GMemory.Equal(_bufferPtr.Raw(), Other._length, Other._bufferPtr.Raw(), Other._length);
}

bool CString::EndsWith(const CString& Other) const
{
    if (Other._length > _length) return false;

    return GMemory.Equal(_bufferPtr + -Other._length, Other._length, Other._bufferPtr.Raw(), Other._length);
}

char CString::LastChar() const
{
    return _bufferPtr[_length - 1];
}

SizeT CString::Hash() const
{
    SizeT Hash = 14695981039346656037ULL; // 64-bit FNV offset basis
    constexpr SizeT FnvPrime = 1099511628211ULL;
    
    char* Buffer = _bufferPtr.Raw();

    while (*Buffer) {
        Hash ^= static_cast<SizeT>(*Buffer++);
        Hash *= FnvPrime;
    }

    return Hash;
}

#if PLATFORM_WINDOWS
CWString CString::WStr() const
{
    char* CStr = _bufferPtr.Raw();

    wchar_t* NewBuffer = new wchar_t[_length + 1];

    UInt64 Index = 0;
    UInt64 Remaining = _length;
    while (Index + Remaining < _length)
    {
        SizeT ConvertedSize = 0;
        wchar_t ConvertedChars[1024];
        errno_t Err = mbstowcs_s(&ConvertedSize, ConvertedChars, CStr + Index, 1024);

        if (Err == 0)
        {
            GMemory.Copy(ConvertedChars, NewBuffer + Index, ConvertedSize * sizeof(char));

            Index += ConvertedSize;
            Remaining -= ConvertedSize;
        }
        else
        {
            throw "Failed to convert char string to wchar string";
        }
    }
    
    NewBuffer[_length] = L'\0';

    return MakeShareable<wchar_t, SArrayDeleter<wchar_t>>(NewBuffer);
}
#endif // PLATFORM_WINDOWS

CString& CString::operator+=(const CString& Other)
{
    return *this = CString("{}{}", _bufferPtr.Raw(), Other._bufferPtr.Raw());
}

CString& CString::operator+=(const char* CStr)
{
    return *this = CString("{}{}", _bufferPtr.Raw(), CStr);
}

CString& CString::operator+=(char Char)
{
    return *this = CString("{}{}", _bufferPtr.Raw(), Char);
}

CString CString::operator+(const CString& Other) const
{
    return CString("{}{}", _bufferPtr.Raw(), Other._bufferPtr.Raw());
}

CString CString::operator+(const char* CStr) const
{
    return CString("{}{}", _bufferPtr.Raw(), CStr);
}

CString CString::operator+(char Char) const
{
    return CString("{}{}", _bufferPtr.Raw(), Char);
}

bool CString::operator >(const CString& Other) const
{
    return _length > Other._length;
}

bool CString::operator <(const CString& Other) const
{
    return _length < Other._length;
}

CString& CString::operator=(const CString& Other)
{
    _bufferPtr = Other._bufferPtr;
    _length = Other._length;

    return *this;
}

CString& CString::operator=(char* CStr)
{
    CopyStr(CStr, StrLen(CStr));

    return *this;
}

CString& CString::operator=(const char* CStr)
{
    CopyStr(CStr, StrLen(CStr));

    return *this;
}

#if PLATFORM_WINDOWS
CString& CString::operator=(wchar_t* WStr)
{
    CopyStr(WStr, StrLen(WStr));

    return *this;
}

CString& CString::operator=(const wchar_t* CWStr)
{
    CopyStr(CWStr, StrLen(CWStr));

    return *this;
}
#endif // PLATFORM_WINDOWS

bool CString::operator==(const CString& Other) const
{
    if (!_bufferPtr || !Other._bufferPtr) return false;
    return std::string_view(_bufferPtr.Raw(), _length) == std::string_view(Other._bufferPtr.Raw(), Other._length);
}

bool CString::operator==(const char* InCStr) const
{
    if (!_bufferPtr || !InCStr) return false;
    return std::string_view(_bufferPtr.Raw(), _length) == std::string_view(InCStr);
}

CString operator+(const char* Lhs, const CString& Rhs)
{
    return CString("%s%s", Lhs, *Rhs);
}

char CString::operator[](UInt64 Index) const
{
    return _bufferPtr[Index];
}

char* CString::operator*() const
{
    return _bufferPtr.Raw();
}