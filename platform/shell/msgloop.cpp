//============================================================================
// Name        : msgloop.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <list>
#include <vector>
#include <string.h>
#include "msgloop.h"

namespace llshell {

typedef struct _Msg
{
    MsgCallbackFunc callback;
    const void* data;
    int id;
} MsgInfo;

typedef struct _TimerMsg
{
    MsgCallbackFunc callback;
    int interval;
    std::chrono::system_clock::time_point timepoint;
    bool once;
    int id;
    const void* userData;
} TimerInfo;

static std::mutex gMainMtx;
static std::condition_variable gMainCv;

static std::mutex gTimerMtx;
static std::condition_variable gTimerCv;

static bool gFinished = false;

static std::queue<MsgInfo> gMsgQueue;
static std::list<TimerInfo> gTimerList;
static int gNextTimerId = 0;

static CmdCallbackFunc gCmdCallbackFun = nullptr;

static void postMsg(const MsgInfo& msg)
{
    std::unique_lock < std::mutex > lck(gMainMtx);
    gMsgQueue.push(msg);
    gMainCv.notify_one();
    lck.unlock();
}

void postCallback(const void* userData, const MsgCallbackFunc& func)
{
    MsgInfo msg;
    msg.id = 0;
    msg.data = userData;
    msg.callback = func;
    postMsg(msg);
}

int setTimer(int interval, const MsgCallbackFunc& func, const void* userData, bool once)
{
    TimerInfo timer;
    timer.interval = interval;
    timer.timepoint = std::chrono::system_clock::now()
            + std::chrono::milliseconds(interval);
    timer.once = once;
    timer.id = gNextTimerId++;
    timer.userData = userData;
    timer.callback = func;

    std::unique_lock < std::mutex > lck(gTimerMtx);
    gTimerList.push_back(timer);
    gTimerCv.notify_one();
    return timer.id;
}

bool killTimer(int tid)
{
    for (auto iter = gTimerList.begin(); iter != gTimerList.end(); iter++)
    {
        if (tid == (*iter).id)
        {
            gTimerList.erase(iter);
            gTimerCv.notify_one();
            return true;
        }
    }
    return false;
}

static void exitApp()
{
    gFinished = true;
    gTimerCv.notify_one();
    postCallback(nullptr, nullptr);
}

static void do_timer_thread()
{
    std::unique_lock < std::mutex > lck(gTimerMtx);
    while (!gFinished)
    {
        gTimerList.sort([](const TimerInfo& timer1, const TimerInfo& timer2)->bool
                {
                    return timer1.timepoint < timer2.timepoint;
                });

        int interval = 0;
        TimerInfo timerInfo;
        auto iter = gTimerList.begin();
        if (iter != gTimerList.end())
        {
            timerInfo = *iter;
            interval = timerInfo.interval;
        }

        if (interval > 0)
        {
            std::cv_status cvsts = gTimerCv.wait_until(lck, timerInfo.timepoint);
            if (cvsts == std::cv_status::timeout)
            {
                MsgInfo msg;
                msg.data = timerInfo.userData;
                msg.id = timerInfo.id;
                msg.callback = timerInfo.callback;
                postMsg(msg);
                if (!timerInfo.once)
                {
                    (*iter).timepoint = std::chrono::system_clock::now()
                            + std::chrono::milliseconds(interval);
                }
                else
                {
                    gTimerList.erase(iter);
                }
            }
        }
        else
        {
            gTimerCv.wait(lck);
        }
    }

    std::cout << "timer thread exit\n";
}

static std::vector<std::string> input_split(const std::string &input,const std::string &pattern)
{
    char* strc = new char[input.size() + 1];
    strcpy(strc, input.c_str());

    std::vector<std::string> resultVec;
    char* section = strtok(strc, pattern.c_str());
    while (section != NULL)
    {
        resultVec.push_back(std::string(section));
        section = strtok(NULL, pattern.c_str());
    }
    delete[] strc;

    return resultVec;
}

static void dumpCommand(const std::string& cmd, std::vector<std::string> params)
{
    std::cout << cmd << "=>(";

    int count = params.size();
    for (int i = 0; i < count; i++)
    {
        std::cout << params[i];
        if (i != count - 1)
            std::cout << ",";
    }
    std::cout << ")" << std::endl;
}

static void onCommand(const std::string& cmd, std::vector<std::string> params)
{
    dumpCommand(cmd, params);

    if (gCmdCallbackFun)
    {
        gCmdCallbackFun(cmd, params);
        return;
    }

    if (cmd == "t")
    {
        int num = params.size();
        if (num > 0)
        {
            int interval = atoi(params[0].c_str());
            bool once = false;
            if (num > 1)
                once = atoi(params[1].c_str());

            auto timeStart = std::chrono::system_clock::now();
            int tid = setTimer(interval, [timeStart](int id, const void* p)
                    {
                        auto timeEnd = std::chrono::system_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart);
                        std::cout << "---time out[" << id << "]---" << duration.count() << "\n";
                    }, nullptr, once);
            std::cout << "setTimer" << "[" << tid << "]:" << interval << "," << once << std::endl;
        }
    }
    else if (cmd == "p")
    {
        int num = params.size();
        if (num > 0)
        {
            long userData = atol(params[0].c_str());
            postCallback((const void*)userData, [](int, const void* userData)
                    {
                        long value = (long)userData;
                        std::cout << "postCallback: userData=" << "(" << value << ")"<< std::endl;
                    });
        }
    }
    else if (cmd == "k")
    {
        int num = params.size();
        if (num > 0)
        {
            int tid = atoi(params[0].c_str());
            killTimer(tid);
        }
    }
    else if (cmd == "exit")
    {
        exitApp();
    }
}

static void do_input_thread()
{
    while (!gFinished)
    {
        std::string input;
        std::vector<std::string> cmdVec;

        std::getline(std::cin, input);
        cmdVec = input_split(input, " \t");
        if (cmdVec.size() > 0 && cmdVec[0].size() > 0)
        {
            auto &cmd = cmdVec[0];
            std::vector<std::string> params;
            if (cmdVec.size() > 1)
            {
                params.resize(cmdVec.size() - 1);
                std::copy(cmdVec.begin() + 1, cmdVec.end(), params.begin());
            }
            onCommand(cmd, params);
        }
    }

    std::cout << "input thread exit\n";
}

int run_msgloop(const CmdCallbackFunc& cmdCallbackFun)
{
    std::cout << "msgloop start\n";
    std::thread timerThread, inputThread;
    timerThread = std::thread(do_timer_thread);
    inputThread = std::thread(do_input_thread);

    while (!gFinished)
    {
        gCmdCallbackFun = cmdCallbackFun;
        std::unique_lock <std::mutex> lck(gMainMtx);
        gMainCv.wait(lck, []{return gMsgQueue.size() > 0;});
        int msgNum = gMsgQueue.size();
        if (msgNum > 0)
        {
            MsgInfo msg = gMsgQueue.front();
            gMsgQueue.pop();
            lck.unlock();

            if (msg.callback)
                msg.callback(msg.id, msg.data);
        }

    }
    timerThread.join();
    inputThread.join();

    std::cout << "msgloop exit\n";
    return 0;
}

} //namespace llshell end
