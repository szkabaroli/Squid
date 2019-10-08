function addModule(modulePath)
	files ("Backends/" .. modulePath .. "/**.cpp")
	files ("Backends/" .. modulePath .. "/**.h") --<for headers
	includedirs ("Backends/" .. modulePath .. "/")
end

project "SquidVulkan"
	kind "SharedLib"
	
	includedirs "%{os.getenv('VULKAN_SDK')}\\Include"
	links "%{os.getenv('VULKAN_SDK')}\\lib\\vulkan-1.lib" 

	files ("Backends/RDI/Vulkan/**.cpp")
	files ("Backends/RDI/Vulkan/**.h") --<for headers
	includedirs {  "Sources" }

	targetdir "../Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "../Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	postbuildcommands {
		"{COPY} %{cfg.targetdir}/SquidVulkan.dll %{_OPTIONS['projectdir']}/Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Sandbox/"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "on"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "on"

project "SquidRuntime"
	kind "StaticLib"
	
	targetdir "../Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "../Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	
	--pchsource "Sources/Squid/pch.cpp"
	--pchheader "pch.h"
	
	files {
		"Sources/**.cpp",
		"Sources/**.h" 
	}
	
	includedirs {
		"Sources"
	}
	
	--Windows platform
	filter "system:windows"
		--desktop platform
		addModule("Platform/Desktop");
		includedirs "../ThirdParty/SDL2/include"
		links {
			"../ThirdParty/SDL2/lib/SDL2",
			"../ThirdParty/SDL2/lib/SDL2main"
		}
	
	--Linux platform
	filter "system:linux"
		--desktop platform
		addModule("Platform/Desktop");
		includedirs "../ThirdParty/SDL2/include"
		links {
			"../ThirdParty/SDL2/lib/SDL2",
			"../ThirdParty/SDL2/lib/SDL2main"
		}
	
	-- configurations
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "on"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "on"