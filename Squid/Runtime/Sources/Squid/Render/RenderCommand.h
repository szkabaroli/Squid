#pragma once
#include "../pch.h"
#include <Squid/Render/RenderHandle.h>

namespace Squid {
	namespace Render {
		enum class ERenderCommandType : uint16_t
		{
			None = 1 << 0,
			Begin = 1 << 1,
			End = 1 << 2,
			BeginRenderPass = 1 << 3,
			EndRenderPass = 1 << 4,
			SetViewport = 1 << 5,
			SetScissor = 1 << 6,
			ClearColor = 1 << 7,
			SetPipeline = 1 << 8,
			Draw = 1 << 9,
		};

		enum class ERenderCommandQueueType : uint8_t
		{
			None = 0 << 0,
			Copy = 0 << 1,
			Compute = 0 << 2,
			Graphics = 0 << 3,
			All = Copy | Compute | Graphics
		};

		struct RenderCommand
		{
			ERenderCommandType type = ERenderCommandType::None;
		};

		template<ERenderCommandType _commandType, ERenderCommandQueueType _queueType>
		struct RenderCommandTyped : public RenderCommand
		{
			static const ERenderCommandType Type = _commandType;
			static const ERenderCommandQueueType QueueType = _queueType;

			RenderCommandTyped() {
				type = Type;
			}
		};

		struct RenderCommandBegin : public RenderCommandTyped<ERenderCommandType::Begin, ERenderCommandQueueType::All> {};
		struct RenderCommandEnd : public RenderCommandTyped<ERenderCommandType::End, ERenderCommandQueueType::All> {};

		struct RenderCommandBeginRenderPass : public RenderCommandTyped<ERenderCommandType::BeginRenderPass, ERenderCommandQueueType::Graphics> {};
		struct RenderCommandEndRenderPass : public RenderCommandTyped<ERenderCommandType::EndRenderPass, ERenderCommandQueueType::Graphics> {};

		struct RenderCommandSetViewport : public RenderCommandTyped<ERenderCommandType::SetViewport, ERenderCommandQueueType::Graphics> {
			uint32_t width = 0;
			uint32_t height = 0;
		};

		struct RenderCommandSetScissor : public RenderCommandTyped<ERenderCommandType::SetScissor, ERenderCommandQueueType::Graphics> {
			uint32_t x = 0;
			uint32_t y = 0;
			uint32_t width = 0;
			uint32_t height = 0;
		};

		struct ClearValue {
			union {
				struct {
					float		r;
					float       g;
					float       b;
					float       a;
				};
				struct {
					float       depth;
					uint32_t    stencil;
				};
			};
		};

		struct RenderCommandClearColor : public RenderCommandTyped<ERenderCommandType::ClearColor, ERenderCommandQueueType::Graphics> {
			ClearValue clearValue;
		};

		struct RenderCommandDraw : public RenderCommandTyped<ERenderCommandType::Draw, ERenderCommandQueueType::Graphics> {
			RenderHandlePipelineState pipelineState;

			uint32_t firstVertex = 0;
			uint32_t vertexCount = 0;
		};

		// CommandList

		struct RenderCommandList {
			void Begin() {
				Scope<RenderCommandBegin> cmd = MakeScope<RenderCommandBegin>();
				recorded.push_back(std::move(cmd));
			};

			void End() {
				Scope<RenderCommandEnd> cmd = MakeScope<RenderCommandEnd>();
				recorded.push_back(std::move(cmd));
			};

			void BeginRenderPass() {
				Scope <RenderCommandBeginRenderPass> cmd = MakeScope<RenderCommandBeginRenderPass>();
				recorded.push_back(std::move(cmd));
			};

			void EndRenderPass() {
				Scope <RenderCommandEndRenderPass> cmd = MakeScope<RenderCommandEndRenderPass>();
				recorded.push_back(std::move(cmd));
			};

			void SetViewport(uint32_t width, uint32_t height) {
				Scope <RenderCommandSetViewport> cmd = MakeScope<RenderCommandSetViewport>();
				cmd->width = width;
				cmd->height = height;
				recorded.push_back(std::move(cmd));
			};

			void SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
				Scope<RenderCommandSetScissor> cmd = MakeScope<RenderCommandSetScissor>();
				cmd->x = x;
				cmd->y = y;
				cmd->width = width;
				cmd->height = height;
				recorded.push_back(std::move(cmd));
			}

			void ClearColor(uint32_t index, const ClearValue& clearValue) {
				Scope<RenderCommandClearColor> cmd = MakeScope<RenderCommandClearColor>();
				cmd->clearValue = clearValue;
				recorded.push_back(std::move(cmd));
			};

			void Draw(const RenderHandlePipelineState& pipelineState, uint32_t vertexCount, uint32_t firstVertex) {
				Scope<RenderCommandDraw> cmd = MakeScope<RenderCommandDraw>();
				cmd->pipelineState = pipelineState;
				cmd->vertexCount = vertexCount;
				cmd->firstVertex = firstVertex;
				recorded.push_back(std::move(cmd));
			};
			std::vector<Scope<RenderCommand>> recorded;
		private:
		};
	}
}