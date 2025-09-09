#include "FilesystemTests.h"

#include "TestSuit.h"

#include "Filesystem/Path.h"
#include "Filesystem/DirectoryReference.h"
#include "Filesystem/FileReference.h"

REGISTER_TEST_CLASS(FilesystemTests);

void CFilesystemTests::TestFiles()
{
    SFileRef Ref = "./Text.txt";

    const CString& Path = Ref->Path();
    CString Combined = GPath.Combine({ GProcess.GetContainerPath(), "Text.txt" });

    bool bEqual = Path == Combined;

    ASSERT_TRUE(bEqual);
    ASSERT_EQUAL(Path, Combined);
    ASSERT_FALSE(Ref->Exists());

#if PLATFORM_WINDOWS
    SFileRef ExistingRef = "./CoreTests.exe";
#elif PLATFORM_MACOS
    SFileRef ExistingRef = "./CoreTests";
#endif

    ASSERT_TRUE(ExistingRef->Exists());
}

void CFilesystemTests::TestDirectories()
{
    SDirectoryRef Ref = ".";

    const CString& Path = Ref->Path();
    const CString& Combined = GProcess.GetContainerPath();
    
    bool bEqual = Path == Combined;
    ASSERT_TRUE(bEqual);
    ASSERT_EQUAL(Path, Combined);
    ASSERT_TRUE(Ref->Exists());

    SDirectoryRef CreateRef = "./Temp";

    ASSERT_TRUE(CreateRef->Create(true));
    ASSERT_TRUE(CreateRef->Exists());

    ASSERT_TRUE(CreateRef->Delete());
    ASSERT_FALSE(CreateRef->Exists());
}

void CFilesystemTests::TestCase()
{
    TestFiles();
    TestDirectories();
}