#include "VulkanRDI.h"
#include "VulkanDevice.h"

namespace {
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation",
		"VK_LAYER_LUNARG_monitor",
		"VK_LAYER_LUNARG_api_dump"
	};

	// Get the required list of extensions based on whether validation layers are enabled
	std::vector<const char*> getRequiredExtensions() {
		std::vector<const char*> extensions;

#ifdef DEBUG
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

		return extensions;
	}

	// Callback function to allow messages from validation layers to be received
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code,
		const char* layerPrefix, const char* msg, void* userData) {
		fprintf(stderr, "Validation layer: %s\n", msg);
		return VK_FALSE;
	}
}

using namespace Squid;
using namespace Squid::Render;

SQUID_API RDI* CreateRDI()
{
    // call the constructor of the actual implementation
    RDI* renderDeviceInterface = new VulkanRDI();
    // return the created function
    return renderDeviceInterface;
}

VulkanRDI::VulkanRDI() {
	std::cout << "constructed Vulkan RDI" << '\n';
	
    // --- Specify details about our application ---
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Test";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Squid";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;

	// --- Get extensions necessary for Vulkan to interface with GLFW ---
	auto extensions = ::getRequiredExtensions();
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#ifdef SQUID_WIN32
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif
	extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#endif

	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

	// Specify global validation layers
#ifdef DEBUG
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
    instanceCreateInfo.enabledLayerCount = 0;
#endif
    for (auto i = extensions.begin(); i != extensions.end(); ++i)
        std::cout << *i << ' ';
    
	// --- Create Vulkan instance ---
	if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance");
	}

#ifdef DEBUG
    VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
    callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    callbackCreateInfo.pfnCallback = debugCallback;

    if ([&]() {
            auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
            if (func != nullptr) {
                return func(instance, &callbackCreateInfo, nullptr, &debugReportCallback);
            }
            else {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
        }() != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug callback");
    }
#endif
}

void VulkanRDI::Destroy() {
	// --- Destroy Vulkan instance ---
#ifdef DEBUG
	auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (vkDestroyDebugReportCallbackEXT != nullptr) {
		vkDestroyDebugReportCallbackEXT(instance, debugReportCallback, nullptr);
	}
#endif

	if (instance != VK_NULL_HANDLE) {
		vkDestroyInstance(instance, nullptr);
		instance = nullptr;
	}

	delete this;
}

Scope<RenderDevice> VulkanRDI::CreateDevice() {
    // --- Create abstract device handle ---
	std::cout << "create device" << '\n';
	
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	//If no device found return empty vector
	if (deviceCount == 0)
		return nullptr;

	//If found any device then get the inforation abaut them
	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

	std::vector<int> deviceInfos;


	for (const auto& physicalDevice : physicalDevices) {
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		VkPhysicalDeviceMemoryProperties memoryProperties;

		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

		if (deviceFeatures.tessellationShader)
			gpu = physicalDevice;
	}

	std::cout << "----------------------------------------------------------------------" << std::endl;

	return MakeScope<VulkanRenderDevice>(instance, gpu);
}

void VulkanRDI::StartFrame() {
    
}

void VulkanRDI::EndFrame() {
	
}
