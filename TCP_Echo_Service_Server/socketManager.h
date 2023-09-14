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
    resultStatus bind();
    resultStatus listen();
    resultStatus accept();

private:
    SOCKET serverSocket_;
};
