//============================================================================
// Name        : llEvtData.h
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#ifndef __LL_EVTDATA_H__
#define __LL_EVTDATA_H__

#include <string>
#include <string.h>
#include <sstream>
#include "llConst.h"
#include "llUtils.h"

NS_LL_BEGIN

#define DATA_BUFFER_INITLEN    64

class EvtData : public std::stringstream
{
public:
    EvtData& operator << (const char& c)
    {
        if (str().empty())
            *((std::ostream*)this) << c;
        else
            *((std::ostream*)this) << '\0' << c;
        return *this;
    }

    EvtData& operator << (const int& n)
    {
        if (str().empty())
            *((std::ostream*)this) << n;
        else
            *((std::ostream*)this) << '\0' << n;
        return *this;
    }

    EvtData& operator << (const long& n)
    {
        if (str().empty())
            *((std::ostream*)this) << n;
        else
            *((std::ostream*)this) << '\0' << n;
        return *this;
    }

    EvtData& operator << (const float& f)
    {
        if (str().empty())
            *((std::ostream*)this) << f;
        else
            *((std::ostream*)this) << '\0' << f;
        return *this;
    }

    EvtData& operator << (const double& d)
    {
        if (str().empty())
            *((std::ostream*)this) << d;
        else
            *((std::ostream*)this) << '\0' << d;
        return *this;
    }

    EvtData& operator << (const std::string& s)
    {
        if (str().empty())
            *((std::ostream*)this) << s;
        else
            *((std::ostream*)this) << '\0' << s;
        return *this;
    }

    EvtData& operator << (const char* cs)
    {
        if (str().empty())
            *((std::ostream*)this) << cs;
        else
            *((std::ostream*)this) << '\0' << cs;
        return *this;
    }

    EvtData& operator >> (char& c)
    {
        char zero;
        *((std::istream*)this) >> c >> zero;
        return *this;
    }

    EvtData& operator >> (int& n)
    {
        char zero;
        *((std::istream*)this) >> n >> zero;
        return *this;
    }

    EvtData& operator >> (long& n)
    {
        char zero;
        *((std::istream*)this) >> n >> zero;
        return *this;
    }

    EvtData& operator >> (float& f)
    {
        char zero;
        *((std::istream*)this) >> f >> zero;
        return *this;
    }

    EvtData& operator >> (double& d)
    {
        char zero;
        *((std::istream*)this) >> d >> zero;
        return *this;
    }

    EvtData& operator >> (std::string& s)
    {
        char c;
        while ((c = get()) != '\0')
        {
            s.push_back(c);
        }
        return *this;
    }

    EvtData& operator >> (char* cs)
    {
        char c;
        int i = 0;
        while ((c = get()) != '\0')
        {
            cs[i++] = c;
        }
        return *this;
    }

public:
    EvtData(int bufLen = DATA_BUFFER_INITLEN);

    EvtData(pbyte pData, int nDataLen);

    EvtData(const EvtData& other)
    {
        mBuf = nullptr;
        createBuffer(other.mWritePos);
        memcpy(mBuf, other.mBuf, other.mWritePos);
        mWritePos = other.mWritePos;
        mReadPos = other.mReadPos;
        *this << other.rdbuf()->str();
        Utils::log("EvtData::copy1");
    }

    ~EvtData();

    pbyte createBuffer(int bufSize);

    void freeBuffer();

    bool ensureExtraCapacity(int nDataLen);

    void reset(pbyte pData, int nDataLen);

    void clear();

    bool write(pbyte pData, int nDataLen);

    int read(pbyte pDataBuf, int nBufLen);

    bool write(EvtData* targetBuf);

    int read(EvtData* targetBuf);

    int readString(EvtData* targetBuf);
    int read(std::string& str);

    bool writeString(const char* cstr);
    bool write(const std::string& str);

    inline pbyte getData() const
    {
        return mBuf + mReadPos;
    }

    inline int getDataLen() const
    {
        return mWritePos - mReadPos;
    }

    inline pbyte getBuffer()
    {
        return mBuf + mWritePos;
    }

    inline const char* c_str() const
    {
        return (const char*)(mBuf + mReadPos);
    }

    inline bool operator==(const char* str) const
    {
        return strcmp(this->c_str(), str) == 0;
    }

    inline EvtData& operator= (const EvtData& other)
    {
        this->createBuffer(other.mWritePos);
        memcpy(this->mBuf, other.mBuf, other.mWritePos);
        this->mWritePos = other.mWritePos;
        this->mReadPos = other.mReadPos;
        *this << other.rdbuf()->str();
        Utils::log("EvtData::copy2");
        return *this;
    }

    int write(const char* fmt, ...);
    int read(const char* fmt, ...);

    void dump();

    EvtData* clone() const;

    inline bool isEmpty() const
    {
        return mBuf == nullptr || mBufSize == 0;
    }

protected:
    pbyte mBuf;
    int mBufSize;

    int mWritePos;
    int mReadPos;

    bool mIsNeedFree;

public:
    static const EvtData EMPTY;
};

NS_LL_END

#endif
