//============================================================================
// Name        : llRef.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "../core/llTimer.h"

#if LL_PLATFORM_LINUX == 1
    #include "llTimer_linux.h"
#elif LL_PLATFORM_WIN == 1
    #include "llTimer_win.h"
#elif LL_PLATFORM_MAC == 1
    #include "llTimer_mac.h"
#elif LL_PLATFORM_COCOS2DX == 1
    #include "llTimer_cocos2dx.h"
#endif
