#pragma once
#include <iostream>

#include "VulkanProject.h"

int main()
{
	try
	{
		VulkanApp app;
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}
