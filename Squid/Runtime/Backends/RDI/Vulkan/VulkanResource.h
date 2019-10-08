#pragma once
#include "pch.h"
#include <Squid/Render/RenderHandle.h>

namespace Squid {
    namespace Render {
		class VulkanRenderDevice;

		class VulkanResource {
		public:
			VulkanResource(VulkanRenderDevice & _device) : device(_device) {};
		protected:
			VulkanRenderDevice &device;
		};

		class VulkanPipelineState final : public VulkanResource {
		public:
			VulkanPipelineState(VulkanRenderDevice &, RenderHandlePipelineState &);
            ~VulkanPipelineState();
			void Compile();
		private:
			RenderHandlePipelineState& state;

			VkShaderModule vertexShaderModule    = VK_NULL_HANDLE;
			VkShaderModule tessContrShaderModule = VK_NULL_HANDLE;
			VkShaderModule tessEvalShaderModule  = VK_NULL_HANDLE;
			VkShaderModule geometryShaderModule  = VK_NULL_HANDLE;
			VkShaderModule fragmentShaderModule  = VK_NULL_HANDLE;

			VkPipeline pipeline             = VK_NULL_HANDLE;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
			VkPipelineCache pipelineCache   = VK_NULL_HANDLE;
		};

		class VulkanBuffer final : public VulkanResource {
		public:
			VulkanBuffer(VulkanRenderDevice&, RenderHandleBuffer &buffer);
            ~VulkanBuffer();
		};

		class VulkanTexture final : public VulkanResource {
		public:
			VulkanTexture(VulkanRenderDevice&, RenderHandleTexture &texture);
            ~VulkanTexture();
		};

		//class VulkanRenderTarget;
	}
}