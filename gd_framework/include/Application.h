#pragma once
#include "NonCopyable.h"
#include "Singleton.h"
#include "Window.h"

#define APPLICATION_NAME "Application"
#define MAIN_WINDOW(...) MainWindow::GetInstance(##__VA_ARGS__)
using MainWindow = Singleton<Window>;

class Application : public NonCopyable
{
public:
	Application() = default;
	virtual ~Application() = default;
	virtual void Run();
	virtual void StartUp() = 0;
	virtual void MainLoop() = 0;
	virtual void Cleanup() = 0;

#ifdef NDEBUG
	bool IsDebug() { return false; }
#else
	bool IsDebug() { return true; }
#endif
private:
	
};

#define APPLICATION_IMPL(App) \
int main(int argc, char* const argv[])\
{\
	try\
	{\
		Singleton<App>::GetInstance().Run();\
	}\
	catch (const std::exception& execption)\
	{\
		std::cout << "v---------" << std::endl;\
		std::cout << execption.what() << std::endl;\
		std::cout << "^---------" << std::endl;\
		return EXIT_FAILURE;\
	}\
	catch (...)\
	{\
		std::cout << "Unknow exception" << std::endl;\
		return EXIT_FAILURE;\
	}\
	return EXIT_SUCCESS;\
}
