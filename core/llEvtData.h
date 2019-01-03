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
#include "llConst.h"
#include "llUtils.h"

NS_LL_BEGIN

#define DATA_BUFFER_INITLEN    64

class EvtData
{
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
