#pragma once

#include "Templates/IOS/NativeInstance.h"

#include "Devices/UIScreen.h"

@class UIScreen;

class CNativeScreen : public TNativeInstance<UIScreen> { };