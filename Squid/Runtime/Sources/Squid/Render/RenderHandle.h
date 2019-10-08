#pragma once
#include "../pch.h"
#include "RenderDevice.h"

namespace Squid {
	namespace Render {

		struct RenderHandle {
			unsigned int randomChar() {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(0, 255);
				return dis(gen);
			}

			std::string generateHex(const uint32_t len) {
				std::stringstream ss;
				for (uint32_t i = 0; i < len; i++) {
					const unsigned int rc = randomChar();
					std::stringstream hexstream;
					hexstream << std::hex << rc;
					auto hex = hexstream.str();
					ss << (hex.length() < 2 ? '0' + hex : hex);
				}
				return ss.str();
			}

			std::string uuid;

			
		
			RenderHandle() {
				uuid = generateHex(16);
			}

        protected:
            std::vector<Ref<RenderDevice>> referencedDevices;
		};

		// RenderHandle - PipelineState
		enum class EImageFormat : uint32_t {
			Unknown,
			R32G32B32A32_TYPELESS,
			R32G32B32A32_FLOAT,
			R32G32B32A32_UINT,
			R32G32B32A32_SINT,
			R32G32B32_TYPELESS,
			R32G32B32_FLOAT,
			R32G32B32_UINT,
			R32G32B32_SINT,
			R16G16B16A16_TYPELESS,
			R16G16B16A16_FLOAT,
			R16G16B16A16_UNORM,
			R16G16B16A16_UINT,
			R16G16B16A16_SNORM,
			R16G16B16A16_SINT,
			R32G32_TYPELESS,
			R32G32_FLOAT,
			R32G32_UINT,
			R32G32_SINT,
			R32G8X24_TYPELESS,
			D32_FLOAT_S8X24_UINT,
			R32_FLOAT_X8X24_TYPELESS,
			X32_TYPELESS_G8X24_UINT,
			R10G10B10A2_TYPELESS,
			R10G10B10A2_UNORM,
			R10G10B10A2_UINT,
			R11G11B10_FLOAT,
			R8G8B8A8_TYPELESS,
			R8G8B8A8_UNORM,
			R8G8B8A8_UNORM_SRGB,
			R8G8B8A8_UINT,
			R8G8B8A8_SNORM,
			R8G8B8A8_SINT,
			R16G16_TYPELESS,
			R16G16_FLOAT,
			R16G16_UNORM,
			R16G16_UINT,
			R16G16_SNORM,
			R16G16_SINT,
			R32_TYPELESS,
			D32_FLOAT,
			R32_FLOAT,
			R32_UINT,
			R32_SINT,
			R24G8_TYPELESS,
			D24_UNORM_S8_UINT,
			R24_UNORM_X8_TYPELESS,
			X24_TYPELESS_G8_UINT,
			R8G8_TYPELESS,
			R8G8_UNORM,
			R8G8_UINT,
			R8G8_SNORM,
			R8G8_SINT,
			R16_TYPELESS,
			R16_FLOAT,
			D16_UNORM,
			R16_UNORM,
			R16_UINT,
			R16_SNORM,
			R16_SINT,
			R8_TYPELESS,
			R8_UNORM,
			R8_UINT,
			R8_SNORM,
			R8_SINT,
			A8_UNORM,
			R1_UNORM,
			R9G9B9E5_SHAREDEXP,
			R8G8_B8G8_UNORM,
			G8R8_G8B8_UNORM,
			BC1_TYPELESS,
			BC1_UNORM,
			BC1_UNORM_SRGB,
			BC2_TYPELESS,
			BC2_UNORM,
			BC2_UNORM_SRGB,
			BC3_TYPELESS,
			BC3_UNORM,
			BC3_UNORM_SRGB,
			BC4_TYPELESS,
			BC4_UNORM,
			BC4_SNORM,
			BC5_TYPELESS,
			BC5_UNORM,
			BC5_SNORM,
			B5G6R5_UNORM,
			B5G5R5A1_UNORM,
			B8G8R8A8_UNORM,
			B8G8R8X8_UNORM,
			R10G10B10_XR_BIAS_A2_UNORM,
			B8G8R8A8_TYPELESS,
			B8G8R8A8_UNORM_SRGB,
			B8G8R8X8_TYPELESS,
			B8G8R8X8_UNORM_SRGB,
			BC6H_TYPELESS,
			BC6H_UF16,
			BC6H_SF16,
			BC7_TYPELESS,
			BC7_UNORM,
			BC7_UNORM_SRGB,
			AYUV,
			Y410,
			Y416,
			NV12,
			P010,
			P016,
			YUY2,
			Y210,
			Y216,
			NV11,
			AI44,
			IA44,
			P8,
			A8P8,
			B4G4R4A4_UNORM,
			FORCE_UINT = 0xffffffff,
		};

		enum class EPrimitiveType : uint8_t {
			PointList,
			LineList,
			TriangleStrip,
			TriangleList,
			PatchList
		};

		enum class EFillMode : uint8_t {
			Solid,
			WireFrame
		};

		enum class ECullMode : uint8_t {
			Back,
			Front,
			None
		};

		enum class EComprasionFunc : uint8_t {
			Never,
			Less,
			Equal,
			LessEqual,
			Greater,
			NotEqual,
			GreaterEqual,
			Always
		};

		enum class EBlendOp : uint8_t {
			Add,
			Subtract,
			ReverseSubtract,
			Min,
			Max
		};

		enum class EBlend : uint8_t {
			Zero,
			One,
			SrcColor,
			InvSrcColor,
			DstColor,
			InvDstColor,
			SrcAlpha,
			InvSrcAlpha,
			DstAlpha,
			InvDstAlpha,
			ConstantColor,
			InvConstantColor,
			ConstantAlpha,
			InvConstantAlpha,
			SrcAlphaSaturate
		};

		enum class EColorWriteMask : uint8_t {
			Disable = 0 << 0,
			Red = 0 << 1,
			Green = 0 << 2,
			Blue = 0 << 3,
			Alpha = 0 << 4,
			All = Red | Green | Blue | Alpha
		};

		EColorWriteMask constexpr operator &(EColorWriteMask x, EColorWriteMask y) {
			return static_cast<EColorWriteMask> (
				static_cast<std::underlying_type<EColorWriteMask>::type>(x) &
				static_cast<std::underlying_type<EColorWriteMask>::type>(y)
			);
		}

		struct RenderHandlePipelineState : public RenderHandle {
			
			//RenderHandlePipelineState() : RenderHandleTyped() {}

			//RenderHandlePipelineState() {
			//	handleType = PipelineState;
			//}

			struct RenderTargetBlend {
				bool blendEnable = false;

				EBlend srcBlend = EBlend::SrcAlpha;
				EBlend dstBlend = EBlend::InvSrcAlpha;
				EBlendOp blendOp = EBlendOp::Add;

				EBlend srcBlendAlpha = EBlend::One;
				EBlend dstBlendAlpha = EBlend::One;
				EBlendOp blendOpAlpha = EBlendOp::Add;

				EColorWriteMask colorWriteMask = EColorWriteMask::All;
			};

			std::vector<EImageFormat> colorTargetFormats;
			EImageFormat depthTargetFormat = EImageFormat::Unknown;

			// Input assembler stage
			EPrimitiveType primitiveType = EPrimitiveType::TriangleList;

			// Tesselation stage
			int controlPoints = 3;

			// Rasterizer stage
			EFillMode fillMode = EFillMode::Solid;
			ECullMode cullMode = ECullMode::None;
			bool frontCounterClockwise = false;
			int depthBias = 0;
			float depthBiasClamp = 0.0f;
			float slopeScaledDepthBias = 0.0f;
			bool depthClipEnable = false;
			bool multisampleEnable = false;
			bool antialiasedLineEnable = false;
			bool conservativeRasterizationEnable = false;
			uint32_t forcedSampleCount = 0;

			// Depth-Stencil
			bool depthTestEnabled = false;
			bool depthWriteEnabled = false;
			EComprasionFunc depthFunc = EComprasionFunc::Never;
			bool stencilTestEnabled = false;

			// Blend state
			bool alphaToCoverageEnable = false;
			bool independentBlendEnable = false;
			RenderTargetBlend renderTargetBlends[8];

			// Programable stages
			std::string vertexShaderName = "";
			std::string pixelShaderName = "";
			std::string geometryShaderName = "";
			std::string hullShaderName = "";
			std::string domainShaderName = "";

            bool Compile() {
				for (auto& d : referencedDevices) {
					d->DispatchPipelineStateHandle(*this, "Compile");
				}
				return true;
            }

			void AddDevice(Ref<RenderDevice> d) {
				d->LoadPipelineStateHandle(*this);
				referencedDevices.push_back(d);
			}

		};

		// RenderHandle - Buffer

		struct RenderHandleBuffer : public RenderHandle {};

		// RenderHandle - Texture


		struct RenderHandleTexture : public RenderHandle {};
	}
}