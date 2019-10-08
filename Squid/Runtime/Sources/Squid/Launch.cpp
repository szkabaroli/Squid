#include "pch.h"

#include <Squid/Platform/Application.h>
#include <Squid/Platform/Window.h>
#include <Squid/Render/RDI.h>
#include <Squid/Render/RenderHandle.h>
#include <Squid/Render/RenderCommand.h>

using namespace Squid;
using namespace Squid::Platform;
using namespace Squid::Render;

int SquidMain(int argc, char** argv) {
	Scope<Application> app = Application::CreateApplication();
	Ref<Window> win = Window::CreateWindowTest("Squid", 800, 640);

	void* vulkanLib = app->OpenLibrary("SquidVulkan");
	PFN_CreateRDI CreateRDI = (PFN_CreateRDI)app->LoadFunction(vulkanLib, "CreateRDI");

	if (CreateRDI == nullptr)
		throw std::runtime_error("error in loading vulkan backend");
	
	RDI* renderDeviceInterface = CreateRDI();

	//Render Scope
	{
		Ref<RenderDevice> renderDevice;
		renderDevice = renderDeviceInterface->CreateDevice();
		renderDevice->CreateSwapchain(*win.get());

		//std::vector<RenderTarget>& backbuffers = renderDevice->GetBackbuffers(win->id);

		RenderHandlePipelineState gbufferState;
		gbufferState.vertexShaderName = "vert";
		gbufferState.pixelShaderName = "frag";
		gbufferState.AddDevice(renderDevice);
		gbufferState.Compile();
		
		RenderCommandList list;
		list.Begin();
		
		list.SetViewport(800, 640);
		list.SetScissor(0, 0, 800, 640);

		//list.BeginRenderPass(rt);

		//ClearValue clearValue = { 0.0f, 0.0f, 0.0f, 0.0f };
		//list.ClearColorAttachment(0, &clearValue);
		list.Draw(gbufferState, 3, 0);

		list.EndRenderPass();
		list.End();

		renderDevice->CompileCommandList(list);
		
		app->SetFrameCallback([&renderDevice]() {
			//renderDevice->BeginFrame(0);
			//renderDevice->EndFrame(0);
		});

		app->Start();
	}

	renderDeviceInterface->Destroy();
	app->FreeLibrary(vulkanLib);
	
	return 0;
}