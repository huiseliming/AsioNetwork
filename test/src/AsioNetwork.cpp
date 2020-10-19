#pragma once
#include "AsioNetwork.h"
#include <GLFW/glfw3.h>


void AsioNetwork::StartUp()
{
	MAIN_WINDOW().Create();

}

void AsioNetwork::MainLoop()
{
	while (!MAIN_WINDOW().ShouldClose())
	{
		MAIN_WINDOW().PollEvents();
	}
}

void AsioNetwork::Cleanup()
{
	MAIN_WINDOW().Destroy();
}



//APPLICATION_IMPL(AsioNetwork)






