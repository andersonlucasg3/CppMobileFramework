#include "AppleLogger.h"

#include "String/String.h"

#include "Templates/Array.h"

#include <Foundation/Foundation.h>

#include <exception>
#include <execinfo.h>
#include <cxxabi.h>

void CAppleLogger::WriteLogLine(const std::string& LogLine) const
{
    // Write to Apple's logging system
    NSLog(@"%s", LogLine.c_str());
}

void CAppleLogger::LogException() const
{
    try
	{
		std::rethrow_exception(std::current_exception());
	}
	catch (const std::exception& Exception)
	{
		Log("EXCEPTION: {}", Exception.what());

		CString StackString = "STACKTRACE:\n";

        void* CallstackArray[128];
        int Frames = backtrace(CallstackArray, 128);
        char** Symbols = backtrace_symbols(CallstackArray, Frames);

        for (int Index = 0; Index < Frames; ++Index)
        {
            CString Frame = Symbols[Index];
            do
            {
                Frame = Frame.Replace("  ", " ");
            }
            while (Frame.Contains("  "));
            
            CString Component = Frame.Split(' ')[3];
            
            SInt32 Status;			
            char* RealName = abi::__cxa_demangle(*Component, nullptr, nullptr, &Status);

            if (Status == 0 && RealName)
            {
                CString DemangledString = CString(Symbols[Index]).Replace(*Component, RealName);

                if (Index == 6)
                {
                    StackString += CString("{} <--- Crash\n", *DemangledString);
                }
                else
                {
                    StackString += CString("{}\n", *DemangledString);
                }
                
                free(RealName);
            }
            else
            {
                StackString += CString("{}\n", Symbols[Index]);
            }
        }
        free(Symbols);

        Log(*StackString);
	}

	_Exit(255);
}