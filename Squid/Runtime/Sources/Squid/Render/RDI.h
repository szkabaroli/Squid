#pragma once
#include "../pch.h"
#include <Squid/Render/RenderDevice.h>

namespace Squid {

	namespace Render {

		class RenderTarget {

		};

		class RDI {
		public:
			virtual void Destroy() = 0;
			virtual Scope<RenderDevice> CreateDevice() = 0;

			//for swapchain render
			virtual void StartFrame() = 0;
			virtual void EndFrame() = 0;
		};

		typedef RDI* (*PFN_CreateRDI)();
	}
}
