#pragma once
#include "pch.h"
#include <Squid/Platform/Window.h>
#include "VulkanDevice.h"

namespace Squid {
    namespace Render {
        
        class VulkanSwapchain {
		    friend class VulkanRenderDevice;

        public:
			VulkanSwapchain(const Squid::Platform::Window &, VulkanRenderDevice &, VkSurfaceKHR &);
			~VulkanSwapchain();
			
            void Create(uint32_t w, uint32_t h);
			void Recreate(uint32_t w, uint32_t h);

			void AcquireNext();
			void Present();

		private:
			VulkanRenderDevice &device;
			VkSurfaceKHR &surface;
			VkSwapchainKHR swapChain;

            struct VulkanSwapchainInfo {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> presentModes;
            } swapchainInfo;

			struct {
				std::vector<VkImage> images;
				std::vector<VkImageView> views;
			} swapchainBuffer;

			VkFormat swapChainImageFormat;
			VkExtent2D swapChainExtent;
			uint32_t imageIndex = 0;

			VkSemaphore imageAvailableSemaphore;
			VkSemaphore renderFinishedSemaphore;
		};
    }
}
