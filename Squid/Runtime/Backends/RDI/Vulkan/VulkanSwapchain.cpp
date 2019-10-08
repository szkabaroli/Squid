#include "VulkanSwapchain.h"

namespace {
	// Specify the color channel format and color space type
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
		// VK_FORMAT_UNDEFINED indicates that the surface has no preferred format, so we can choose any
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		}

		// Otherwise, choose a preferred combination
		for (const auto& availableFormat : availableFormats) {
			// Ideal format and color space
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		// Otherwise, return any format
		return availableFormats[0];
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
		// Second choice
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				// First choice
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				// Third choice
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, uint32_t width, uint32_t height) {
		if (capabilities.currentExtent.width == -1) {
			//int width, height;

			//width = System::windowWidth(windowId);
			//height = System::windowWidth(windowId);

			VkExtent2D actualExtent = { width, height };

			if (actualExtent.width < capabilities.minImageExtent.width) 
				actualExtent.width = capabilities.minImageExtent.width;

			if (actualExtent.height < capabilities.minImageExtent.height)
				actualExtent.height = capabilities.minImageExtent.height;
			

			if (actualExtent.width > capabilities.maxImageExtent.width)
				actualExtent.width = capabilities.maxImageExtent.width;

			if (actualExtent.height > capabilities.maxImageExtent.height)
				actualExtent.height = capabilities.maxImageExtent.height;

			return actualExtent;
		}
		else {
			return capabilities.currentExtent;
		}
	}
}

using namespace Squid::Render;

VulkanSwapchain::VulkanSwapchain(const Squid::Platform::Window &win, VulkanRenderDevice &_device, VkSurfaceKHR &_surface) : device(_device), surface(_surface) {
    uint32_t width, height;

	width = 1920;
	height = 1080;

	// Swapchain capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.physicalDevice, surface, &swapchainInfo.capabilities);

    // get present formats
    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device.physicalDevice, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        swapchainInfo.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device.physicalDevice, surface, &formatCount, swapchainInfo.formats.data());
    }

    // Get present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device.physicalDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        swapchainInfo.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device.physicalDevice, surface, &presentModeCount, swapchainInfo.presentModes.data());
    }

	Create(width, height);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(device.handle, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
	    vkCreateSemaphore(device.handle, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create semaphores");
	}
}

VulkanSwapchain::~VulkanSwapchain() {
    vkDestroySemaphore(device.handle, imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(device.handle, renderFinishedSemaphore, nullptr);

	for (size_t i = 0; i < swapchainBuffer.views.size(); i++) {
		vkDestroyImageView(device.handle, swapchainBuffer.views[i], nullptr);
	}

	vkDestroySwapchainKHR(device.handle, swapChain, nullptr);
}

void VulkanSwapchain::Create(uint32_t width, uint32_t height) {

	VkSurfaceFormatKHR surfaceFormat = ::chooseSwapSurfaceFormat(swapchainInfo.formats);
	VkPresentModeKHR presentMode = ::chooseSwapPresentMode(swapchainInfo.presentModes);
	VkExtent2D extent = { width, height }; //::chooseSwapExtent(surfaceCapabilities, windowId);

	// Can do multiple buffering here!
	// Triple Bufferring --> Displaying, Ready to be displayed next, Being worked on
	uint32_t imageCount = swapchainInfo.capabilities.minImageCount; // 2 = only double buffering right now // 2 + 1 = triple buffering
	if (swapchainInfo.capabilities.maxImageCount > 0 && imageCount > swapchainInfo.capabilities.maxImageCount)
		imageCount = swapchainInfo.capabilities.maxImageCount;

	// Construct swap chain create info
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	// TODO: Concurent queue usage
	// An image is owned by one queue family at a time and ownership must be explicitly transfered between uses
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	createInfo.preTransform = swapchainInfo.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	// Create swap chain
	if (vkCreateSwapchainKHR(device.handle, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		throw std::runtime_error("Failed to create swap chain");

	// Retrieve swap chain images
	vkGetSwapchainImagesKHR(device.handle, swapChain, &imageCount, nullptr);
	swapchainBuffer.images.resize(imageCount);
	vkGetSwapchainImagesKHR(device.handle, swapChain, &imageCount, swapchainBuffer.images.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	swapchainBuffer.views.resize(swapchainBuffer.images.size());

	for (size_t i = 0; i < swapchainBuffer.images.size(); i++) {
		// Create an image view for each swap chain image
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapchainBuffer.images[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		// Create the image view
		if (vkCreateImageView(device.handle, &createInfo, nullptr, &swapchainBuffer.views[i]) != VK_SUCCESS)
			throw std::runtime_error("Failed to create image views");
	}
}

void VulkanSwapchain::Recreate(uint32_t w, uint32_t h) {
    vkDestroySwapchainKHR(device.handle, swapChain, nullptr);
	Create(w, h);
}

void VulkanSwapchain::AcquireNext() {
	VkResult res;
	
    //TODO: validationEnabled
	if (true) {
		// the validation layer implementation expects the application to explicitly synchronize with the GPU
		//TODO: MAX_FRAMES_IN_FLIGHT for optimal GPU usage
		//res = vkQueueWaitIdle(device.queues[device.presentQueueFamily]);
	}

	res = vkAcquireNextImageKHR(device.handle, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	// must recreate swapchain -or- swap chain isn't working //CHECK IF BUGS
	if (res == VK_ERROR_OUT_OF_DATE_KHR) {
		Recreate(swapChainExtent.width, swapChainExtent.height);
		return;
	}
	else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to acquire swap chain image");
	}
}

void VulkanSwapchain::Present() {
    
    VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
    VkSwapchainKHR swapchains[] = { swapChain };

	// Submit result back to swap chain for presentation
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapchains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	VkResult res = vkQueuePresentKHR(device.queues[device.presentQueueFamily], &presentInfo);

	if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
		Recreate(swapChainExtent.width, swapChainExtent.height);
	else if (res != VK_SUCCESS)
		throw std::runtime_error("Failed to present swap chain image");
}
