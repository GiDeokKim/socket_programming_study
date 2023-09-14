#include <iostream>
#include "socketManager.h"
#pragma warning (disable:4996)

socketManager::socketManager()
    :clientSocket_(INVALID_SOCKET)
{}

socketManager::~socketManager()
{
    closeSocket();
    uninitialize();
}

void socketManager::closeSocket()
{
    if (clientSocket_ != INVALID_SOCKET)
    {
        ::closesocket(clientSocket_);
        clientSocket_ = INVALID_SOCKET;
    }
}

socketManager::resultStatus socketManager::initialize()
{
    WSADATA wsaData = { 0 };
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "ERROR: Unable to initialize Winsock." << std::endl;
        return resultStatus::FAILED;
    }

    return resultStatus::SUCCESS;
}

socketManager::resultStatus socketManager::uninitialize()
{
    ::WSACleanup();
    return resultStatus::SUCCESS;
}

socketManager::resultStatus socketManager::create()
{
    clientSocket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket_ == INVALID_SOCKET)
    {
        std::cout << "ERROR: Unable to create a client socket." << std::endl;
        return resultStatus::FAILED;
    }

    return resultStatus::SUCCESS;
}

socketManager::resultStatus socketManager::connect()
{
    SOCKADDR_IN serverAddress = { 0 };
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(25000);
    serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    if (::connect(clientSocket_, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cout << "ERROR: Unable to connect to the server." << std::endl;
        return resultStatus::FAILED;
    }

    return resultStatus::SUCCESS;
}

socketManager::resultStatus socketManager::chat()
{
    char buffer[128] = { 0 };
    while (true)
    {
        gets_s(buffer);
        if (strcmp(buffer, "EXIT") == 0)
        {
            break;
        }

        ::send(clientSocket_, buffer, strlen(buffer) + 1, 0);
        memset(buffer, 0, sizeof(buffer));
        ::recv(clientSocket_, buffer, sizeof(buffer), 0);
        std::cout << "From server: " << buffer << std::endl;
    }
    return resultStatus::SUCCESS;
}