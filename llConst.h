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

//#define LL_DEBUG

//fsm state (new)
#define FSMS_INVAL                  (-1L)
#define FSMS_NULL                   (-1L)
#define FSMS_ROOT                   CNTT_FSMS_ROOT

//fsm state flag (new)
#define SFL_ACTIVE                  0x00000001L             //默认激活子节点
#define SFL_MATCHFALSE              0x00000002L
#define SFL_LEAFMODE                0x00000004L
#define SFL_MOUNTER                 0x00000008L             //该节点是挂钩
#define SFL_MOUNTEE                 0x00000010L             //该节点是挂子(only apply to CNTT_FSMS_ROOT)
#define SFL_EVENTUP                 0x00000020L             //事件是否向上路由(from mountee to mounter)
#define SFL_EVENTDOWN               0x00000040L             //事件是否向下路由(from mounter to mountee)

    //fsm transfer flag
#define TFL_TEXTEVT                 0x0001L                 //文本事件 否则为数值事件
#define TFL_VIAQUEUE                0x0002L                 //经由消息队列后转移状态 (not used)
#define TFL_NEXTPROC                0x0004L                 //由新状态来处理该事件
#define TFL_ENQUEUE                 0x0008L                 //事件加入目标状态的消息队列
#define TFL_DEQUEUE                 0x0010L                 //从目标状态的消息队列取消息
#define TFL_OFFLINE                 0x0020L                 //当前状态inactive时，消息加入队列，当状态重新进入active时，再处理该消息

    //fsm state procedure flag
#define SPFL_ENTER                  0x01
#define SPFL_EXIT                   0x02
#define SPFL_MODE                   0x04

    //fsm state on which procedure
#define SOP_IDLE                    0x00
#define SOP_ENTER                   0x01
#define SOP_MODE                    0x02
#define SOP_EXIT                    0x04

    //fsm result code
#define FSMR_SUCCESS           0
#define FSMR_FAILURE          -1
#define FSMR_FATALERR         -2

    //event handle result code
#define FSMR_CONTINUE          0
#define FSMR_BREAK             1
#define FSMR_REMOVE            2
#define FSMR_UNTOUCHED         3


#ifdef LL_DEBUG
    #define LLASSERT(x) \
        if (!(x)) \
	        __ll_assert(__FILE__, __LINE__)

    void __ll_assert(const char*, int lineno)
    {
        abort();
    }
#else
    #define LLASSERT(x)
#endif

#endif
