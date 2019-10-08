#pragma once
#include "../pch.h"
#include <Squid/Platform/Window.h>

namespace Squid {
	using namespace Platform;
	namespace Render {

		struct RenderCommandList; 

		struct RenderCommandBegin;
		struct RenderCommandEnd;
		struct RenderCommandBeginRenderPass;
		struct RenderCommandEndRenderPass;
		struct RenderCommandSetViewport;
		struct RenderCommandSetScissor;
		struct RenderCommandClearColor;
		struct RenderCommandDraw;

		struct RenderHandle;
		struct RenderHandlePipelineState;

		enum ERenderHandleType {
			PipelineState
		};
		
		enum EDeviceType {
			Integrated,
			Discrate
		};

		enum EPipelineStage {
			Vertex,
			TessControl,
			TessEvaluation,
			Geometry,
			Pixel,
			Compute
		};

		struct RenderDeviceInfo {
			uint32_t deviceIndex;
			const char* deviceName;
			const char* vendor;
			uint32_t driverVersion;
			EDeviceType deviceType;
			//std::vector<EPipelineStage> supportedStages;
		};

		class RenderDevice {
		public:
			virtual ~RenderDevice() {} // <= important!
			virtual void CreateSwapchain(const Window &) = 0;

			virtual bool LoadPipelineStateHandle(RenderHandlePipelineState &) = 0;
			virtual void DispatchPipelineStateHandle(const RenderHandlePipelineState &, std::string methodName) = 0;

			virtual void BeginFrame(uint32_t windowId) = 0;
			virtual void EndFrame(uint32_t windowId) = 0;
				
			bool CompileCommandList(const RenderCommandList& cmdList);

		protected:
			//Compile list native methods
			virtual bool CompileCommand(const RenderCommandBegin& cmd) = 0;
			virtual bool CompileCommand(const RenderCommandEnd& cmd) = 0;

			virtual bool CompileCommand(const RenderCommandBeginRenderPass& cmd) = 0;
			virtual bool CompileCommand(const RenderCommandEndRenderPass& cmd) = 0;
			virtual bool CompileCommand(const RenderCommandSetViewport& cmd) = 0;
			virtual bool CompileCommand(const RenderCommandSetScissor& cmd) = 0;
			virtual bool CompileCommand(const RenderCommandClearColor& cmd) = 0;
			virtual bool CompileCommand(const RenderCommandDraw& cmd) = 0;
		};
	}
}


