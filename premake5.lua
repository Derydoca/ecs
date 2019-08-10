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
    kind "StaticLib"
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

    includedirs
    {
        "%{wks.location}/ECS/src",
        "%{prj.location}/src",
        "%{wks.location}/Vendor/GoogleTest/googletest/include"
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
            "_HAS_TR1_NAMESPACE",
            "_CRT_SECURE_NO_WARNINGS",
            "_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
        }
        
    filter "configurations:Debug"
        defines "ECS_DEBUG"
        symbols "On"
        staticruntime "Off"
        runtime "Debug"

project "GoogleTest"
    location "Vendor/GeneratedProjects"
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
        "Vendor/GoogleTest/googletest/src/gtest-all.cc"
    }

    includedirs
    {
        "%{wks.location}/Vendor/GoogleTest/googletest",
        "%{wks.location}/Vendor/GoogleTest/googletest/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"
        staticruntime "Off"
        runtime "Debug"
