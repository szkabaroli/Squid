#pragma once
#include "../pch.h"

//typedef void (*Callback)();
typedef std::function<void()> Callback;

namespace Squid {
	namespace Platform {
		
		class Window;

		class Application {
		public:
			Application() {}
			virtual ~Application() {}

			//Shared lib methods
			virtual void* OpenLibrary(const char* name) = 0;
			virtual void* LoadFunction(void* handle, const char* name) = 0;
			virtual void FreeLibrary(void* handle) = 0;

			//virtual void Init(const char* name) = 0;
			//virtual void Shutdown() = 0;

			//virtual std::unique_ptr<IWindow> CreateWindow(const char* name, uint8_t width, uint8_t height) = 0;

			virtual void Start() = 0;
			virtual bool Frame() = 0;
			virtual void Stop() = 0;

			virtual void SetFrameCallback(Callback cb) = 0;
			virtual void SetStopCallback(Callback cb) = 0;

			static std::unique_ptr<Application> CreateApplication();

		private:

			virtual void FrameCallback() = 0;
			virtual void StopCallback() = 0;
		};
	}
}