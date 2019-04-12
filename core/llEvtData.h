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

static const int  DATA_BUFFER_INITLEN = 64;
static const char ZERO = '\0';

class DataBuf
{
    friend EvtData;
public:
    DataBuf(int bufLen = DATA_BUFFER_INITLEN);
    DataBuf(pbyte pData, int nDataLen);
    DataBuf(const DataBuf& other)
    {
        mBuf = nullptr;
        createBuffer(other.mWritePos);
        memcpy(mBuf, other.mBuf, other.mWritePos);
        mWritePos = other.mWritePos;
        mReadPos = other.mReadPos;
    }

    ~DataBuf();

    pbyte createBuffer(int bufSize);

    void freeBuffer();

    bool ensureExtraCapacity(int nDataLen);

    void reset(pbyte pData, int nDataLen);

    void clear();

    bool write(pbyte pData, int nDataLen);

    int read(pbyte pDataBuf, int nBufLen);

    bool write(DataBuf* targetBuf);

    int read(DataBuf* targetBuf);

    int readString(DataBuf* targetBuf);
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
        return mBuf + mReadPos;
    }

    inline const char* c_str() const
    {
        return (const char*)(mBuf + mReadPos);
    }

    inline bool operator==(const char* str) const
    {
        return strcmp(this->c_str(), str) == 0;
    }

    inline DataBuf& operator= (const DataBuf& other)
    {
        this->createBuffer(other.mWritePos);
        memcpy(this->mBuf, other.mBuf, other.mWritePos);
        this->mWritePos = other.mWritePos;
        this->mReadPos = other.mReadPos;
        LLLOG("DataBuf::copy2");
        return *this;
    }

    int write(const char* fmt, ...);
    int read(const char* fmt, ...);

    void dump();

    DataBuf* clone() const;

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
};

class EvtData : public DataBuf, public std::stringstream
{
public:
    EvtData(int bufLen = DATA_BUFFER_INITLEN)
    {
    }

    EvtData(pbyte pData, int nDataLen)
    {
    }

    EvtData(const EvtData& other)
    {
        *this << other.str();
    }

    inline EvtData& operator= (const EvtData& other)
    {
        *this = other;
        *this << other.str();
        return *this;
    }

    EvtData& operator << (const char& c)
    {
        *((std::ostream*)this) << c << ZERO;
        return *this;
    }

    EvtData& operator << (const int& n)
    {
        *((std::ostream*)this) << n << ZERO;
        return *this;
    }

    EvtData& operator << (const long& n)
    {
        *((std::ostream*)this) << n << ZERO;
        return *this;
    }

    EvtData& operator << (const float& f)
    {
        *((std::ostream*)this) << f << ZERO;
        return *this;
    }

    EvtData& operator << (const double& d)
    {
        *((std::ostream*)this) << d << ZERO;
        return *this;
    }

    EvtData& operator << (const std::string& s)
    {
        *((std::ostream*)this) << s << ZERO;
        return *this;
    }

    EvtData& operator << (const char* cs)
    {
        *((std::ostream*)this) << cs << ZERO;
        return *this;
    }

    EvtData& operator << (const DataBuf& dataBuf)
    {
        int dataLen = dataBuf.mWritePos - dataBuf.mReadPos;
        *this << dataLen;
        int readPos = dataBuf.mReadPos;
        while (readPos < dataBuf.mWritePos)
        {
            unsigned char c = dataBuf.mBuf[readPos++];
            *((std::ostream*)this) << c;
        }
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
        while ((c = get()) != EOF && c != ZERO)
        {
            s.push_back(c);
        }
        return *this;
    }

    EvtData& operator >> (char* cs)
    {
        char c;
        int i = 0;
        while ((c = get()) != EOF && c != ZERO)
        {
            cs[i++] = c;
        }
        return *this;
    }

    EvtData& operator >> (DataBuf& dataBuf)
    {
        int dataLen;
        *this >> dataLen;
        if (dataLen > 0)
        {
            int pos = (int)tellg();
            dataBuf.write((pbyte)str().c_str() + pos, dataLen);
            seekg(pos + dataLen);
        }
        return *this;
    }

    EvtData& operator >> (unsigned char* ucs)
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

    int getSize()
    {
        int size = 0;
        int pos = (int)tellg();
        int bufSize = str().size();
        char c;
        while (pos < bufSize && (c = str().at(pos++)) != EOF && c != ZERO)
        {
            size++;
        }
        return size;
    }

    EvtData* clone() const
    {
        auto dst = new EvtData(mBufSize);
        memcpy(dst->mBuf, mBuf, mWritePos);
        dst->mWritePos = mWritePos;
        dst->mReadPos = mReadPos;
        *dst << str();
        return dst;
    }

public:
    static const EvtData EMPTY;
};

NS_LL_END

#endif
