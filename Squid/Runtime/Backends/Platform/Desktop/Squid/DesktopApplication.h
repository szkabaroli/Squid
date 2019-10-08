#pragma once

#include <Squid/Platform/Application.h>
#include <SDL.h>

namespace Squid {
	namespace Platform {

		class DesktopApplication final : public Application {
		public:

			DesktopApplication();
			virtual ~DesktopApplication();

			virtual void* OpenLibrary(const char* name) override;
			virtual void* LoadFunction(void* handle, const char* name) override;
			virtual void FreeLibrary(void* handle) override;

			//virtual void Init(const char* name) override;
			//virtual void Shutdown() override;

			//virtual std::unique_ptr<IWindow> CreateWindow(const char* name, uint8_t width, uint8_t height) override;

			virtual void Start() override;
			virtual bool Frame() override;
			virtual void Stop() override;

			virtual void SetFrameCallback(Callback cb) override;
			virtual void SetStopCallback(Callback cb) override;

		private:
			
			void HandleApplicationEvents();
			
			virtual void FrameCallback() override;
			virtual void StopCallback() override;

			bool mRunning = false;

			//Callbacks
			Callback FrameCb = nullptr;
			Callback StopCb = nullptr;
		};
	}
}