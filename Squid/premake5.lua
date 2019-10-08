newoption {
	trigger     = "projectdir",
	value       = "FILE",
	description = "The game project location"
}

workspace "Squid"
	architecture "x64"
	location (_OPTIONS["projectdir"] .. "/Build/" .. _ACTION)
	configurations { "Debug", "Release" }

	language "C++"	
	cppdialect "C++11"

	--Windows platform
	filter "system:windows"
		defines {
			"SQUID_WIN32",
			"SQUID_MICROSOFT"
		}
	
	--Linux platform
	filter "system:linux"
		defines {
			"SQUID_LINUX",
			"SQUID_POSIX"
		}

function addModule(modulePath)
	files ("Runtime/Backends/" .. modulePath .. "/**.cpp")
	files ("Runtime/Backends/" .. modulePath .. "/**.h") --<for headers
	includedirs ("Runtime/Backends/" .. modulePath .. "/")
end

project "SquidVulkan"
	kind "SharedLib"
	
	includedirs "%{os.getenv('VULKAN_SDK')}\\Include"
	links "%{os.getenv('VULKAN_SDK')}\\lib\\vulkan-1.lib" 

	files ("Runtime/Backends/RDI/Vulkan/**.cpp")
	files ("Runtime/Backends/RDI/Vulkan/**.h") --<for headers
	includedirs {  "Runtime/Sources" }

	targetdir "Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

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
	
	targetdir "Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	
	--pchsource "Sources/Squid/pch.cpp"
	--pchheader "pch.h"
	
	files {
		"Runtime/Sources/**.cpp",
		"Runtime/Sources/**.h" 
	}
	
	includedirs {
		"Runtime/Sources"
	}
	
	--Windows platform
	filter "system:windows"
		--desktop platform
		addModule("Platform/Desktop");
		includedirs "ThirdParty/SDL2/include"
		links {
			"ThirdParty/SDL2/lib/SDL2",
			"ThirdParty/SDL2/lib/SDL2main"
		}
	
	--Linux platform
	filter "system:linux"
		--desktop platform
		addModule("Platform/Desktop");
		includedirs "ThirdParty/SDL2/include"
		links {
			"ThirdParty/SDL2/lib/SDL2",
			"ThirdParty/SDL2/lib/SDL2main"
		}
	
	-- configurations
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "on"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "on"

project "Sandbox"
	links {"SquidRuntime"}
	links {"SquidVulkan"}
	
	targetdir "%{_OPTIONS['projectdir']}/Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir "%{_OPTIONS['projectdir']}/Intermediate/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	
	debugdir "%{_OPTIONS['projectdir']}/Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	files { "%{_OPTIONS['projectdir']}/Sandbox/Sources/**.cpp", "Shaders/**.spv" }
	includedirs { "Runtime/Sources", "Runtime/Modules/Platform/Desktop" }

	postbuildcommands {
		"{COPY} %{os.getcwd()}/Shaders/**.spv %{_OPTIONS['projectdir']}/Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}/Shaders"
	}

	filter "files:frag.spv"
		buildaction "Copy"
	
	filter "configurations:Debug"
		kind "ConsoleApp"
		defines { "DEBUG" }
		symbols "on"

	filter "configurations:Release"
		kind "WindowedApp"
		defines { "NDEBUG" }
		optimize "on"