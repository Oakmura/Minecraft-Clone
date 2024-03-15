workspace "MinecraftClone"
	architecture "x86_64"
  startproject "MinecraftClone"

	configurations
	{
		"Debug",
		"Release",
    "Distribution"
	}

  flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/%{wks.name}/vendor/spdlog/include"
IncludeDir["stb"] = "%{wks.location}/%{wks.name}/vendor/stb"
IncludeDir["ImGui"] = "%{wks.location}/%{wks.name}/vendor/imgui"
IncludeDir["DirectXTK"] = "%{wks.location}/%{wks.name}/vendor/DirectXTK"
IncludeDir["OpenSimplexNoise"] = "%{wks.location}/%{wks.name}/vendor/OpenSimplexNoise/OpenSimplexNoise"

project "SimpleMath"
    location "%{wks.location}/%{wks.name}/vendor/DirectXTK"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

	  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	  objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{IncludeDir.DirectXTK}/Src/pch.cpp"

	  includedirs
	  {
      "%{IncludeDir.DirectXTK}/Inc",
      "%{IncludeDir.DirectXTK}/Src",
	  }
  
    files
    {
        "%{IncludeDir.DirectXTK}/Inc/SimpleMath.inl",
        "%{IncludeDir.DirectXTK}/Inc/SimpleMath.h",
        "%{IncludeDir.DirectXTK}/Src/SimpleMath.cpp",
        "%{IncludeDir.DirectXTK}/Src/pch.h",
        "%{IncludeDir.DirectXTK}/Src/pch.cpp",
    }
    
	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Distribution"
		runtime "Release"
		optimize "on"
    symbols "off"


project "OpenSimplexNoise"
	location "%{wks.location}/%{wks.name}/vendor/OpenSimplexNoise"
	kind "StaticLib"
	language "C++"
  staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{IncludeDir.OpenSimplexNoise}/OpenSimplexNoise.h",
    "%{IncludeDir.OpenSimplexNoise}/OpenSimplexNoise.cpp",
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Distribution"
		runtime "Release"
		optimize "on"
    symbols "off"


project "ImGui"
	location "%{IncludeDir.ImGui}"
	kind "StaticLib"
	language "C++"
  staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{IncludeDir.ImGui}/imconfig.h",
		"%{IncludeDir.ImGui}/imgui.h",
		"%{IncludeDir.ImGui}/imgui.cpp",
		"%{IncludeDir.ImGui}/imgui_draw.cpp",
		"%{IncludeDir.ImGui}/imgui_internal.h",
		"%{IncludeDir.ImGui}/imgui_tables.cpp",
		"%{IncludeDir.ImGui}/imgui_widgets.cpp",
		"%{IncludeDir.ImGui}/imstb_rectpack.h",
		"%{IncludeDir.ImGui}/imstb_textedit.h",
		"%{IncludeDir.ImGui}/imstb_truetype.h",
		"%{IncludeDir.ImGui}/imgui_demo.cpp",
    "%{IncludeDir.ImGui}/backends/imgui_impl_win32.cpp",
    "%{IncludeDir.ImGui}/backends/imgui_impl_win32.h",
    "%{IncludeDir.ImGui}/backends/imgui_impl_dx11.cpp",
    "%{IncludeDir.ImGui}/backends/imgui_impl_dx11.h"
	}

  includedirs
	{
    "%{IncludeDir.ImGui}"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Distribution"
		runtime "Release"
		optimize "on"
    symbols "off"


project "MinecraftClone"
	location "MinecraftClone"
	kind "ConsoleApp"
	language "C++"
  cppdialect "C++17"
  staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputdir .. "/%{prj.name}")

  pchheader "Precompiled.h"
  pchsource "%{wks.location}/%{wks.name}/src/Precompiled.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
    "%{prj.name}/src/Shaders/**.hlsl",
	}

  defines
  {
    "_CRT_SECURE_NO_WARNINGS"
  } 

	includedirs
	{
		"%{prj.name}/src",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.stb}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.DirectXTK}/Inc",
    "%{IncludeDir.OpenSimplexNoise}"
	}

  links
	{
      "d3d11.lib",
      "d3dcompiler.lib",
      "dxgi.lib",
      "dxguid.lib",
      "ImGui",
      "OpenSimplexNoise"
	}

  filter { "files:**.hlsl" }
    shadermodel "5.0"
  filter { "files:**VS.hlsl" }
    shadertype "Vertex"
  filter { "files:**PS.hlsl" }
    shadertype "Pixel"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
    runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
    runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "DIST"
    runtime "Release"
		optimize "on"