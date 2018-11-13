#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <list>
#include <vector>
#include <string.h>

namespace llshell {

typedef std::function<void(void*)> CallbackFunc;

typedef struct _Msg
{
    CallbackFunc callback;
    void* data;
} MsgInfo;

typedef struct _TimerMsg
{
    CallbackFunc callback;
    int interval;
    std::chrono::system_clock::time_point timepoint;
    bool once;
    int id;
} TimerInfo;

static std::mutex gMainMtx;
static std::condition_variable gMainCv;

static std::mutex gTimerMtx;
static std::condition_variable gTimerCv;

static bool gFinished = false;

static std::queue<MsgInfo> gMsgQueue;
static std::list<TimerInfo> gTimerList;

static void postMsg(const MsgInfo& msg)
{
    std::unique_lock < std::mutex > lck(gMainMtx);
    gMsgQueue.push(msg);
    gMainCv.notify_one();
}

void postCallback(std::function<void(void*)> func)
{
    MsgInfo msg;
    msg.data = nullptr;
    msg.callback = func;
    postMsg(msg);
}

void setTimer(int interval, std::function<void(void*)> func, bool once)
{
    TimerInfo timer;
    timer.interval = interval;
    timer.timepoint = std::chrono::system_clock::now()
            + std::chrono::milliseconds(interval);
    timer.once = once;
    timer.id = 0;
    timer.callback = func;

    std::unique_lock < std::mutex > lck(gTimerMtx);
    gTimerList.push_back(timer);
    gTimerCv.notify_one();
}

static void exitApp()
{
    gFinished = true;
    gTimerCv.notify_one();
    postCallback(nullptr);
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
                msg.data = nullptr;
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
            //std::cout << interval << "," << once << std::endl;
            setTimer(interval, [timeStart](void* p)
                    {
                        auto timeEnd = std::chrono::system_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart);
                        std::cout << "---time out---" << duration.count() << "\n";
                    }, once);
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

int main()
{
    std::cout << "msgloop start\n";
    std::thread timerThread, inputThread;
    timerThread = std::thread(do_timer_thread);
    inputThread = std::thread(do_input_thread);
    std::unique_lock < std::mutex > lck(gMainMtx);

    while (!gFinished)
    {
        gMainCv.wait(lck, []{return gMsgQueue.size() > 0;});

        while (gMsgQueue.size() > 0)
        {
            MsgInfo& msg = gMsgQueue.front();
            if (msg.callback)
                msg.callback(msg.data);

            gMsgQueue.pop();
        }
    }
    timerThread.join();
    inputThread.join();

    std::cout << "msgloop exit\n";
    return 0;
}

} //namespace llshell end
