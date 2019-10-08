#pragma once

#include <Squid/Platform/Window.h>
#include <SDL.h>
#include <SDL_syswm.h>

namespace Squid {
	namespace Platform {
		class DesktopWindow final : public Window {
		public:
			DesktopWindow(const char* title, uint32_t width, uint32_t height);
			virtual ~DesktopWindow();
			//virtual void Hide() override;
			//virtual bool Show() override;

			//virtual void Minimize() override;
			//virtual void Maximize() override;
			//virtual void Restore()  override;

			//virtual bool IsMaximized() const override;
			//virtual bool IsMinimized() const override;
			virtual bool IsVisible()   const override;

			//virtual void Move(int x, int y) override;
			//virtual void Resize(uint8_t width, uint8_t height) override;

			//virtual void SetWindowMode(EWindowMode windowMode) override;
			//virtual void SetWindowTitle(const char* title) override;

			//virtual EWindowMode GetWindowMode()  const override;
			//virtual const char* GetWindowTitle() const override;*/

			
			void HandleWindowEvents(SDL_Event& event);
			
		private:
			SDL_Window* window;
		};
	}
}