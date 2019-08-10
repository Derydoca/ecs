workspace "ECS"
    architecture "x64"
    startproject "ECS"

    configurations
    {
        "Debug"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ECS"
    location "ECS"
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
        defines "ECS_DEBUG"
        symbols "On"
        runtime "Debug"

project "ECS.Test"
    location "ECS.Test"
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

    includedirs { vendorincludes }
    includedirs
    {
        "%{wks.location}/ECS/src",
        "%{prj.location}/src",
        "%{wks.location}/vendor/gtest/googletest/include"
    }

    links
    {
        "ECS",
        "GoogleTest"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING",
            "_HAS_TR1_NAMESPACE"
        }
        
    filter "configurations:Debug"
        defines "ECS_DEBUG"
        symbols "On"
        staticruntime "Off"
        runtime "Debug"

project "GoogleTest"
    location "vendor/GeneratedProjects"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    flags
    {
        "MultiProcessorCompile"
    }

    files
    {
        "vendor/gtest/googletest/src/gtest-all.cc"
    }

    includedirs
    {
        "%{wks.location}/vendor/gtest/googletest",
        "%{wks.location}/vendor/gtest/googletest/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"
        staticruntime "Off"
        runtime "Debug"
