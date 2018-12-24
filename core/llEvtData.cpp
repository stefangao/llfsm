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
    pBuf = nullptr;
    nSize = 0;
    nWritePos = 0;
    nReadPos = 0;
    isNeedFree = false;

    if (bufLen > 0)
        createBuffer(bufLen);
}

EvtData::EvtData(pbyte pData, int nDataLen)
{
    pBuf = nullptr;
    reset(pData, nDataLen);
}

EvtData::~EvtData()
{
    freeBuffer();
}

pbyte EvtData::createBuffer(int bufSize)
{
    freeBuffer();

    pBuf = (pbyte)malloc(bufSize);
    LLASSERT(pBuf != NULL, "createBuffer: error");

    nSize = bufSize;
    nWritePos = 0;
    nReadPos = 0;
    isNeedFree = true;
    return pBuf;
}

void EvtData::freeBuffer()
{
    if (pBuf && isNeedFree)
    {
        free(pBuf);
        pBuf = nullptr;
        nWritePos = 0;
        nReadPos = 0;
        isNeedFree = false;
    }
}

bool EvtData::ensureExtraCapacity(int nDataLen)
{
    if (nDataLen <= 0)
        return true;

    if (nSize == 0)
        createBuffer(DATA_BUFFER_INITLEN);

    int nNewBufLen = nSize;
    while (nWritePos + nDataLen > nNewBufLen) {nNewBufLen *= 2;}

    if (nSize < nNewBufLen)
    {
        pbyte pNewBuf = (pbyte)realloc(pBuf, nNewBufLen);
        LLASSERT(pNewBuf != NULL, "ensureExtraCapacity error");
        if (pNewBuf == NULL)
            return false;

        pBuf = pNewBuf;
        nSize = nNewBufLen;
        isNeedFree = true;
    }
    return true;
}

void EvtData::reset(pbyte pData, int nDataLen)
{
    freeBuffer();

    pBuf = pData;
    nSize = nDataLen;
    nWritePos = nDataLen;
    nReadPos = 0;
    isNeedFree = false;
}

void EvtData::clear()
{
    nWritePos = 0;
    nReadPos = 0;
}

bool EvtData::write(pbyte pData, int nDataLen)
{
    LLASSERT(pData != NULL && nDataLen > 0, "argument error");

    if (!ensureExtraCapacity(nDataLen))
        return false;

    memcpy(pBuf + nWritePos, pData, nDataLen);
    nWritePos += nDataLen;
    return true;
}

int EvtData::read(pbyte pDataBuf, int nBufLen)
{
    LLASSERT(pDataBuf != NULL && nBufLen > 0, "argument error");

    int nReadLen = std::min(nBufLen, nWritePos - nReadPos);
    memcpy(pDataBuf, pBuf + nReadPos, nReadLen);
    nReadPos += nReadLen;
    return nReadLen;
}

bool EvtData::write(EvtData* targetBuf)
{
    LLASSERT(targetBuf != NULL, "argument error");

    int dataLen = targetBuf->getDataLen();
    if (!ensureExtraCapacity(dataLen))
        return false;

    memcpy(pBuf + nWritePos, targetBuf->pBuf + targetBuf->nReadPos, dataLen);
    targetBuf->nReadPos += dataLen;
    nWritePos += dataLen;
    return true;
}

int EvtData::read(EvtData* targetBuf)
{
    LLASSERT(targetBuf != NULL, "argument error");

    int dataLen = getDataLen();
    if (!targetBuf->ensureExtraCapacity(dataLen))
        return -1;

    memcpy(targetBuf->pBuf + targetBuf->nWritePos, pBuf + nReadPos, dataLen);
    nReadPos += dataLen;
    targetBuf->nWritePos += dataLen;
    return dataLen;
}

int EvtData::readString(EvtData* targetBuf)
{
    LLASSERT(targetBuf != NULL, "argument error");

    int dataLen = 0;
    while (nReadPos < nWritePos)
    {
        unsigned char c = pBuf[nReadPos++];
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
    len = vsnprintf((char*)pBuf + nWritePos, nSize - nWritePos - 1, fmt, args);
    LLASSERT(len > 0 && len < nSize - nWritePos - 1, "write error");

    nWritePos += len;
    va_end(args);

    pBuf[nWritePos] = ' ';
    nWritePos++;

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
    len = vsscanf((char*)pBuf + nReadPos, newfmt, args);
    nReadPos += len;
    va_end(args);

    return len;
}

void EvtData::dump()
{
}

EvtData* EvtData::clone() const
{
    auto dst = new EvtData(nSize);
    memcpy(dst->pBuf, pBuf, nWritePos);
    dst->nWritePos = nWritePos;
    dst->nReadPos = nReadPos;
    return dst;
}

NS_LL_END
