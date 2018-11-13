#ifndef __LL_MSGLOOP_H__
#define __LL_MSGLOOP_H__

#include <functional>

namespace llshell {

void setTimer(int interval, std::function<void(void*)> func, bool once);
void postCallback(std::function<void(void*)> func);
int main();

}

#endif
