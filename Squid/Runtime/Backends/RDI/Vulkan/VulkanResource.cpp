#include "VulkanResource.h"
#include "VulkanDevice.h"

namespace Squid {
    namespace Render {

		static std::string readFile(const std::string& filename) {
			std::ifstream file(filename, std::ios::in | std::ios::binary);

			if (!file.is_open())
				throw std::runtime_error("Failed to open file: " + filename);
			
			std::string buffer;
			file.seekg(0, std::ios::end);
			std::cout << file.tellg() ;
			buffer.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(&buffer[0], buffer.size());
			file.close();
			
			return buffer;
		}

		static std::string loadShaderBytecode(std::string shaderName) {
			return readFile("Shaders/" + shaderName + ".spv");
		}

		VkFormat ConvertFormat(EImageFormat value) {
			switch (value) {
				case EImageFormat::Unknown: return VK_FORMAT_UNDEFINED;
				case EImageFormat::R32G32B32A32_TYPELESS: return VK_FORMAT_R32G32B32A32_UINT;
				case EImageFormat::R32G32B32A32_FLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
				case EImageFormat::R32G32B32A32_UINT: return VK_FORMAT_R32G32B32A32_UINT;
				case EImageFormat::R32G32B32A32_SINT: return VK_FORMAT_R32G32B32A32_SINT;
				case EImageFormat::R32G32B32_TYPELESS: return VK_FORMAT_R32G32B32_UINT;
				case EImageFormat::R32G32B32_FLOAT: return VK_FORMAT_R32G32B32_SFLOAT;
				case EImageFormat::R32G32B32_UINT: return VK_FORMAT_R32G32B32_UINT;
				case EImageFormat::R32G32B32_SINT: return VK_FORMAT_R32G32B32_SINT;
				case EImageFormat::R16G16B16A16_TYPELESS: return VK_FORMAT_R16G16B16A16_UINT;
				case EImageFormat::R16G16B16A16_FLOAT: return VK_FORMAT_R16G16B16A16_SFLOAT;
				case EImageFormat::R16G16B16A16_UNORM: return VK_FORMAT_R16G16B16A16_UNORM;
				case EImageFormat::R16G16B16A16_UINT: return VK_FORMAT_R16G16B16A16_UINT;
				case EImageFormat::R16G16B16A16_SNORM: return VK_FORMAT_R16G16B16A16_SNORM;
				case EImageFormat::R16G16B16A16_SINT: return VK_FORMAT_R16G16B16A16_SINT;
				case EImageFormat::R32G32_TYPELESS: return VK_FORMAT_R32G32_UINT;
				case EImageFormat::R32G32_FLOAT: return VK_FORMAT_R32G32_SFLOAT;
				case EImageFormat::R32G32_UINT: return VK_FORMAT_R32G32_UINT;
				case EImageFormat::R32G32_SINT: return VK_FORMAT_R32G32_SINT;
				case EImageFormat::R32G8X24_TYPELESS: return VK_FORMAT_D32_SFLOAT_S8_UINT; // possible mismatch!
				case EImageFormat::D32_FLOAT_S8X24_UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;
				case EImageFormat::R32_FLOAT_X8X24_TYPELESS: return VK_FORMAT_R32G32_SFLOAT; // possible mismatch!
				case EImageFormat::X32_TYPELESS_G8X24_UINT: return VK_FORMAT_R32G32_UINT;    // possible mismatch!
				case EImageFormat::R10G10B10A2_TYPELESS: return VK_FORMAT_A2B10G10R10_UINT_PACK32;
				case EImageFormat::R10G10B10A2_UNORM: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
				case EImageFormat::R10G10B10A2_UINT: return VK_FORMAT_A2B10G10R10_UINT_PACK32;
				case EImageFormat::R11G11B10_FLOAT: return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
				case EImageFormat::R8G8B8A8_TYPELESS: return VK_FORMAT_R8G8B8A8_UINT;
				case EImageFormat::R8G8B8A8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;
				case EImageFormat::R8G8B8A8_UNORM_SRGB: return VK_FORMAT_R8G8B8A8_SRGB;
				case EImageFormat::R8G8B8A8_UINT: return VK_FORMAT_R8G8B8A8_UINT;
				case EImageFormat::R8G8B8A8_SNORM: return VK_FORMAT_R8G8B8A8_SNORM;
				case EImageFormat::R8G8B8A8_SINT: return VK_FORMAT_R8G8B8A8_SINT;
				case EImageFormat::R16G16_TYPELESS: return VK_FORMAT_R16G16_UINT;
				case EImageFormat::R16G16_FLOAT: return VK_FORMAT_R16G16_SFLOAT;
				case EImageFormat::R16G16_UNORM: return VK_FORMAT_R16G16_UNORM;
				case EImageFormat::R16G16_UINT: return VK_FORMAT_R16G16_UINT;
				case EImageFormat::R16G16_SNORM: return VK_FORMAT_R16G16_SNORM;
				case EImageFormat::R16G16_SINT: return VK_FORMAT_R16G16_SINT;
				case EImageFormat::R32_TYPELESS: return VK_FORMAT_D32_SFLOAT;
				case EImageFormat::D32_FLOAT: return VK_FORMAT_D32_SFLOAT;
				case EImageFormat::R32_FLOAT: return VK_FORMAT_R32_SFLOAT;
				case EImageFormat::R32_UINT: return VK_FORMAT_R32_UINT;
				case EImageFormat::R32_SINT: return VK_FORMAT_R32_SINT;
				case EImageFormat::R24G8_TYPELESS: return VK_FORMAT_D24_UNORM_S8_UINT;
				case EImageFormat::D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
				case EImageFormat::R24_UNORM_X8_TYPELESS: return VK_FORMAT_D24_UNORM_S8_UINT;
				case EImageFormat::X24_TYPELESS_G8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
				case EImageFormat::R8G8_TYPELESS: return VK_FORMAT_R8G8_UINT;
				case EImageFormat::R8G8_UNORM: return VK_FORMAT_R8G8_UNORM;
				case EImageFormat::R8G8_UINT: return VK_FORMAT_R8G8_UINT;
				case EImageFormat::R8G8_SNORM: return VK_FORMAT_R8G8_SNORM;
				case EImageFormat::R8G8_SINT: return VK_FORMAT_R8G8_SINT;
				case EImageFormat::R16_TYPELESS: return VK_FORMAT_D16_UNORM;
				case EImageFormat::R16_FLOAT: return VK_FORMAT_R16_SFLOAT;
				case EImageFormat::D16_UNORM: return VK_FORMAT_D16_UNORM;
				case EImageFormat::R16_UNORM: return VK_FORMAT_R16_UNORM;
				case EImageFormat::R16_UINT: return VK_FORMAT_R16_UINT;
				case EImageFormat::R16_SNORM: return VK_FORMAT_R16_SNORM;
				case EImageFormat::R16_SINT: return VK_FORMAT_R16_SINT;
				case EImageFormat::R8_TYPELESS: return VK_FORMAT_R8_UINT;
				case EImageFormat::R8_UNORM: return VK_FORMAT_R8_UNORM;
				case EImageFormat::R8_UINT: return VK_FORMAT_R8_UINT;
				case EImageFormat::R8_SNORM: return VK_FORMAT_R8_SNORM;
				case EImageFormat::R8_SINT: return VK_FORMAT_R8_SINT;
				case EImageFormat::A8_UNORM: return VK_FORMAT_R8_UNORM;                         // mismatch!
				case EImageFormat::R1_UNORM: return VK_FORMAT_R8_UNORM;                         // mismatch!
				case EImageFormat::R9G9B9E5_SHAREDEXP: return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32; // maybe ok
				case EImageFormat::R8G8_B8G8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;            // mismatch
				case EImageFormat::G8R8_G8B8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;            // mismatch
				case EImageFormat::BC1_TYPELESS: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
				case EImageFormat::BC1_UNORM: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
				case EImageFormat::BC1_UNORM_SRGB: return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
				case EImageFormat::BC2_TYPELESS: return VK_FORMAT_BC2_UNORM_BLOCK;
				case EImageFormat::BC2_UNORM: return VK_FORMAT_BC2_UNORM_BLOCK;
				case EImageFormat::BC2_UNORM_SRGB: return VK_FORMAT_BC2_SRGB_BLOCK;
				case EImageFormat::BC3_TYPELESS: return VK_FORMAT_BC3_UNORM_BLOCK;
				case EImageFormat::BC3_UNORM: return VK_FORMAT_BC3_UNORM_BLOCK;
				case EImageFormat::BC3_UNORM_SRGB: return VK_FORMAT_BC3_SRGB_BLOCK;
				case EImageFormat::BC4_TYPELESS: return VK_FORMAT_BC4_UNORM_BLOCK;
				case EImageFormat::BC4_UNORM: return VK_FORMAT_BC4_UNORM_BLOCK;
				case EImageFormat::BC4_SNORM: return VK_FORMAT_BC4_SNORM_BLOCK;
				case EImageFormat::BC5_TYPELESS: return VK_FORMAT_BC5_UNORM_BLOCK;
				case EImageFormat::BC5_UNORM: return VK_FORMAT_BC5_UNORM_BLOCK;
				case EImageFormat::BC5_SNORM: return VK_FORMAT_BC5_SNORM_BLOCK;
				case EImageFormat::B5G6R5_UNORM: return VK_FORMAT_B5G6R5_UNORM_PACK16;
				case EImageFormat::B5G5R5A1_UNORM: return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
				case EImageFormat::B8G8R8A8_UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
				case EImageFormat::B8G8R8X8_UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
				case EImageFormat::R10G10B10_XR_BIAS_A2_UNORM: return VK_FORMAT_B10G11R11_UFLOAT_PACK32; // mismatch
				case EImageFormat::B8G8R8A8_TYPELESS: return VK_FORMAT_B8G8R8A8_UINT;
				case EImageFormat::B8G8R8A8_UNORM_SRGB: return VK_FORMAT_B8G8R8A8_SRGB;
				case EImageFormat::B8G8R8X8_TYPELESS: return VK_FORMAT_B8G8R8A8_UINT;
				case EImageFormat::B8G8R8X8_UNORM_SRGB: return VK_FORMAT_B8G8R8A8_SRGB;
				case EImageFormat::BC6H_TYPELESS: return VK_FORMAT_BC6H_SFLOAT_BLOCK;
				case EImageFormat::BC6H_UF16: return VK_FORMAT_BC6H_UFLOAT_BLOCK;
				case EImageFormat::BC6H_SF16: return VK_FORMAT_BC6H_SFLOAT_BLOCK;
				case EImageFormat::BC7_TYPELESS: return VK_FORMAT_BC7_UNORM_BLOCK; // maybe mismatch
				case EImageFormat::BC7_UNORM: return VK_FORMAT_BC7_UNORM_BLOCK;
				case EImageFormat::BC7_UNORM_SRGB: return VK_FORMAT_BC7_SRGB_BLOCK;
				case EImageFormat::B4G4R4A4_UNORM: return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
				default: return VK_FORMAT_UNDEFINED;
			}
		}

		VkPolygonMode ConvertFillMode(EFillMode fillMode) {
			switch (fillMode) {
				case EFillMode::WireFrame:	return VK_POLYGON_MODE_LINE;
				case EFillMode::Solid:		return VK_POLYGON_MODE_FILL;
				default:					return VK_POLYGON_MODE_FILL;
			}
		}

		VkCullModeFlagBits ConvertCullMode(ECullMode cullMode) {
			switch (cullMode) {
				case ECullMode::Back:	return VK_CULL_MODE_BACK_BIT;
				case ECullMode::Front:	return VK_CULL_MODE_FRONT_BIT;
				case ECullMode::None:	return VK_CULL_MODE_NONE;
				default:				return VK_CULL_MODE_NONE;
			}
		}

		VkPrimitiveTopology ConvertTopology(EPrimitiveType primitiveType) {
			switch (primitiveType) {
				case EPrimitiveType::PointList:		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
				case EPrimitiveType::LineList:		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
				case EPrimitiveType::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
				case EPrimitiveType::TriangleList:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				case EPrimitiveType::PatchList:		return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
				default:							return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
			}
		}

		VkCompareOp ConvertComparisonFunc(EComprasionFunc func) {
			switch (func) {
				case EComprasionFunc::Never:		return VK_COMPARE_OP_NEVER;
				case EComprasionFunc::Less:			return VK_COMPARE_OP_LESS;
				case EComprasionFunc::Equal:		return VK_COMPARE_OP_EQUAL;
				case EComprasionFunc::LessEqual:	return VK_COMPARE_OP_LESS_OR_EQUAL;
				case EComprasionFunc::Greater:		return VK_COMPARE_OP_GREATER;
				case EComprasionFunc::NotEqual:		return VK_COMPARE_OP_NOT_EQUAL;
				case EComprasionFunc::GreaterEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
				case EComprasionFunc::Always:		return VK_COMPARE_OP_ALWAYS;
				default:							return VK_COMPARE_OP_NEVER;
			}
		}

		VkBlendFactor ConvertBlend(EBlend blend) {
			switch (blend) {
				case EBlend::Zero:				return VK_BLEND_FACTOR_ZERO;
				case EBlend::One:				return VK_BLEND_FACTOR_ONE;
				case EBlend::SrcColor:			return VK_BLEND_FACTOR_SRC_COLOR;
				case EBlend::InvSrcColor:		return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
				case EBlend::DstColor:			return VK_BLEND_FACTOR_DST_COLOR;
				case EBlend::InvDstColor:		return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
				case EBlend::SrcAlpha:			return VK_BLEND_FACTOR_SRC_ALPHA;
				case EBlend::InvSrcAlpha:		return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				case EBlend::DstAlpha:			return VK_BLEND_FACTOR_DST_ALPHA;
				case EBlend::InvDstAlpha:		return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
				case EBlend::ConstantColor:		return VK_BLEND_FACTOR_CONSTANT_COLOR;
				case EBlend::InvConstantColor:	return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
				case EBlend::ConstantAlpha:		return VK_BLEND_FACTOR_CONSTANT_ALPHA;
				case EBlend::InvConstantAlpha:	return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
				case EBlend::SrcAlphaSaturate:	return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
				default:						return VK_BLEND_FACTOR_ZERO;
			}
		}

		VkBlendOp ConvertBlendOp(EBlendOp blendOp) {
			switch (blendOp) {
				case EBlendOp::Add:				return VK_BLEND_OP_ADD;
				case EBlendOp::Subtract:		return VK_BLEND_OP_SUBTRACT;
				case EBlendOp::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
				case EBlendOp::Min:				return VK_BLEND_OP_MIN;
				case EBlendOp::Max:				return VK_BLEND_OP_MAX;
				default:						return VK_BLEND_OP_ADD;
			}
		}

		// VulkanPipelineState

		VulkanPipelineState::VulkanPipelineState(VulkanRenderDevice &_device, RenderHandlePipelineState &_state) : VulkanResource(_device), state(_state) {
			
		}

		VulkanPipelineState::~VulkanPipelineState() {

		}

		void VulkanPipelineState::Compile() {
			VkRenderPass dummyRenderPass = VK_NULL_HANDLE;

			std::vector<VkAttachmentDescription> attachmentDescriptions;
			std::vector<VkAttachmentReference> colorAttachmentReferences;

			// Color attachemnts
			for (uint32_t i = 0; i < state.colorTargetFormats.size(); ++i) {
				VkAttachmentDescription attachment = {};
				attachment.format = ConvertFormat(state.colorTargetFormats[i]);
				attachment.samples = VK_SAMPLE_COUNT_1_BIT;
				attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachment.finalLayout = VK_IMAGE_LAYOUT_GENERAL;
				attachmentDescriptions.push_back(attachment);

				VkAttachmentReference reference = {};
				reference.attachment = i;
				reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				colorAttachmentReferences.push_back(reference);
			}

			// Subpass
			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = colorAttachmentReferences.size();
			subpass.pColorAttachments = colorAttachmentReferences.data();

			// Depth attachemnt
			if (state.depthTargetFormat != EImageFormat::Unknown) {
				VkAttachmentDescription depthAttachment = {};
				depthAttachment.format = ConvertFormat(state.depthTargetFormat);
				depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
				depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
				depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				depthAttachment.finalLayout = VK_IMAGE_LAYOUT_GENERAL;
				attachmentDescriptions.push_back(depthAttachment);

				VkAttachmentReference depthAttachmentReference = {};
				depthAttachmentReference.attachment = state.colorTargetFormats.size();
				depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

				subpass.pDepthStencilAttachment = &depthAttachmentReference;
			}

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = attachmentDescriptions.size();
			renderPassInfo.pAttachments = attachmentDescriptions.data();
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;

			if (vkCreateRenderPass(device.handle, &renderPassInfo, nullptr, &dummyRenderPass) != VK_SUCCESS)
				throw std::runtime_error("Failed to create render pass!");

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0;            // Optional
			pipelineLayoutInfo.pSetLayouts = nullptr;         // Optional
			pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optional
			pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

			if (vkCreatePipelineLayout(device.handle, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
				throw std::runtime_error("Failed to create pipeline layout!");

			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.layout = pipelineLayout;
			pipelineInfo.renderPass = dummyRenderPass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			// --- Shader stages ---
			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

			// Vertex shader
			if (state.vertexShaderName != "") {
				std::string bytecode = loadShaderBytecode(state.vertexShaderName);
				std::cout << "Vertex shader: " << std::endl;
				std::cout << bytecode.c_str() << std::endl;

				VkShaderModuleCreateInfo moduleInfo = {};
				moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				moduleInfo.codeSize = bytecode.size();
				moduleInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode.data());

				if (vkCreateShaderModule(device.handle, &moduleInfo, nullptr, &vertexShaderModule) != VK_SUCCESS)
					throw std::runtime_error("Failed to create shader module!");

				VkPipelineShaderStageCreateInfo stageInfo = {};
				stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				stageInfo.module = vertexShaderModule;
				stageInfo.pName = "VSmain";

				shaderStages.push_back(stageInfo);
			}

			// Hull shader
			if (state.hullShaderName != "") {
				std::string bytecode = loadShaderBytecode(state.hullShaderName);
				std::cout << "Hull shader: " << std::endl;
				std::cout << bytecode.c_str() << std::endl;

				VkShaderModuleCreateInfo moduleInfo = {};
				moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				moduleInfo.codeSize = bytecode.size();
				moduleInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode.data());

				if (vkCreateShaderModule(device.handle, &moduleInfo, nullptr, &tessContrShaderModule) != VK_SUCCESS)
					throw std::runtime_error("Failed to create shader module!");

				VkPipelineShaderStageCreateInfo stageInfo = {};
				stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				stageInfo.module = tessContrShaderModule;
				stageInfo.pName = "HSmain";

				shaderStages.push_back(stageInfo);
			}

			// Domain shader
			if (state.domainShaderName != "") {
				std::string bytecode = loadShaderBytecode(state.domainShaderName);
				std::cout << "Domain shader: " << std::endl;
				std::cout << bytecode.c_str() << std::endl;

				VkShaderModuleCreateInfo moduleInfo = {};
				moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				moduleInfo.codeSize = bytecode.size();
				moduleInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode.data());

				if (vkCreateShaderModule(device.handle, &moduleInfo, nullptr, &tessEvalShaderModule) != VK_SUCCESS)
					throw std::runtime_error("Failed to create shader module!");

				VkPipelineShaderStageCreateInfo stageInfo = {};
				stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
				stageInfo.module = tessEvalShaderModule;
				stageInfo.pName = "DSmain";

				shaderStages.push_back(stageInfo);
			}

			// Geometry shader
			if (state.geometryShaderName != "") {
				std::string bytecode = loadShaderBytecode(state.geometryShaderName);
				std::cout << "Geoetry shader: " << std::endl;
				std::cout << bytecode.c_str() << std::endl;

				VkShaderModuleCreateInfo moduleInfo = {};
				moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				moduleInfo.codeSize = bytecode.size();
				moduleInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode.data());

				if (vkCreateShaderModule(device.handle, &moduleInfo, nullptr, &geometryShaderModule) != VK_SUCCESS)
					throw std::runtime_error("failed to create shader module!");

				VkPipelineShaderStageCreateInfo stageInfo = {};
				stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
				stageInfo.module = geometryShaderModule;
				stageInfo.pName = "GSmain";

				shaderStages.push_back(stageInfo);
			}

			// Pixel shader
			if (state.pixelShaderName != "") {
				std::string bytecode = loadShaderBytecode(state.pixelShaderName);
				std::cout << "Pixel shader: " << std::endl;
				std::cout << bytecode.c_str() << std::endl;

				VkShaderModuleCreateInfo moduleInfo = {};
				moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				moduleInfo.codeSize = bytecode.size();
				moduleInfo.pCode = reinterpret_cast<const uint32_t*>(bytecode.data());

				if (vkCreateShaderModule(device.handle, &moduleInfo, nullptr, &fragmentShaderModule) != VK_SUCCESS)
					throw std::runtime_error("failed to create shader module!");

				VkPipelineShaderStageCreateInfo stageInfo = {};
				stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				stageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				stageInfo.module = fragmentShaderModule;
				stageInfo.pName = "PSmain";

				shaderStages.push_back(stageInfo);
			}

			pipelineInfo.stageCount = (uint32_t) shaderStages.size();
			pipelineInfo.pStages = shaderStages.data();

			// --- Fixed function states ---

			//Vertex input
			//TODO: Implement this with shader reflection
			VkPipelineVertexInputStateCreateInfo vertexInputState = {};
			vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputState.vertexBindingDescriptionCount = 0;
			vertexInputState.pVertexBindingDescriptions = nullptr; // Optional
			vertexInputState.vertexAttributeDescriptionCount = 0;
			vertexInputState.pVertexAttributeDescriptions = nullptr; // Optional
			pipelineInfo.pVertexInputState = &vertexInputState;

			// Input assembly
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
			inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyState.topology = ConvertTopology(state.primitiveType);
			inputAssemblyState.primitiveRestartEnable = VK_FALSE;
			pipelineInfo.pInputAssemblyState = &inputAssemblyState;

			// Tessellation
			if (state.hullShaderName != "" && state.domainShaderName != "") {
				VkPipelineTessellationStateCreateInfo tessellationState = {};
				tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
				tessellationState.patchControlPoints = state.controlPoints;
				pipelineInfo.pTessellationState = &tessellationState;
			}
			else
				pipelineInfo.pTessellationState = nullptr;

			// Rasterizer
			VkPipelineRasterizationStateCreateInfo rasterizationState = {};
			rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationState.rasterizerDiscardEnable = VK_FALSE;
			rasterizationState.lineWidth = 1.0f;
			rasterizationState.depthClampEnable = (VkBool32) state.depthClipEnable;
			rasterizationState.polygonMode = ConvertFillMode(state.fillMode);
			rasterizationState.cullMode = ConvertCullMode(state.cullMode);
			rasterizationState.frontFace = state.frontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
			rasterizationState.depthBiasEnable = (VkBool32) (state.depthBias != 0);
			rasterizationState.depthBiasConstantFactor = (float) state.depthBias;
			rasterizationState.depthBiasClamp = state.depthBiasClamp;
			rasterizationState.depthBiasSlopeFactor = state.slopeScaledDepthBias;
			pipelineInfo.pRasterizationState = &rasterizationState;

			// Viewport, Scissor
			VkViewport viewport = {};
			viewport.x = 0;
			viewport.y = 0;
			viewport.width = 65535;
			viewport.height = 65535;
			viewport.minDepth = 0;
			viewport.maxDepth = 1;

			VkRect2D scissor = {};
			scissor.extent.width = 65535;
			scissor.extent.height = 65535;

			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;
			pipelineInfo.pViewportState = &viewportState;

			// Depth-Stencil
			VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
			depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilState.depthTestEnable = state.depthTestEnabled ? VK_TRUE : VK_FALSE;
			depthStencilState.depthWriteEnable = state.depthWriteEnabled ? VK_TRUE : VK_FALSE;
			depthStencilState.depthCompareOp = ConvertComparisonFunc(state.depthFunc);
			// TODO stencil!
			depthStencilState.stencilTestEnable = state.stencilTestEnabled ? VK_TRUE : VK_FALSE;
			depthStencilState.front = depthStencilState.back;
			depthStencilState.back.compareOp = VK_COMPARE_OP_ALWAYS;
			pipelineInfo.pDepthStencilState = &depthStencilState;

			// MSAA
			VkPipelineMultisampleStateCreateInfo multisampleState = {};
			multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampleState.sampleShadingEnable = VK_FALSE;
			multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampleState.minSampleShading = 1.0f;
			multisampleState.pSampleMask = nullptr;
			multisampleState.alphaToCoverageEnable = VK_FALSE;
			multisampleState.alphaToOneEnable = VK_FALSE;
			pipelineInfo.pMultisampleState = &multisampleState;

			// Blending:
			std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments(state.colorTargetFormats.size());
			for (size_t i = 0; i < colorBlendAttachments.size(); ++i) {
				auto renderTargetBlend = state.renderTargetBlends[i];

				colorBlendAttachments[i].blendEnable = renderTargetBlend.blendEnable ? VK_TRUE : VK_FALSE;

				colorBlendAttachments[i].colorWriteMask = 0; 
				if (renderTargetBlend.colorWriteMask == (renderTargetBlend.colorWriteMask & EColorWriteMask::Red))
					colorBlendAttachments[i].colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
				if (renderTargetBlend.colorWriteMask == (renderTargetBlend.colorWriteMask & EColorWriteMask::Green))
					colorBlendAttachments[i].colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
				if (renderTargetBlend.colorWriteMask == (renderTargetBlend.colorWriteMask & EColorWriteMask::Blue))
					colorBlendAttachments[i].colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
				if (renderTargetBlend.colorWriteMask == (renderTargetBlend.colorWriteMask & EColorWriteMask::Alpha))
					colorBlendAttachments[i].colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;

				colorBlendAttachments[i].srcColorBlendFactor = ConvertBlend(renderTargetBlend.srcBlend);
				colorBlendAttachments[i].dstColorBlendFactor = ConvertBlend(renderTargetBlend.dstBlend);
				colorBlendAttachments[i].colorBlendOp = ConvertBlendOp(renderTargetBlend.blendOp);
				colorBlendAttachments[i].srcAlphaBlendFactor = ConvertBlend(renderTargetBlend.srcBlendAlpha);
				colorBlendAttachments[i].dstAlphaBlendFactor = ConvertBlend(renderTargetBlend.dstBlendAlpha);
				colorBlendAttachments[i].alphaBlendOp = ConvertBlendOp(renderTargetBlend.blendOpAlpha);
			}

			VkPipelineColorBlendStateCreateInfo colorBlendState = {};
			colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendState.logicOpEnable = VK_FALSE;
			colorBlendState.logicOp = VK_LOGIC_OP_COPY;
			colorBlendState.attachmentCount = (uint32_t) colorBlendAttachments.size();
			colorBlendState.pAttachments = colorBlendAttachments.data();
			colorBlendState.blendConstants[0] = 1.0f;
			colorBlendState.blendConstants[1] = 1.0f;
			colorBlendState.blendConstants[2] = 1.0f;
			colorBlendState.blendConstants[3] = 1.0f;
			pipelineInfo.pColorBlendState = &colorBlendState;

			// Dynamic state
			VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR,
				VK_DYNAMIC_STATE_STENCIL_REFERENCE,
				VK_DYNAMIC_STATE_BLEND_CONSTANTS,
			};

			VkPipelineDynamicStateCreateInfo dynamicState = {};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.dynamicStateCount = 4;
			dynamicState.pDynamicStates = dynamicStates;
			pipelineInfo.pDynamicState = &dynamicState;

			//TODO: Implement cacheing mechanism
			if (vkCreateGraphicsPipelines(device.handle, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
				throw std::runtime_error("Failed to create the graphics pipeline!");

			// Cleanup

			// Destroy the dummy pass
			vkDestroyRenderPass(device.handle, dummyRenderPass, nullptr);

			// Destroy shader modules
			if (state.vertexShaderName != "")
				vkDestroyShaderModule(device.handle, vertexShaderModule, nullptr);
			if (state.hullShaderName != "")
				vkDestroyShaderModule(device.handle, tessContrShaderModule, nullptr);
			if (state.domainShaderName != "")
				vkDestroyShaderModule(device.handle, tessEvalShaderModule, nullptr);
			if (state.geometryShaderName != "")
				vkDestroyShaderModule(device.handle, geometryShaderModule, nullptr);
			if (state.pixelShaderName != "")
				vkDestroyShaderModule(device.handle, fragmentShaderModule, nullptr);
		}

		// VulkanBuffer

        VulkanBuffer::VulkanBuffer(VulkanRenderDevice& _device, RenderHandleBuffer &buffer) : VulkanResource(_device) {
			
		}

		VulkanBuffer::~VulkanBuffer() {

		}

		// VulkanTexture
        
		VulkanTexture::VulkanTexture(VulkanRenderDevice& _device, RenderHandleTexture &texture) : VulkanResource(_device) {

		}

		VulkanTexture::~VulkanTexture() {

		}
	}
}