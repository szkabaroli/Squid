#pragma once
#include "../pch.h"

namespace Squid {
	namespace Platform {

		enum EWindowMode {

		};

		class Window {
		public:
			virtual ~Window() {}

			static Ref<Window> CreateWindowTest(const char* title, uint32_t width, uint32_t height);

			//static void Destroy(std::unique_ptr<Window>);

			//virtual void Hide() = 0;
			//virtual bool Show() = 0;

			//virtual void Minimize() = 0;
			//virtual void Maximize() = 0;
			//virtual void Restore() = 0;

			//virtual bool IsMaximized() const = 0;
			//virtual bool IsMinimized() const = 0;
			virtual bool IsVisible()   const = 0;

			//virtual void Move(int x, int y) = 0;
			//virtual void Resize(uint8_t width, uint8_t height) = 0;

			//virtual void SetWindowMode(EWindowMode windowMode) = 0;
			//virtual void SetWindowTitle(const char* title) = 0;

			//virtual EWindowMode GetWindowMode()  const = 0;
			//virtual const char* GetWindowTitle() const = 0;

		public:
			uint32_t id;
			void* handle;
			uint32_t width;
			uint32_t height;

		protected:
			bool initialized = false;
			bool visible = false;

		};
	}
}
