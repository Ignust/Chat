#pragma once

class IManagerListener
{
public:
    IManagerListener() = default;
    virtual ~IManagerListener() = default;

    virtual void responseDisconnectServer() = 0;
    virtual void responseDisconnectClient(const int ID) = 0;
};
