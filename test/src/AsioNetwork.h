#pragma once
#include "Application.h"
#include <vector>
#include <thread>

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


