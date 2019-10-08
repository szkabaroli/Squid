#include "VulkanDevice.h"

namespace Squid {
	namespace Render {
		
		VulkanRenderDevice::VulkanRenderDevice(VkInstance &_instance, VkPhysicalDevice &_physicalDevice) : instance(_instance), physicalDevice(_physicalDevice) {

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
			queueFamilies.resize(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
			
			graphicsQueueFamily = FindQueueFamily(VK_QUEUE_GRAPHICS_BIT);
			computeQueueFamily = FindQueueFamily(VK_QUEUE_COMPUTE_BIT);
			transferQueueFamily = FindQueueFamily(VK_QUEUE_TRANSFER_BIT);
			
			std::set<int> usedQueueFamilies = {
				graphicsQueueFamily,
				computeQueueFamily,
				transferQueueFamily
			};
			
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			queueCreateInfos.reserve(usedQueueFamilies.size());
			
			float queuePriority = 1.0f;
			for (int queueFamily : usedQueueFamilies) {
				if (queueFamily == -1) continue;
				
				VkDeviceQueueCreateInfo queueCreateInfo = {};
				
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				
				queueCreateInfos.push_back(queueCreateInfo);
			}
			std::vector<const char*> deviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};
			
			VkDeviceCreateInfo deviceCreateInfo = {};
			deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data(); 
			deviceCreateInfo.pEnabledFeatures = nullptr;
			deviceCreateInfo.enabledLayerCount = 0;
			deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
			deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
			//FIXME call physical device properties some reason?           
			if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &handle) != VK_SUCCESS)
				throw std::runtime_error("Failed to create VkDevice!");
				
			for (int queueFamily : usedQueueFamilies) {
				queues[queueFamily] = nullptr;
				vkGetDeviceQueue(handle, queueFamily, 0, &queues[queueFamily]);
			}
				
			std::cout << "device created" << std::endl;
		}
		
		VulkanRenderDevice::~VulkanRenderDevice() {
			if(handle != VK_NULL_HANDLE) {
				vkDestroyDevice(handle, nullptr);
				handle = VK_NULL_HANDLE;
			}
			
			std::cout << "device destroyed" << std::endl;
		}
		
		uint32_t VulkanRenderDevice::FindQueueFamily(VkQueueFlagBits queueFlags) {
			int familyIndex = -1;
			
			for (uint32_t i = 0; i < queueFamilies.size(); ++i)
			{
				// First try to find a queue, for which the flags match exactly
				// (i.e. dedicated compute or transfer queue)
				const auto &family = queueFamilies[i];
				if (family.queueFlags == queueFlags)
				{
					familyIndex = i;
					break;
				}
			}
			
			if (familyIndex == -1)
			{
				for (uint32_t i = 0; i < queueFamilies.size(); ++i)
				{
					// Try to find a queue for which all requested flags are set
					const auto &family = queueFamilies[i];
					// Check only QueueFlags as VK_QUEUE_TRANSFER_BIT is
					// optional for graphics and/or compute queues
                    if ((family.queueFlags & queueFlags) == queueFlags)
					{
                        familyIndex = i;
                        break;
					}
				}
			}

            return familyIndex;
		}
        void VulkanRenderDevice::CreateSwapchain(const Platform::Window &win) {
            VkSurfaceKHR surface;
#ifdef SQUID_WIN32
            VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
            surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            surfaceInfo.pNext = NULL;
			surfaceInfo.flags = 0;
            surfaceInfo.hinstance = GetModuleHandle(nullptr);
            surfaceInfo.hwnd = (HWND)win.handle;

            if (vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surface) != VK_SUCCESS)
                throw std::runtime_error("Failed to create Win32 surface!");
#else
			#TODO
#endif

            if ((graphicsQueueFamily == -1 && computeQueueFamily == -1 && transferQueueFamily == -1) || presentQueueFamily > 0)
                throw std::runtime_error("fdasf");

            const auto supportsPresent = [&surface](VkPhysicalDevice device, int familyIndex) {
                VkBool32 presentSupport = VK_FALSE;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, familyIndex, surface, &presentSupport);
                return (presentSupport == VK_TRUE);
			};

            bool graphicsPresentSupported = supportsPresent(physicalDevice, graphicsQueueFamily);
            bool computePresentSupported = supportsPresent(physicalDevice, computeQueueFamily);
            bool transferPresentSupported = supportsPresent(physicalDevice, transferQueueFamily);
            if (graphicsPresentSupported)
				presentQueueFamily = graphicsQueueFamily;
            if (computeQueueFamily != graphicsQueueFamily && computePresentSupported)
                presentQueueFamily = computeQueueFamily;

            if (transferQueueFamily != graphicsQueueFamily && transferQueueFamily != computeQueueFamily)
                presentQueueFamily = transferQueueFamily;

			swapchains.insert(std::make_pair(win.id, MakeScope<VulkanSwapchain>(win, *this, surface)));
			std::cout << "swapchain setup is done" << std::endl;
            std::cout << "Graphics Queue: " << graphicsQueueFamily << std::endl;
            std::cout << " Compute Queue: " << computeQueueFamily << std::endl;
            std::cout << "Transfer Queue: " << transferQueueFamily << std::endl;
            std::cout << " Present Queue: " << presentQueueFamily << std::endl;

			//TODO: Proper command buffer creation
			VkCommandPoolCreateInfo poolCreateInfo = {};
			poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolCreateInfo.queueFamilyIndex = graphicsQueueFamily;

			if (vkCreateCommandPool(handle, &poolCreateInfo, nullptr, &cmdPool) != VK_SUCCESS)
				throw std::runtime_error("Failed to create comand pool!");

			VkCommandBufferAllocateInfo cmdAllocInfo = {};
			cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmdAllocInfo.commandPool = cmdPool;
			cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			cmdAllocInfo.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(handle, &cmdAllocInfo, &cmdBuffer) != VK_SUCCESS)
				throw std::runtime_error("Failed to allocate gfx comand buffer!");
		}
		
		void VulkanRenderDevice::BeginFrame(uint32_t windowId) {
            swapchains[windowId]->AcquireNext();
		}

        void VulkanRenderDevice::EndFrame(uint32_t windowId) {
			swapchains[windowId]->Present();
		}

        bool VulkanRenderDevice::LoadPipelineStateHandle(RenderHandlePipelineState &handle) {
			resources[handle.uuid] = Scope<VulkanResource>(new VulkanPipelineState(*this , handle));
			return true;
		}

		void VulkanRenderDevice::DispatchPipelineStateHandle(const RenderHandlePipelineState &handle, std::string methodName) {
			if (methodName == "Compile") {
				static_cast<VulkanPipelineState*>(resources[handle.uuid].get())->Compile();
			} else {
				throw std::runtime_error("Mehod: " + methodName + "() you want to dispatch, is not exist on PipelineState handle!");
			}
		}

		bool VulkanRenderDevice::CompileCommand(const RenderCommandBegin& cmd) {
			std::cout << "VCompile RenderCommandBegin" << std::endl;

			VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
			cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			cmdBufferBeginInfo.flags = 0;

			if (vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo) != VK_SUCCESS)
				throw std::runtime_error("Failed to begin command buffer");

			return true;
		};

		bool VulkanRenderDevice::CompileCommand(const RenderCommandEnd& cmd) {
			std::cout << "VCompile RenderCommandEnd" << std::endl;

			if (vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS);
				throw std::runtime_error("Failed to end command buffer");

			return true;
		};

		bool VulkanRenderDevice::CompileCommand(const RenderCommandBeginRenderPass& cmd) {
			std::cout << "VCompile RenderCommandBeginRenderPass" << std::endl;



			return true;
		};

		bool VulkanRenderDevice::CompileCommand(const RenderCommandEndRenderPass& cmd) {
			std::cout << "VCompile RenderCommandEndRenderPass" << std::endl;
			return true;
		};

		bool VulkanRenderDevice::CompileCommand(const RenderCommandSetViewport& cmd) {
			std::cout << "VCompile RenderCommandSetViewport" << std::endl;
			return true;
		};

		bool VulkanRenderDevice::CompileCommand(const RenderCommandSetScissor& cmd) {
			std::cout << "VCompile RenderCommandSetScissor" << std::endl;
			return true;
		};

		bool VulkanRenderDevice::CompileCommand(const RenderCommandClearColor& cmd) {
			std::cout << "VCompile RenderCommandClearColor" << std::endl;
			return true;
		};

		bool VulkanRenderDevice::CompileCommand(const RenderCommandDraw& cmd) {
			std::cout << "VCompile RenderCommandDraw" << std::endl;
			return true;
		};
	}
}