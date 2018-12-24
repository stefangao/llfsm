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

NS_LL_BEGIN

#define DATA_BUFFER_INITLEN    64

class EvtData
{
public:
    EvtData(int bufLen = DATA_BUFFER_INITLEN);

    EvtData(pbyte pData, int nDataLen);

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
        return pBuf + nReadPos;
    }

    inline int getDataLen() const
    {
        return nWritePos - nReadPos;
    }

    inline pbyte getBuffer()
    {
        return pBuf + nWritePos;
    }

    inline const char* c_str() const
    {
        return (const char*)(pBuf + nReadPos);
    }

    inline bool operator==(const char* str) const
    {
        return strcmp(this->c_str(), str) == 0;
    }

    int write(const char* fmt, ...);
    int read(const char* fmt, ...);

    void dump();

    EvtData* clone() const;

protected:
    pbyte pBuf;
    int nSize;

    int nWritePos;
    int nReadPos;

    bool isNeedFree;

public:
    static const EvtData EMPTY;
};

NS_LL_END

#endif
