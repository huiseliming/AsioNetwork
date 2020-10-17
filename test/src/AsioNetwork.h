#pragma once
#include "Application.h"
#include <vector>
#include <thread>
#include "network/Server.h"
#include "network/Client.h"


class AsioNetwork : public Application
{
public:
	virtual void StartUp() override;
	virtual void MainLoop() override;
	virtual void Cleanup() override;
private:
	//Server m_server;
	//Client m_client;
};


