#include "DesktopWindow.h"

using namespace Squid::Platform;

std::map<uint32_t, Ref<Window>> windows;

DesktopWindow::DesktopWindow(const char* title, uint32_t width, uint32_t height) {
	
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

	if (handle == nullptr) {
		printf("Could not create window: %s\n", SDL_GetError());
		throw std::runtime_error("failed to create sdl window");
	}
	else {
		initialized = true;
		id = SDL_GetWindowID(window);
		visible = true;
	}

	SDL_SysWMinfo systemInfo; 
	SDL_VERSION(&systemInfo.version);
	SDL_GetWindowWMInfo(window, &systemInfo);
	handle = systemInfo.info.win.window;

	std::cout << "created window" << '\n';
}

DesktopWindow::~DesktopWindow() {
	SDL_DestroyWindow(window);
	std::cout << "destructed window" << '\n';
}

Ref<Window> Window::CreateWindowTest(const char* title, uint32_t width, uint32_t height) {
	Ref<Window> ptr = MakeRef<DesktopWindow>(title, width, height);
	windows.emplace(ptr->id, ptr);
	return ptr;
}

bool DesktopWindow::IsVisible() const {
	return visible;
}

void DesktopWindow::HandleWindowEvents(SDL_Event& event) {
	
	if (event.type == SDL_WINDOWEVENT && event.window.windowID == id) {
		switch (event.window.event) {
		//Window appeared
		case SDL_WINDOWEVENT_SHOWN:
			visible = true;
			break;
		//Window disappeared
		case SDL_WINDOWEVENT_HIDDEN:
			visible = false;
			break;
		//Get new dimensions and repaint
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			//mWidth = e.window.data1;
			//mHeight = e.window.data2;
			break;
		//Mouse enter
		case SDL_WINDOWEVENT_ENTER:
			//mMouseFocus = true;
			break;
		//Mouse exit
		case SDL_WINDOWEVENT_LEAVE:
			//mMouseFocus = false;
			break;
		//Keyboard focus gained
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			//mKeyboardFocus = true;
			break;
		//Keyboard focus lost
		case SDL_WINDOWEVENT_FOCUS_LOST:
			//mKeyboardFocus = false;
			break;
		//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			//mMinimized = true;
			break;
		//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			//mMinimized = false;
			break;
		//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			//mMinimized = false;
			break;
		//Hide on close
		case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow(window);
			std::cout << "Hide window" << '\n';
			break;
		}
	}
}