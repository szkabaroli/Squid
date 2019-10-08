#pragma once
#include "pch.h"
#include <Squid/Render/RDI.h>

namespace Squid {
	namespace Render {

		class VulkanRDI final : public RDI {
		public:
			VulkanRDI();
			virtual void Destroy() override;
			virtual Scope<RenderDevice> CreateDevice() override;
			

			virtual void StartFrame() override;
			virtual void EndFrame() override;
		private:
			VkInstance instance = VK_NULL_HANDLE;
			VkPhysicalDevice gpu = VK_NULL_HANDLE;
			VkDebugReportCallbackEXT debugReportCallback = VK_NULL_HANDLE;

            VkSurfaceKHR surface;
		};
	}
}