#include <memory>
#include <iostream>
#include "socketManager.h"

int main()
{
    std::unique_ptr<socketManager> client = std::make_unique<socketManager>();;

    if (client->initialize() != socketManager::resultStatus::SUCCESS)
    {
        std::cerr << "Failed to initialize the client->" << std::endl;
        return 1;
    }

    if (client->create() != socketManager::resultStatus::SUCCESS)
    {
        std::cerr << "Failed to create the client socket." << std::endl;
        return 2;
    }

    if (client->connect() != socketManager::resultStatus::SUCCESS)
    {
        std::cerr << "Failed to connect to the server." << std::endl;
        return 3;
    }

    std::cout << "Connected to server. You can start chatting now. Type 'EXIT' to quit." << std::endl;

    if (client->chat() != socketManager::resultStatus::SUCCESS)
    {
        std::cerr << "Chat session terminated with errors." << std::endl;
        return 4;
    }

    std::cout << "Chat session ended successfully." << std::endl;
    return 0;
}
