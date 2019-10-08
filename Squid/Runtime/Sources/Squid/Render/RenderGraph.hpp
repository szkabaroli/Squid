#pragma once
#include "../pch.h"
#include "RenderDevice.h"

enum class ERenderGraphQueueFlagBits {
	RenderGraphGraphics = 1 << 0,
	RenderGraphCompute = 1 << 1,
	RenderGraphAsyncGraphics = 1 << 2,
	RenderGraphAsyncCompute = 1 << 3,
};

struct AttachmentInfo {
	//SizeClass size_class = SizeClass::SwapchainRelative;
	//EAttachentFormat format;
	float sizeX = 1.0f;
	float sizeY = 1.0f;
	float sizeZ = 0.0f;
	unsigned samples = 1;
	unsigned levels = 1;
	unsigned layers = 1;
	//EImageUsageFlags aux_usage = 0;
	bool persistent = true;
	bool unormSRGBAlias = false;
};

struct BufferInfo {
	//VkDeviceSize size = 0;
	//EBufferUsageFlags usage = 0;
	bool persistent = true;

	bool operator==(const BufferInfo &other) const {
		return //size == other.size &&
	           //usage == other.usage &&
	          persistent == other.persistent;
	}

	bool operator!=(const BufferInfo &other) const {
		return !(*this == other);
	}
};

class RenderResource {
public:
};

class RenderPass {
friend class RenderGraph;
public:
    RenderPass(RenderGraph &graph, uint32_t index, ERenderGraphQueueFlagBits queueFlags) : graph(graph), passIndex(index), queue(queue) {};

    RenderResource& SetDepthStencilInput(const std::string &name);
	RenderResource& SetDepthStencilOutput(const std::string &name, const AttachmentInfo &info);
	
    RenderResource &AddColorOutput(const std::string &name, const AttachmentInfo &info, const std::string &input = "");
	RenderResource &AddAttachmentInput(const std::string &name);

    RenderResource &AddTextureInput(const std::string &name);

    /*void SetOrGetClearDepthStencil(std::function<bool (EClearDepthStencilValue *)> func)
	{
		GetOrClearDepthStencilCallback = std::move(func);
	}*/

	/*void set_get_clear_color(std::function<bool (unsigned, EClearColorValue *)> func)
	{
		GetOrClearColorCallback = std::move(func);
	}*/
private:
	RenderGraph &graph;

    std::string renderPassName;
	uint32_t passIndex;
	RenderDeviceQueue queue;

	std::vector<RenderTextureResource *> colorOutputs;
	std::vector<RenderTextureResource *> colorInputs;
	std::vector<RenderTextureResource *> attachmentsInputs;
	RenderTextureResource *depthStencilInput = nullptr;
	RenderTextureResource *depthStencilOutput = nullptr;

    //std::function<void (ECommandList &)> BuildRenderPassCallback;
	//std::function<bool (EClearDepthStencilValue *)> GetOrClearDepthStencilCallback;
	//std::function<bool (unsigned, EClearColorValue *)> GetOrClearColorCallback;
};

class RenderGraph {
public:

    RenderGraph();

	void Validate();
    RenderPass& AddPass(std::string* passName, ERenderGraphQueueFlagBits queueFlags);

private:
	RenderDevice* device = nullptr;

	std::vector<Scope<RenderPass>> passes;
	std::vector<Scope<RenderResource>> resources;
	std::unordered_map<std::string, uint32_t> passToIndex;
	std::unordered_map<std::string, uint32_t> resourceToIndex;
	std::string backbufferSource;
};