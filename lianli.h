#ifndef __LIANLI_H__
#define __LIANLI_H__

#ifdef __cplusplus
    #define NS_LL_BEGIN                     namespace lianli {
    #define NS_LL_END                       }
    #define USING_NS_LL                     using namespace lianli
#else
    #define NS_LL_BEGIN
    #define NS_LL_END
    #define USING_NS_LL
#endif

#include "llState.h"
#include "llFSM.h"
#include "llContext.h"

#endif
