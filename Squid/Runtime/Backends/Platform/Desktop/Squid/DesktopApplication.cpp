#include "DesktopApplication.h"
#include "DesktopWindow.h"

using namespace Squid::Platform;

extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

//The entry point for the application
int SquidMain(int argc, char** argv);
int main(int argc, char* argv[])
{
	std::cout << "squid_main() called" << '\n';
	return SquidMain(argc, argv);
}

extern std::map<uint32_t, Ref<Window>> windows;
extern int windowCounter;

DesktopApplication::DesktopApplication() {
	std::cout << "Init sdl" << '\n';
	if (SDL_Init((SDL_INIT_VIDEO) | SDL_INIT_NOPARACHUTE) != 0)
	{
		const char* errorMessage = SDL_GetError();
		std::cout << errorMessage << '\n';
	}

	std::cout << "created app" << '\n';

	//initilize the selected render device
	//GraphicsRDI->Init();
}

DesktopApplication::~DesktopApplication() {
	windows.clear();
	SDL_Quit();

	std::cout << "destructed app" << '\n';
}

Scope<Application> Application::CreateApplication() {
	return std::move(MakeScope<DesktopApplication>());
}

void* DesktopApplication::OpenLibrary(const char* name) {
	return SDL_LoadObject(name);
}

void* DesktopApplication::LoadFunction(void* handle, const char* name) {
	return SDL_LoadFunction(handle, name);
}

void DesktopApplication::FreeLibrary(void* handle) {
	SDL_UnloadObject(handle);
}

void DesktopApplication::Start() {
	mRunning = true;
	while (Frame()) {}
	StopCallback();
}

bool DesktopApplication::Frame() {
	FrameCallback();
	HandleApplicationEvents();
	return mRunning;
}

void DesktopApplication::Stop() {
	mRunning = false;
}

void DesktopApplication::HandleApplicationEvents() {
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			mRunning = false;
		}

		//Handle event per windows
		bool allWindowsClosed = true;
		for (const auto& window : windows) {
			//Cast to derived DesktopWindow class so we can access HandleWindowEvents
			DesktopWindow* windowD = static_cast<DesktopWindow*>(window.second.get());
			windowD->HandleWindowEvents(event);

			if (windowD->IsVisible()) {
				allWindowsClosed = false;
			}
		}

		//If all windows hidden close the application
		if (allWindowsClosed) {
			mRunning = false;
		}
	}
}

//#TODO templates based callback system

void DesktopApplication::FrameCallback() {
	if (FrameCb != nullptr) {
		FrameCb();
	}
}

void DesktopApplication::StopCallback() {
	if (StopCb != nullptr) {
		StopCb();
	}
}

void DesktopApplication::SetFrameCallback(Callback cb) { FrameCb = cb; }
void DesktopApplication::SetStopCallback(Callback cb) { StopCb = cb; }
