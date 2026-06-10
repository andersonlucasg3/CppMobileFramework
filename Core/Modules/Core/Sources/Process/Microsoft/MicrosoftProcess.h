#pragma once

#include "Process/Desktop/DesktopProcess.h"

#include "Templates/Array.h"

#include "String/String.h"

class CMicrosoftProcess : public CDesktopProcess
{
	CString ExecutablePath;
	CString ExecutableContainerPath;
	TArray<CString> CommandLineArgs;

public:
	CORE_API CMicrosoftProcess() = default;
	CORE_API ~CMicrosoftProcess() override = default;

	CORE_API void SetCommandLine(int argc, char* argv[]) override;
	CORE_API const CString& GetExecutablePath() const override;
	CORE_API const CString& GetContainerPath() const override;
	CORE_API const TArray<CString>& GetCommandLineArgs() const override;
};

typedef CMicrosoftProcess CPlatformProcess;