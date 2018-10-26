#include "llContext.h"

NS_LL_BEGIN

const Context Context::DEFAULT = Context("defaultContext");

Context::Context(const std::string& name) :
        mName(name)
{

}

NS_LL_END
