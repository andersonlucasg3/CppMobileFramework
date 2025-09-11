#pragma once

#include "ObjectPtr.h"
#include "WeakObjectPtr.h"

#define FORWARD_DECLARE_OBJECT_PTR(ClassName)                                   \
typedef TObjectPtr< C##ClassName > C##ClassName##ObjectPtr;                     \
typedef TWeakObjectPtr< C##ClassName > C##ClassName##WeakObjectPtr

#define FORWARD_DECLARE_OBJECT(ClassName)                                       \
class C##ClassName;                                                             \
FORWARD_DECLARE_OBJECT_PTR(ClassName)