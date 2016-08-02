#ifndef TWINCATVAR_H
#define TWINCATVAR_H

#include <string>

#include "AdsLib.h"

template <typename T> class TwincatVar
{
public:
    TwincatVar(const std::string& name, const AmsAddr remote, const long port): m_name(name),m_remote(remote),m_port(port) {}

    virtual ~TwincatVar() {}

    T getValue() const
    {
        uint32_t bytesRead;
        T buffer;
        uint32_t handle;

        handle = getHandleByName(m_port, m_remote, m_name);

        const long status = AdsSyncReadReqEx2(m_port,
                                                  &m_remote,
                                                  ADSIGRP_SYM_VALBYHND,
                                                  handle,
                                                  sizeof(buffer),
                                                  &buffer,
                                                  &bytesRead);
            if (status)
            {
                std::cout << "ADS read failed with: " << std::dec << status << '\n';
                return T();
            }

        releaseHandle(m_port, m_remote, handle);
        return buffer;
    }


protected:

private:
    std::string m_name;
    AmsAddr m_remote;
    long m_port;
    T value;

uint32_t getHandleByName(long port, const AmsAddr& server, const std::string handleName) const
{
    uint32_t handle = 0;
    const long handleStatus = AdsSyncReadWriteReqEx2(port,
                                                     &server,
                                                     ADSIGRP_SYM_HNDBYNAME,
                                                     0,
                                                     sizeof(handle),
                                                     &handle,
                                                     handleName.size(),
                                                     handleName.c_str(),
                                                     nullptr);
    if (handleStatus) {
        std::cout << "Create handle for '" << handleName << "' failed with: 0x" << std::hex << handleStatus << '\n';
    }
    return handle;
}

void releaseHandle(long port, const AmsAddr& server, uint32_t handle) const
{
    const long releaseHandle = AdsSyncWriteReqEx(port, &server, ADSIGRP_SYM_RELEASEHND, 0, sizeof(handle), &handle);
    if (releaseHandle) {
        std::cout << "Release handle 0x" << std::hex << handle << "' failed with: 0x" << releaseHandle << '\n';
    }
}


};

#endif // TWINCATVAR_H
