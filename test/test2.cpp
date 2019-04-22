//============================================================================
// Name        : test2.cpp
// Author      : Stefan Gao (stefan.gao@gmail.com)
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include "test2.h"
using namespace test2;

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

    int size()
    {
    	return tellp();
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
};

TestCase2::TestCase2()
{

}

TestCase2::~TestCase2()
{

}

bool TestCase2::onInit()
{
    FSM *fsm = new FSMA();
    fsm->create("fsm2").start();

    EvtStream data_orig;
    data_orig << 234;
    int dataLen1 = data_orig.getDataLen();
    data_orig << 789;
    int dataLen2 = data_orig.getDataLen();

    int orig_p1 = data_orig.tellp();
    int orig_g1 = data_orig.tellg();

    EvtStream data = data_orig;

    int orig_p2 = data_orig.tellp();
    int orig_g2 = data_orig.tellg();

    int p1 = data.tellp();
    int g1 = data.tellg();

    int a, b;
    data >> a;

    int p2 = data.tellp();
    int g2 = data.tellg();

    data >> b;

    int p3 = data.tellp();
    int g3 = data.tellg();

    char buf[128] = {0};
    data.std::stringstream::read(buf, 128);

    return true;
}

namespace test2 {
/////////// FSMA ///////////
BEGIN_STATE_TABLE(FSMA)
    STATE_ENTRY(DAEMON, Daemon, S_ROOT,    SFL_ACTIVE)
    STATE_ENTRY(TEST1,  Test1,  DAEMON,    SFL_ACTIVE)
    STATE_ENTRY(TEST2,  Test2,  DAEMON,    0)
END_STATE_TABLE()

BEGIN_TRANS_TABLE(FSMA, FSM)
    //TRANS_ENTRY(TEST1, "TestEvt1", TEST2)
END_TRANS_TABLE()

bool FSMA::Test1::onEventProc(const std::string& evtName, EvtData& evtData)
{
    return true;
}

void FSMA::Test1::onHeartBeat()
{
    postEvent("TestEvt1");
}

bool FSMA::Test2::onEventProc(const std::string& evtName, EvtData& evtData)
{
    return true;
}

void FSMA::Test2::onHeartBeat()
{
    self()->stop();
}

}
