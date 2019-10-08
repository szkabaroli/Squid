#include "RenderDevice.h"
#include "RenderCommand.h"

#define COMPILE_CMD(TYPE_STRUCT)										\
		case TYPE_STRUCT::Type:											\
			if(!CompileCommand(*static_cast<TYPE_STRUCT*>(cmd.get())))	\
				return false;											\
				break;													\

namespace Squid {
	namespace Render {
		bool RenderDevice::CompileCommandList(const RenderCommandList& list) {
			if (list.recorded.size() == 0)
				return false;

			std::cout << "Compiling list with size of: " << list.recorded.size() << std::endl;
			
			for (auto& cmd : list.recorded) {
				switch (cmd->type) {
					COMPILE_CMD(RenderCommandBegin);
					COMPILE_CMD(RenderCommandEnd);
					COMPILE_CMD(RenderCommandBeginRenderPass);
					COMPILE_CMD(RenderCommandEndRenderPass);
					COMPILE_CMD(RenderCommandSetViewport);
					COMPILE_CMD(RenderCommandSetScissor);
					COMPILE_CMD(RenderCommandClearColor);
					COMPILE_CMD(RenderCommandDraw);
				default:
					return false;
				}
			}
			return true;
		};

		
	}
}

#undef COMPILE_CMD