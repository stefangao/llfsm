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

//fsm state type
#define FSMS_INVAL                  (-1L)
#define FSMS_NULL                   (-1L)
#define FSMS_ROOT                   10000000

//fsm state flag
#define SFL_ACTIVE                  0x00000001L             //the state is active
#define SFL_MATCHFALSE              0x00000002L
#define SFL_LEAFMODE                0x00000004L
#define SFL_EVENTUP                 0x00000020L             //transfer event upward
#define SFL_EVENTDOWN               0x00000040L             //transfer event downward

//fsm transfer flag
#define TFL_NEXTPROC                0x00000001L             //handle the event by next state
#define TFL_ENQUEUE                 0x00000002L             //push event in queue
#define TFL_DEQUEUE                 0x00000004L             //pop event from queue
#define TFL_OFFLINE                 0x00000008L             //if the state is inactive，handle the events when it is active

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
    #define LLASSERT(x, msg) \
        if (!(x)) \
	        __ll_assert(__FILE__, __LINE__)

    void __ll_assert(const char*, int lineno)
    {
        abort();
    }
#else
    #define LLASSERT(x, msg)
#endif

#endif
