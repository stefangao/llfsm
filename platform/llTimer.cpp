//============================================================================
// Name        : llRef.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "../core/llTimer.h"

#if LL_PLATFORM_SHELL == 1
    #include "llTimer_shell.h"
#elif LL_PLATFORM_WIN == 1
    #include "llTimer_win.h"
#endif

