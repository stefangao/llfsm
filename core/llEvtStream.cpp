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

DataBuf::DataBuf(int bufLen)
{
    mBuf = nullptr;
    mBufSize = 0;
    mWritePos = 0;
    mReadPos = 0;
    mIsNeedFree = false;

    if (bufLen > 0)
        createBuffer(bufLen);
}

DataBuf::DataBuf(pbyte pData, int nDataLen)
{
    mBuf = nullptr;
    reset(pData, nDataLen);
}

DataBuf::~DataBuf()
{
    freeBuffer();
}

pbyte DataBuf::createBuffer(int bufSize)
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

void DataBuf::freeBuffer()
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

bool DataBuf::ensureExtraCapacity(int nDataLen)
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

void DataBuf::reset(pbyte pData, int nDataLen)
{
    freeBuffer();

    mBuf = pData;
    mBufSize = nDataLen;
    mWritePos = nDataLen;
    mReadPos = 0;
    mIsNeedFree = false;
}

void DataBuf::clear()
{
    mWritePos = 0;
    mReadPos = 0;
}

bool DataBuf::write(pbyte pData, int nDataLen)
{
    LLASSERT(pData != NULL && nDataLen > 0, "argument error");

    if (!ensureExtraCapacity(nDataLen))
        return false;

    memcpy(mBuf + mWritePos, pData, nDataLen);
    mWritePos += nDataLen;
    return true;
}

int DataBuf::read(pbyte pDataBuf, int nBufLen)
{
    LLASSERT(pDataBuf != NULL && nBufLen > 0, "argument error");

    int nReadLen = std::min(nBufLen, mWritePos - mReadPos);
    memcpy(pDataBuf, mBuf + mReadPos, nReadLen);
    mReadPos += nReadLen;
    return nReadLen;
}

bool DataBuf::write(DataBuf* targetBuf)
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

int DataBuf::read(DataBuf* targetBuf)
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

int DataBuf::readString(DataBuf* targetBuf)
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

int DataBuf::read(std::string& str)
{
    DataBuf temp;
    int len = readString(&temp);
    str = temp.c_str();
    return len;
}

bool DataBuf::writeString(const char* cstr)
{
    LLASSERT(cstr != NULL, "argument error");

    int nDataLen = (int)strlen(cstr);
    return write((pbyte)cstr, nDataLen + 1);
}

bool DataBuf::write(const std::string& str)
{
    return writeString(str.c_str());
}

int DataBuf::write(const char* fmt, ...)
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

int DataBuf::read(const char* fmt, ...)
{
    va_list args;
    int len = 0;

    int fmtLen = strlen(fmt);
    char* newfmt = new char[fmtLen + 2];
    memcpy(newfmt, fmt, fmtLen);
    newfmt[fmtLen] = ' ';
    newfmt[fmtLen + 1] = '\0';

    va_start(args, fmt);
    len = vsscanf((char*)mBuf + mReadPos, newfmt, args);
    mReadPos += len;
    va_end(args);
    delete[] newfmt;

    return len;
}

void DataBuf::dump()
{
}

DataBuf* DataBuf::clone() const
{
    auto dst = new DataBuf(mBufSize);
    memcpy(dst->mBuf, mBuf, mWritePos);
    dst->mWritePos = mWritePos;
    dst->mReadPos = mReadPos;
    return dst;
}

NS_LL_END
