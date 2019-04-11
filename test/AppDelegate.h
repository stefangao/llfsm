//============================================================================
// Name        : AppDelegate.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_APPDELEGATE_H__
#define __LL_APPDELEGATE_H__

#include "lianli.h"
USING_NS_LL;

class AppDelegate
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void onUserEvent(const std::string& fsmName, const std::string& evtName, std::vector<std::string>& evtParams);
};

#endif
