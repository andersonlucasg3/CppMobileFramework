#pragma once

#include <objc/objc.h>

#ifdef METALCPP_SYMBOL_VISIBILITY_HIDDEN
#define _NS_PRIVATE_VISIBILITY __attribute__((visibility("hidden")))
#else
#define _NS_PRIVATE_VISIBILITY __attribute__((visibility("default")))
#endif // METALCPP_SYMBOL_VISIBILITY_HIDDEN

#define _NS_PRIVATE_IMPORT __attribute__((weak_import))

#ifdef __OBJC__
#define _NS_PRIVATE_OBJC_LOOKUP_CLASS(symbol) ((__bridge void*)objc_lookUpClass(#symbol))
#define _NS_PRIVATE_OBJC_GET_PROTOCOL(symbol) ((__bridge void*)objc_getProtocol(#symbol))
#else
#define _NS_PRIVATE_OBJC_LOOKUP_CLASS(symbol) objc_lookUpClass(#symbol)
#define _NS_PRIVATE_OBJC_GET_PROTOCOL(symbol) objc_getProtocol(#symbol)
#endif // __OBJC__

#define _NS_PRIVATE_DEF_CLS(symbol) void* s_k##symbol _NS_PRIVATE_VISIBILITY = _NS_PRIVATE_OBJC_LOOKUP_CLASS(symbol)
#define _NS_PRIVATE_DEF_PRO(symbol) void* s_k##symbol _NS_PRIVATE_VISIBILITY = _NS_PRIVATE_OBJC_GET_PROTOCOL(symbol)
#define _NS_PRIVATE_DEF_SEL(accessor, symbol) SEL s_k##accessor _NS_PRIVATE_VISIBILITY = sel_registerName(symbol)

#if defined(__MAC_15_0) || defined(__IPHONE_18_0) || defined(__TVOS_18_0)
#define _NS_PRIVATE_DEF_CONST(type, symbol)              \
    _NS_EXTERN type const NS##symbol _NS_PRIVATE_IMPORT; \
    type const                       NS::symbol = (nullptr != &NS##symbol) ? NS##symbol : type()
#else
#define _NS_PRIVATE_DEF_CONST(type, symbol) \
    _NS_EXTERN type const MTL##symbol _NS_PRIVATE_IMPORT; \
    type const             NS::symbol = Private::LoadSymbol<type>("NS" #symbol)
#endif

#define _NS_PRIVATE_CLS(symbol) (Private::Class::s_k##symbol)
#define _NS_PRIVATE_SEL(accessor) (Private::Selector::s_k##accessor)

#define _NS_PRIVATE_EXTERN_CLS(symbol) extern void* s_k##symbol
#define _NS_PRIVATE_EXTERN_PRO(symbol) extern void* s_k##symbol
#define _NS_PRIVATE_EXTERN_SEL(accessor) extern SEL s_k##accessor
#define _NS_PRIVATE_EXTERN_CONST(type, symbol) extern type const NS::symbol

namespace NS::Private
{
    namespace Class
    {
        _NS_PRIVATE_EXTERN_CLS(NSURL);
        _NS_PRIVATE_EXTERN_CLS(NSArray);
        _NS_PRIVATE_EXTERN_CLS(NSAutoreleasePool);
        _NS_PRIVATE_EXTERN_CLS(NSBundle);
        _NS_PRIVATE_EXTERN_CLS(NSCondition);
        _NS_PRIVATE_EXTERN_CLS(NSDate);
        _NS_PRIVATE_EXTERN_CLS(NSDictionary);
        _NS_PRIVATE_EXTERN_CLS(NSError);
        _NS_PRIVATE_EXTERN_CLS(NSNotificationCenter);
        _NS_PRIVATE_EXTERN_CLS(NSNumber);
        _NS_PRIVATE_EXTERN_CLS(NSObject);
        _NS_PRIVATE_EXTERN_CLS(NSProcessInfo);
        _NS_PRIVATE_EXTERN_CLS(NSSet);
        _NS_PRIVATE_EXTERN_CLS(NSValue);
        _NS_PRIVATE_EXTERN_CLS(NSString);
        _NS_PRIVATE_EXTERN_CLS(NSFileManager);
        _NS_PRIVATE_EXTERN_CLS(NSURLSession);
        _NS_PRIVATE_EXTERN_CLS(NSURLSessionTask);
        _NS_PRIVATE_EXTERN_CLS(NSURLSessionDataTask);
        _NS_PRIVATE_EXTERN_CLS(NSURLRequest);
        _NS_PRIVATE_EXTERN_CLS(NSURLResponse);
        _NS_PRIVATE_EXTERN_CLS(NSOperationQueue);
    }

    namespace Selector
    {
        _NS_PRIVATE_EXTERN_SEL(defaultManager);
        _NS_PRIVATE_EXTERN_SEL(urlForDirectory_inDomain_appropriateForURL_create_error_);
        _NS_PRIVATE_EXTERN_SEL(createDirectoryAtPath_withIntermediateDirectories_attributes_error_);
        _NS_PRIVATE_EXTERN_SEL(createDirectoryAtURL_withIntermediateDirectories_attributes_error_);
        _NS_PRIVATE_EXTERN_SEL(removeItemAtPath_error_);
        _NS_PRIVATE_EXTERN_SEL(removeItemAtURL_error_);
        _NS_PRIVATE_EXTERN_SEL(fileExistsAtPath_);
        _NS_PRIVATE_EXTERN_SEL(fileExistsAtPath_isDirectory_);
        _NS_PRIVATE_EXTERN_SEL(initFileURLWithPath_);
        _NS_PRIVATE_EXTERN_SEL(path);
        _NS_PRIVATE_EXTERN_SEL(array);
        _NS_PRIVATE_EXTERN_SEL(arrayWithObject_);
        _NS_PRIVATE_EXTERN_SEL(arrayWithObjects_count_);
        _NS_PRIVATE_EXTERN_SEL(initWithObjects_count_);
        _NS_PRIVATE_EXTERN_SEL(count);
        _NS_PRIVATE_EXTERN_SEL(addObject_);
        _NS_PRIVATE_EXTERN_SEL(addObserverName_object_queue_block_);
        _NS_PRIVATE_EXTERN_SEL(activeProcessorCount);
        _NS_PRIVATE_EXTERN_SEL(allBundles);
        _NS_PRIVATE_EXTERN_SEL(allFrameworks);
        _NS_PRIVATE_EXTERN_SEL(allObjects);
        _NS_PRIVATE_EXTERN_SEL(alloc);
        _NS_PRIVATE_EXTERN_SEL(appStoreReceiptURL);
        _NS_PRIVATE_EXTERN_SEL(arguments);
        _NS_PRIVATE_EXTERN_SEL(automaticTerminationSupportEnabled);
        _NS_PRIVATE_EXTERN_SEL(autorelease);
        _NS_PRIVATE_EXTERN_SEL(beginActivityWithOptions_reason_);
        _NS_PRIVATE_EXTERN_SEL(boolValue);
        _NS_PRIVATE_EXTERN_SEL(broadcast);
        _NS_PRIVATE_EXTERN_SEL(builtInPlugInsPath);
        _NS_PRIVATE_EXTERN_SEL(builtInPlugInsURL);
        _NS_PRIVATE_EXTERN_SEL(bundleIdentifier);
        _NS_PRIVATE_EXTERN_SEL(bundlePath);
        _NS_PRIVATE_EXTERN_SEL(bundleURL);
        _NS_PRIVATE_EXTERN_SEL(bundleWithPath_);
        _NS_PRIVATE_EXTERN_SEL(bundleWithURL_);
        _NS_PRIVATE_EXTERN_SEL(caseInsensitiveCompare_);
        _NS_PRIVATE_EXTERN_SEL(characterAtIndex_);
        _NS_PRIVATE_EXTERN_SEL(charValue);
        _NS_PRIVATE_EXTERN_SEL(countByEnumeratingWithState_objects_count_);
        _NS_PRIVATE_EXTERN_SEL(cStringUsingEncoding_);
        _NS_PRIVATE_EXTERN_SEL(code);
        _NS_PRIVATE_EXTERN_SEL(compare_);
        _NS_PRIVATE_EXTERN_SEL(copy);
        _NS_PRIVATE_EXTERN_SEL(dateWithTimeIntervalSinceNow_);
        _NS_PRIVATE_EXTERN_SEL(defaultCenter);
        _NS_PRIVATE_EXTERN_SEL(descriptionWithLocale_);
        _NS_PRIVATE_EXTERN_SEL(disableAutomaticTermination_);
        _NS_PRIVATE_EXTERN_SEL(disableSuddenTermination);
        _NS_PRIVATE_EXTERN_SEL(debugDescription);
        _NS_PRIVATE_EXTERN_SEL(description);
        _NS_PRIVATE_EXTERN_SEL(dictionary);
        _NS_PRIVATE_EXTERN_SEL(dictionaryWithObject_forKey_);
        _NS_PRIVATE_EXTERN_SEL(dictionaryWithObjects_forKeys_count_);
        _NS_PRIVATE_EXTERN_SEL(domain);
        _NS_PRIVATE_EXTERN_SEL(doubleValue);
        _NS_PRIVATE_EXTERN_SEL(drain);
        _NS_PRIVATE_EXTERN_SEL(enableAutomaticTermination_);
        _NS_PRIVATE_EXTERN_SEL(enableSuddenTermination);
        _NS_PRIVATE_EXTERN_SEL(endActivity_);
        _NS_PRIVATE_EXTERN_SEL(environment);
        _NS_PRIVATE_EXTERN_SEL(errorWithDomain_code_userInfo_);
        _NS_PRIVATE_EXTERN_SEL(executablePath);
        _NS_PRIVATE_EXTERN_SEL(executableURL);
        _NS_PRIVATE_EXTERN_SEL(fileSystemRepresentation);
        _NS_PRIVATE_EXTERN_SEL(fileURLWithPath_);
        _NS_PRIVATE_EXTERN_SEL(floatValue);
        _NS_PRIVATE_EXTERN_SEL(fullUserName);
        _NS_PRIVATE_EXTERN_SEL(getValue_size_);
        _NS_PRIVATE_EXTERN_SEL(globallyUniqueString);
        _NS_PRIVATE_EXTERN_SEL(hash);
        _NS_PRIVATE_EXTERN_SEL(hasPerformanceProfile_);
        _NS_PRIVATE_EXTERN_SEL(hostName);
        _NS_PRIVATE_EXTERN_SEL(infoDictionary);
        _NS_PRIVATE_EXTERN_SEL(init);
        _NS_PRIVATE_EXTERN_SEL(initWithCoder_);
        _NS_PRIVATE_EXTERN_SEL(initWithBool_);
        _NS_PRIVATE_EXTERN_SEL(initWithBytes_objCType_);
        _NS_PRIVATE_EXTERN_SEL(initWithBytesNoCopy_length_encoding_freeWhenDone_);
        _NS_PRIVATE_EXTERN_SEL(initWithChar_);
        _NS_PRIVATE_EXTERN_SEL(initWithCString_encoding_);
        _NS_PRIVATE_EXTERN_SEL(initWithDomain_code_userInfo_);
        _NS_PRIVATE_EXTERN_SEL(initWithDouble_);
        _NS_PRIVATE_EXTERN_SEL(initWithFloat_);
        _NS_PRIVATE_EXTERN_SEL(initWithInt_);
        _NS_PRIVATE_EXTERN_SEL(initWithLong_);
        _NS_PRIVATE_EXTERN_SEL(initWithLongLong_);
        _NS_PRIVATE_EXTERN_SEL(initWithObjects_forKeys_count_);
        _NS_PRIVATE_EXTERN_SEL(initWithPath_);
        _NS_PRIVATE_EXTERN_SEL(initWithShort_);
        _NS_PRIVATE_EXTERN_SEL(initWithString_);
        _NS_PRIVATE_EXTERN_SEL(initWithUnsignedChar_);
        _NS_PRIVATE_EXTERN_SEL(initWithUnsignedInt_);
        _NS_PRIVATE_EXTERN_SEL(initWithUnsignedLong_);
        _NS_PRIVATE_EXTERN_SEL(initWithUnsignedLongLong_);
        _NS_PRIVATE_EXTERN_SEL(initWithUnsignedShort_);
        _NS_PRIVATE_EXTERN_SEL(initWithURL_);
        _NS_PRIVATE_EXTERN_SEL(integerValue);
        _NS_PRIVATE_EXTERN_SEL(intValue);
        _NS_PRIVATE_EXTERN_SEL(isDeviceCertified_);
        _NS_PRIVATE_EXTERN_SEL(isEqual_);
        _NS_PRIVATE_EXTERN_SEL(isEqualToNumber_);
        _NS_PRIVATE_EXTERN_SEL(isEqualToString_);
        _NS_PRIVATE_EXTERN_SEL(isEqualToValue_);
        _NS_PRIVATE_EXTERN_SEL(isiOSAppOnMac);
        _NS_PRIVATE_EXTERN_SEL(isLoaded);
        _NS_PRIVATE_EXTERN_SEL(isLowPowerModeEnabled);
        _NS_PRIVATE_EXTERN_SEL(isMacCatalystApp);
        _NS_PRIVATE_EXTERN_SEL(isOperatingSystemAtLeastVersion_);
        _NS_PRIVATE_EXTERN_SEL(keyEnumerator);
        _NS_PRIVATE_EXTERN_SEL(length);
        _NS_PRIVATE_EXTERN_SEL(lengthOfBytesUsingEncoding_);
        _NS_PRIVATE_EXTERN_SEL(load);
        _NS_PRIVATE_EXTERN_SEL(loadAndReturnError_);
        _NS_PRIVATE_EXTERN_SEL(localizedDescription);
        _NS_PRIVATE_EXTERN_SEL(localizedFailureReason);
        _NS_PRIVATE_EXTERN_SEL(localizedInfoDictionary);
        _NS_PRIVATE_EXTERN_SEL(localizedRecoveryOptions);
        _NS_PRIVATE_EXTERN_SEL(localizedRecoverySuggestion);
        _NS_PRIVATE_EXTERN_SEL(localizedStringForKey_value_table_);
        _NS_PRIVATE_EXTERN_SEL(lock);
        _NS_PRIVATE_EXTERN_SEL(longValue);
        _NS_PRIVATE_EXTERN_SEL(longLongValue);
        _NS_PRIVATE_EXTERN_SEL(mainBundle);
        _NS_PRIVATE_EXTERN_SEL(maximumLengthOfBytesUsingEncoding_);
        _NS_PRIVATE_EXTERN_SEL(methodSignatureForSelector_);
        _NS_PRIVATE_EXTERN_SEL(mutableBytes);
        _NS_PRIVATE_EXTERN_SEL(name);
        _NS_PRIVATE_EXTERN_SEL(nextObject);
        _NS_PRIVATE_EXTERN_SEL(numberWithBool_);
        _NS_PRIVATE_EXTERN_SEL(numberWithChar_);
        _NS_PRIVATE_EXTERN_SEL(numberWithDouble_);
        _NS_PRIVATE_EXTERN_SEL(numberWithFloat_);
        _NS_PRIVATE_EXTERN_SEL(numberWithInt_);
        _NS_PRIVATE_EXTERN_SEL(numberWithLong_);
        _NS_PRIVATE_EXTERN_SEL(numberWithLongLong_);
        _NS_PRIVATE_EXTERN_SEL(numberWithShort_);
        _NS_PRIVATE_EXTERN_SEL(numberWithUnsignedChar_);
        _NS_PRIVATE_EXTERN_SEL(numberWithUnsignedInt_);
        _NS_PRIVATE_EXTERN_SEL(numberWithUnsignedLong_);
        _NS_PRIVATE_EXTERN_SEL(numberWithUnsignedLongLong_);
        _NS_PRIVATE_EXTERN_SEL(numberWithUnsignedShort_);
        _NS_PRIVATE_EXTERN_SEL(objCType);
        _NS_PRIVATE_EXTERN_SEL(object);
        _NS_PRIVATE_EXTERN_SEL(objectAtIndex_);
        _NS_PRIVATE_EXTERN_SEL(objectEnumerator);
        _NS_PRIVATE_EXTERN_SEL(objectForInfoDictionaryKey_);
        _NS_PRIVATE_EXTERN_SEL(objectForKey_);
        _NS_PRIVATE_EXTERN_SEL(operatingSystem);
        _NS_PRIVATE_EXTERN_SEL(operatingSystemVersion);
        _NS_PRIVATE_EXTERN_SEL(operatingSystemVersionString);
        _NS_PRIVATE_EXTERN_SEL(pathForAuxiliaryExecutable_);
        _NS_PRIVATE_EXTERN_SEL(performActivityWithOptions_reason_usingBlock_);
        _NS_PRIVATE_EXTERN_SEL(performExpiringActivityWithReason_usingBlock_);
        _NS_PRIVATE_EXTERN_SEL(physicalMemory);
        _NS_PRIVATE_EXTERN_SEL(pointerValue);
        _NS_PRIVATE_EXTERN_SEL(preflightAndReturnError_);
        _NS_PRIVATE_EXTERN_SEL(privateFrameworksPath);
        _NS_PRIVATE_EXTERN_SEL(privateFrameworksURL);
        _NS_PRIVATE_EXTERN_SEL(processIdentifier);
        _NS_PRIVATE_EXTERN_SEL(processInfo);
        _NS_PRIVATE_EXTERN_SEL(processName);
        _NS_PRIVATE_EXTERN_SEL(processorCount);
        _NS_PRIVATE_EXTERN_SEL(rangeOfString_options_);
        _NS_PRIVATE_EXTERN_SEL(release);
        _NS_PRIVATE_EXTERN_SEL(removeObserver_);
        _NS_PRIVATE_EXTERN_SEL(resourcePath);
        _NS_PRIVATE_EXTERN_SEL(resourceURL);
        _NS_PRIVATE_EXTERN_SEL(respondsToSelector_);
        _NS_PRIVATE_EXTERN_SEL(retain);
        _NS_PRIVATE_EXTERN_SEL(retainCount);
        _NS_PRIVATE_EXTERN_SEL(setAutomaticTerminationSupportEnabled_);
        _NS_PRIVATE_EXTERN_SEL(setProcessName_);
        _NS_PRIVATE_EXTERN_SEL(sharedFrameworksPath);
        _NS_PRIVATE_EXTERN_SEL(sharedFrameworksURL);
        _NS_PRIVATE_EXTERN_SEL(sharedSupportPath);
        _NS_PRIVATE_EXTERN_SEL(sharedSupportURL);
        _NS_PRIVATE_EXTERN_SEL(sharedSession);
        _NS_PRIVATE_EXTERN_SEL(shortValue);
        _NS_PRIVATE_EXTERN_SEL(showPools);
        _NS_PRIVATE_EXTERN_SEL(signal);
        _NS_PRIVATE_EXTERN_SEL(string);
        _NS_PRIVATE_EXTERN_SEL(stringValue);
        _NS_PRIVATE_EXTERN_SEL(stringWithString_);
        _NS_PRIVATE_EXTERN_SEL(stringWithCString_encoding_);
        _NS_PRIVATE_EXTERN_SEL(stringByAppendingString_);
        _NS_PRIVATE_EXTERN_SEL(stringByResolvingSymlinksInPath);
        _NS_PRIVATE_EXTERN_SEL(systemUptime);
        _NS_PRIVATE_EXTERN_SEL(thermalState);
        _NS_PRIVATE_EXTERN_SEL(unload);
        _NS_PRIVATE_EXTERN_SEL(unlock);
        _NS_PRIVATE_EXTERN_SEL(unsignedCharValue);
        _NS_PRIVATE_EXTERN_SEL(unsignedIntegerValue);
        _NS_PRIVATE_EXTERN_SEL(unsignedIntValue);
        _NS_PRIVATE_EXTERN_SEL(unsignedLongValue);
        _NS_PRIVATE_EXTERN_SEL(unsignedLongLongValue);
        _NS_PRIVATE_EXTERN_SEL(unsignedShortValue);
        _NS_PRIVATE_EXTERN_SEL(URLForAuxiliaryExecutable_);
        _NS_PRIVATE_EXTERN_SEL(userInfo);
        _NS_PRIVATE_EXTERN_SEL(userName);
        _NS_PRIVATE_EXTERN_SEL(UTF8String);
        _NS_PRIVATE_EXTERN_SEL(valueWithBytes_objCType_);
        _NS_PRIVATE_EXTERN_SEL(valueWithPointer_);
        _NS_PRIVATE_EXTERN_SEL(wait);
        _NS_PRIVATE_EXTERN_SEL(waitUntilDate_);
        _NS_PRIVATE_EXTERN_SEL(cancel);
        _NS_PRIVATE_EXTERN_SEL(resume);
        _NS_PRIVATE_EXTERN_SEL(suspend);
        _NS_PRIVATE_EXTERN_SEL(URLWithString_);
        _NS_PRIVATE_EXTERN_SEL(dataTaskWithURL_);
        _NS_PRIVATE_EXTERN_SEL(initWithURL_MIMEType_expectedContentLength_textEncodingName_);
        _NS_PRIVATE_EXTERN_SEL(sessionWithConfiguration_);
        _NS_PRIVATE_EXTERN_SEL(sessionWithConfiguration_delegate_delegateQueue_);
    }
}