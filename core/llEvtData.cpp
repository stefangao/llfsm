//============================================================================
// Name        : llEvtData.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "llUtils.h"
#include "llEvtData.h"

NS_LL_BEGIN

const EvtData EvtData::EMPTY = EvtData(0);

EvtData::EvtData(int bufLen)
{
    mBuf = nullptr;
    mBufSize = 0;
    mWritePos = 0;
    mReadPos = 0;
    mIsNeedFree = false;

    if (bufLen > 0)
        createBuffer(bufLen);
}

EvtData::EvtData(pbyte pData, int nDataLen)
{
    mBuf = nullptr;
    reset(pData, nDataLen);
}

EvtData::~EvtData()
{
    freeBuffer();
}

pbyte EvtData::createBuffer(int bufSize)
{
    freeBuffer();

    mBuf = (pbyte)malloc(bufSize);
    LLASSERT(mBuf != NULL, "createBuffer: error");

    mBufSize = bufSize;
    mWritePos = 0;
    mReadPos = 0;
    mIsNeedFree = true;
    return mBuf;
}

void EvtData::freeBuffer()
{
    if (mBuf && mIsNeedFree)
    {
        free(mBuf);
        mBuf = nullptr;
        mWritePos = 0;
        mReadPos = 0;
        mIsNeedFree = false;
    }
}

bool EvtData::ensureExtraCapacity(int nDataLen)
{
    if (nDataLen <= 0)
        return true;

    if (mBufSize == 0)
        createBuffer(DATA_BUFFER_INITLEN);

    int nNewBufLen = mBufSize;
    while (mWritePos + nDataLen > nNewBufLen) {nNewBufLen *= 2;}

    if (mBufSize < nNewBufLen)
    {
        pbyte pNewBuf = (pbyte)realloc(mBuf, nNewBufLen);
        LLASSERT(pNewBuf != NULL, "ensureExtraCapacity error");
        if (pNewBuf == NULL)
            return false;

        mBuf = pNewBuf;
        mBufSize = nNewBufLen;
        mIsNeedFree = true;
    }
    return true;
}

void EvtData::reset(pbyte pData, int nDataLen)
{
    freeBuffer();

    mBuf = pData;
    mBufSize = nDataLen;
    mWritePos = nDataLen;
    mReadPos = 0;
    mIsNeedFree = false;
}

void EvtData::clear()
{
    mWritePos = 0;
    mReadPos = 0;
}

bool EvtData::write(pbyte pData, int nDataLen)
{
    LLASSERT(pData != NULL && nDataLen > 0, "argument error");

    if (!ensureExtraCapacity(nDataLen))
        return false;

    memcpy(mBuf + mWritePos, pData, nDataLen);
    mWritePos += nDataLen;
    return true;
}

int EvtData::read(pbyte pDataBuf, int nBufLen)
{
    LLASSERT(pDataBuf != NULL && nBufLen > 0, "argument error");

    int nReadLen = std::min(nBufLen, mWritePos - mReadPos);
    memcpy(pDataBuf, mBuf + mReadPos, nReadLen);
    mReadPos += nReadLen;
    return nReadLen;
}

bool EvtData::write(EvtData* targetBuf)
{
    LLASSERT(targetBuf != NULL, "argument error");

    int dataLen = targetBuf->getDataLen();
    if (!ensureExtraCapacity(dataLen))
        return false;

    memcpy(mBuf + mWritePos, targetBuf->mBuf + targetBuf->mReadPos, dataLen);
    targetBuf->mReadPos += dataLen;
    mWritePos += dataLen;
    return true;
}

int EvtData::read(EvtData* targetBuf)
{
    LLASSERT(targetBuf != NULL, "argument error");

    int dataLen = getDataLen();
    if (!targetBuf->ensureExtraCapacity(dataLen))
        return -1;

    memcpy(targetBuf->mBuf + targetBuf->mWritePos, mBuf + mReadPos, dataLen);
    mReadPos += dataLen;
    targetBuf->mWritePos += dataLen;
    return dataLen;
}

int EvtData::readString(EvtData* targetBuf)
{
    LLASSERT(targetBuf != NULL, "argument error");

    int dataLen = 0;
    while (mReadPos < mWritePos)
    {
        unsigned char c = mBuf[mReadPos++];
        targetBuf->write(&c, 1);
        if (c == '\0')
            break;

        dataLen++;
    }

    return dataLen;
}

int EvtData::read(std::string& str)
{
    EvtData temp;
    int len = readString(&temp);
    str = temp.c_str();
    return len;
}

bool EvtData::writeString(const char* cstr)
{
    LLASSERT(cstr != NULL, "argument error");

    int nDataLen = (int)strlen(cstr);
    return write((pbyte)cstr, nDataLen + 1);
}

bool EvtData::write(const std::string& str)
{
    return writeString(str.c_str());
}

int EvtData::write(const char* fmt, ...)
{
    va_list args;
    int len = 0;

    va_start(args, fmt);
    len = vsnprintf((char*)mBuf + mWritePos, mBufSize - mWritePos - 1, fmt, args);
    LLASSERT(len > 0 && len < mBufSize - mWritePos - 1, "write error");

    mWritePos += len;
    va_end(args);

    mBuf[mWritePos] = ' ';
    mWritePos++;

    return len;
}

int EvtData::read(const char* fmt, ...)
{
    va_list args;
    int len = 0;

    size_t fmtlen = strlen(fmt);
    char newfmt[fmtlen + 2];
    strcpy(newfmt, fmt);
    newfmt[fmtlen] = ' ';
    newfmt[fmtlen + 1] = '\0';

    va_start(args, fmt);
    len = vsscanf((char*)mBuf + mReadPos, newfmt, args);
    mReadPos += len;
    va_end(args);

    return len;
}

void EvtData::dump()
{
}

EvtData* EvtData::clone() const
{
    auto dst = new EvtData(mBufSize);
    memcpy(dst->mBuf, mBuf, mWritePos);
    dst->mWritePos = mWritePos;
    dst->mReadPos = mReadPos;
    return dst;
}

NS_LL_END
