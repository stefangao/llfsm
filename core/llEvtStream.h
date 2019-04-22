//============================================================================
// Name        : llEvtData.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_EVTSTREAM_H__
#define __LL_EVTSTREAM_H__

#include <string>
#include <string.h>
#include <sstream>
#include "llConst.h"
#include "llUtils.h"

NS_LL_BEGIN

static const int  DATA_BUFFER_INITLEN = 64;
static const char ZERO = '\0';

class EvtStream : public std::stringstream
{
public:
    EvtStream()
    {

    }

    EvtStream(const EvtStream& other)
    {
        *((std::ostream*)this) << other.str();
    }

    inline EvtStream& operator= (const EvtStream& other)
    {
        *((std::ostream*)this) << other.str();
        return *this;
    }

    EvtStream& operator << (const char& c)
    {
        *((std::ostream*)this) << c << ZERO;
        return *this;
    }

    EvtStream& operator << (const int& n)
    {
        *((std::ostream*)this) << n << ZERO;
        return *this;
    }

    EvtStream& operator << (const long& n)
    {
        *((std::ostream*)this) << n << ZERO;
        return *this;
    }

    EvtStream& operator << (const unsigned long& n)
    {
        *((std::ostream*)this) << n << ZERO;
        return *this;
    }

    EvtStream& operator << (const float& f)
    {
        *((std::ostream*)this) << f << ZERO;
        return *this;
    }

    EvtStream& operator << (const double& d)
    {
        *((std::ostream*)this) << d << ZERO;
        return *this;
    }

    EvtStream& operator << (const std::string& s)
    {
        *((std::ostream*)this) << s << ZERO;
        return *this;
    }

    EvtStream& operator << (const char* cs)
    {
        *((std::ostream*)this) << cs << ZERO;
        return *this;
    }

    EvtStream& operator >> (char& c)
    {
        char zero;
        *((std::istream*)this) >> c >> zero;
        return *this;
    }

    EvtStream& operator >> (int& n)
    {
        char zero;
        *((std::istream*)this) >> n >> zero;
        return *this;
    }

    EvtStream& operator >> (long& n)
    {
        char zero;
        *((std::istream*)this) >> n >> zero;
        return *this;
    }

    EvtStream& operator >> (unsigned long& n)
    {
        char zero;
        *((std::istream*)this) >> n >> zero;
        return *this;
    }

    EvtStream& operator >> (float& f)
    {
        char zero;
        *((std::istream*)this) >> f >> zero;
        return *this;
    }

    EvtStream& operator >> (double& d)
    {
        char zero;
        *((std::istream*)this) >> d >> zero;
        return *this;
    }

    EvtStream& operator >> (std::string& s)
    {
        char c;
        while ((c = get()) != EOF && c != ZERO)
        {
            s.push_back(c);
        }
        return *this;
    }

    EvtStream& operator >> (char* cs)
    {
        char c;
        int i = 0;
        while ((c = get()) != EOF && c != ZERO)
        {
            cs[i++] = c;
        }
        return *this;
    }

    EvtStream& operator >> (EvtStream& dataBuf)
    {
        int dataLen;
        *this >> dataLen;
        if (dataLen > 0)
        {
            int pos = (int)tellg();
            dataBuf.write(str().c_str() + pos, dataLen);
            seekg(pos + dataLen);
        }
        return *this;
    }

    EvtStream& operator >> (unsigned char* ucs)
    {
        int dataLen;
        *this >> dataLen;
        if (dataLen > 0)
        {
            int pos = (int)tellg();
            memcpy(ucs, (pbyte)str().c_str() + pos, dataLen);
            seekg(pos + dataLen);
        }
        return *this;
    }

    int size() const
    {
        return str().size();
    }

    bool empty() const
    {
        return str().size() == 0;
    }

    EvtStream* clone() const
    {
        auto stream = new EvtStream(*this);
        return stream;
    }

    int getPos()
    {
        return tellg();
    }

    inline int getDataLen()
    {
        char c;
        int dataLen = 0;
        int pos = (int)tellg();
        int bufSize = str().size();

        while (pos < bufSize && (c = str().at(pos++)) != EOF && c != ZERO)
        {
            dataLen++;
        }
        return dataLen;
    }

public:
    static const EvtStream EMPTY;
};

NS_LL_END

#endif
