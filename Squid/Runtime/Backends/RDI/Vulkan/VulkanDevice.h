#pragma once
#include "pch.h"
#include <Squid/Render/RenderDevice.h>
#include "VulkanRDI.h"
#include "VulkanSwapchain.h"
#include "VulkanResource.h"

namespace Squid {
	namespace Render {

		class VulkanResource;
		
		class VulkanRenderDevice final : public RenderDevice {
			friend class VulkanRDI;
			friend class VulkanSwapchain;
			friend class VulkanPipelineState;
		public:
			VulkanRenderDevice(VkInstance &, VkPhysicalDevice &);
			virtual ~VulkanRenderDevice();
			virtual void CreateSwapchain(const Window &) override;

			virtual void BeginFrame(uint32_t windowId) override;
			virtual void EndFrame(uint32_t windowId) override;

			virtual bool LoadPipelineStateHandle(RenderHandlePipelineState& handle) override;
			virtual void DispatchPipelineStateHandle(const RenderHandlePipelineState& handle, std::string methodName) override;
		protected:
			virtual bool CompileCommand(const RenderCommandBegin& cmd) override;
			virtual bool CompileCommand(const RenderCommandEnd& cmd) override;

			virtual bool CompileCommand(const RenderCommandBeginRenderPass& cmd) override;
			virtual bool CompileCommand(const RenderCommandEndRenderPass& cmd) override;
			virtual bool CompileCommand(const RenderCommandSetViewport& cmd) override;
			virtual bool CompileCommand(const RenderCommandSetScissor& cmd) override;
			virtual bool CompileCommand(const RenderCommandClearColor& cmd) override;
			virtual bool CompileCommand(const RenderCommandDraw& cmd) override;
		private:
			uint32_t FindQueueFamily(VkQueueFlagBits);

			VkInstance &instance;
			VkPhysicalDevice &physicalDevice;
			VkDevice handle;

			// Properties
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			VkPhysicalDeviceMemoryProperties memoryProperties;

			// Device queues
			int graphicsQueueFamily = -1;
			int transferQueueFamily = -1;
			int computeQueueFamily = -1;
			int presentQueueFamily = -1;
			std::vector<VkQueueFamilyProperties> queueFamilies;
			std::map<int, VkQueue> queues;

			// Vulkan specific objects
			std::map<uint32_t, Scope<VulkanSwapchain>> swapchains;
			std::map<std::string, Scope<VulkanResource>> resources;

			//Command list variables
			//TODO: eg multiple lists 
			VkCommandPool cmdPool;
			std::vector<VkCommandBuffer> frameCmdBuffers;

			VkCommandBuffer cmdBuffer;
		};
	}
}

