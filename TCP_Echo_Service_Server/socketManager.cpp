#include <iostream>
#include "socketManager.h"

socketManager::socketManager()
    :serverSocket_(INVALID_SOCKET)
{}

socketManager::~socketManager()
{
    uninitialize();
}

socketManager::resultStatus socketManager::initialize()
{
    WSADATA wsaData;
    auto result = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cout << "ERROR: Unable to initialize Winsock. " << result << std::endl;
        return resultStatus::FAILED;
    }

    return resultStatus::SUCCESS;
}

socketManager::resultStatus socketManager::uninitialize()
{
    if (serverSocket_ != INVALID_SOCKET)
    {
        ::closesocket(serverSocket_);
        serverSocket_ = INVALID_SOCKET;
    }

    ::WSACleanup();

    return resultStatus::SUCCESS;
}

socketManager::resultStatus socketManager::create()
{
    serverSocket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ == INVALID_SOCKET)
    {
        std::cout << "ERROR: Unable to create a listening socket." << std::endl;
        return resultStatus::FAILED;
    }

    return resultStatus::SUCCESS;
}

socketManager::resultStatus socketManager::bind()
{
    SOCKADDR_IN serverAddress = {0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(25000);
    serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    if (::bind(serverSocket_, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cout << "ERROR: Unable to bind IP address and port to socket." << std::endl;
        return resultStatus::FAILED;
    }

    return resultStatus::SUCCESS;
}

socketManager::resultStatus socketManager::listen()
{
    if (::listen(serverSocket_, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "ERROR: Cannot switch to listen state." << std::endl;
        return resultStatus::FAILED;
    }

    return resultStatus::SUCCESS;
}

socketManager::resultStatus socketManager::accept()
{
    SOCKADDR_IN clientAddress = {0};
    int addressLength = sizeof(clientAddress);
    SOCKET clientSocket = 0;
    char buffer[128] = {0};
    int receivedBytes = 0;

    while ((clientSocket = ::accept(serverSocket_,
                               (SOCKADDR*)&clientAddress,
                               &addressLength)) != INVALID_SOCKET)
    {
        std::cout << "A new client has connected." << std::endl;

        while ((receivedBytes = ::recv(clientSocket, buffer, sizeof(buffer), 0)) > 0)
        {
            ::send(clientSocket, buffer, sizeof(buffer), 0); // Echo back only the received bytes
            std::cout << buffer << std::endl;
            memset(buffer, 0, sizeof(buffer));
        }

        ::shutdown(clientSocket, SD_BOTH);
        ::closesocket(clientSocket);
        std::cout << "Client connection was lost." << std::endl;
    }

    return resultStatus::SUCCESS;
}