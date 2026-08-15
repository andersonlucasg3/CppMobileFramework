#pragma once

#include "Application.h"

class CUIPlaygroundApp : public CApplication
{
public:
    CUIWindow* CreateMainWindow() override;
};
