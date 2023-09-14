#pragma once
#include <WinSock2.h>

class socketManager
{
public:
    enum resultStatus
    {
        FAILED = 0,
        SUCCESS,
    };

public:
    socketManager();
    ~socketManager();
    resultStatus initialize();
    resultStatus uninitialize();
    resultStatus create();
    resultStatus connect();
    resultStatus chat();

private:
    void closeSocket();

private:
    SOCKET clientSocket_;
};

