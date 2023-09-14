#include <memory>
#include "socketManager.h"

int main()
{
	std::unique_ptr<socketManager> server = std::make_unique<socketManager>();

	server->initialize();
	server->create();
	server->bind();
	server->listen();
	server->accept();
	server->uninitialize();

	return 0;
}