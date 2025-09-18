#pragma once

#include "Preprocessors.h"

#include "Templates/Map.h"

#include "SmartPointer/SharedPointer.h"

#include "Threading/CriticalSection.h"
#include "Threading/ScopeLock.h"
#include "Threading/CriticalSection.h"

#include "String/String.h"

#include "Filesystem/Path.h"

#include COMPILE_PLATFORM_TYPE_HEADER_FEATURE(Process, Process.h)

template<
    typename TItem,
    typename TItemPtr = TSharedPtr<TItem>
>
class TFilesystemReference
{
    inline static SCriticalSection _allReferencesCS;
    inline static TMap<CString, TItemPtr> _allReferences;
    
protected:
    CString _path;
    CString _relativePath;
    CString _name;
    bool _bExists;

    inline TFilesystemReference(const CString& InPath)
    {
        _path = GPath.GetFullPath(InPath);

        // TODO: need to think how to move this to platform context
        // maintaining the idea that android works with /sdcard/Android/data/com.package/files
        // I mean, mobile in general
        CString WorkingPath = GProcess.GetContainerPath();

        if (_path == WorkingPath)
        {
            _relativePath = ".";
        }
        else if (_path.StartsWith(WorkingPath))
        {
            _relativePath = _path.Replace(*WorkingPath, "");
            _relativePath = _relativePath.SubString(1, _relativePath.Len());
        }
        else
        {
            _relativePath = _path;
        }

        _name = GPath.LastPathComponent(_path);
    }

    inline virtual ~TFilesystemReference() = default;

    inline virtual void UpdateExistance() = 0;

    inline static TItemPtr Get(const CString& InPath, const TFunction<TItemPtr(const CString&)>& FactoryFunction)
    {
        CString Path = InPath;

        if (GPath.GetPathRoot(InPath) == "")
        {
            // TODO: need to think how to move this to platform context
            // maintaining the idea that android works with /sdcard/Android/data/com.package/files
            // I mean, mobile in general
            Path = GPath.Combine({ GProcess.GetContainerPath(), Path });
        }

        Path = GPath.GetFullPath(Path);

        {
            SScopeLock Lock(_allReferencesCS);

            if (TItemPtr* Item = _allReferences.Find(Path))
            {
                return *Item;
            }

            TItemPtr NewReference = FactoryFunction(Path);
            _allReferences.Add(Path, NewReference);
            return NewReference;
        }
    }

public:
    inline virtual bool Delete() = 0;

    inline const CString& Path() const
    {
        return _path;
    }

    inline const CString& RelativePath() const
    {
        return _relativePath;
    }

    inline const CString& Name() const
    {
        return _name;
    }

    inline bool Exists() const
    {
        return _bExists;
    }

    friend struct SDirectoryRef;
    friend struct SFileRef;
};