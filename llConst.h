#ifndef __LLCONST_H__
#define __LLCONST_H__

#ifdef __cplusplus
    #define NS_LL_BEGIN                     namespace lianli {
    #define NS_LL_END                       }
    #define USING_NS_LL                     using namespace lianli
#else
    #define NS_LL_BEGIN
    #define NS_LL_END
    #define USING_NS_LL
#endif

#endif
