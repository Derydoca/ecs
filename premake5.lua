workspace "ecs"
    architecture "x64"
    startproject "ecs"

    configurations
    {
        "Debug"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ecs"
    location "ecs"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{prj.location}/src/pch.cpp"

    flags
    {
        "MultiProcessorCompile"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.location}/src",
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

    filter "configurations:Debug"
        staticruntime "Off"
        defines "DD_DEBUG"
        symbols "On"
        runtime "Debug"
