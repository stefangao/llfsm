#ifndef __LLCONTEXT_H__
#define __LLCONTEXT_H__

#include <string>
#include "llConst.h"

NS_LL_BEGIN

class Context
{
public:
	Context(const std::string& name);

protected:
	std::string mName;

public:
	static const Context DEFAULT;
};

NS_LL_END

#endif
