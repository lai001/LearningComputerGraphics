set_xmakever("2.6.3")
includes("../Foundation/Foundation")

add_requires("glm")
add_requires("glfw")
add_requires("spdlog")
add_requires("assimp", {configs = {shared = true}})

add_requires("openimageio")

rule("LearningComputerGraphics.deps")
    on_load(function (target)
        import("net.http")
        import("devel.git")
        import("utils.archive")
        if os.exists("Vendor/imgui") == false then
            local oldir = os.cd("Vendor")
            git.clone("https://github.com/ocornut/imgui.git", {branch = "master", outputdir = "imgui"})
            git.checkout("v1.87", {repodir = "imgui"})
            os.cd(oldir)
        end
        if os.exists("../Foundation") == false then
            git.clone("https://github.com/lai001/Foundation.git", {branch = "main", outputdir = "../Foundation"})
        end
        if os.exists("../Vendor/DiligentEngine") == false then
            local oldir = os.cd("../Vendor")
            os.exec("git clone --recursive https://github.com/DiligentGraphics/DiligentEngine.git")
            os.cd(oldir)
        end
        if os.exists("./Vendor/DiligentEngine") == false then
            local oldir = os.cd("../Vendor/DiligentEngine")
            os.exec("cmake -S . -B ./build/Win64 -DENGINE_DLL=YES -DBUILD_TYPE=Debug -D CMAKE_INSTALL_PREFIX=../../LearningComputerGraphics/Vendor/DiligentEngine -D DILIGENT_NO_FORMAT_VALIDATION=YES -A x64")
            os.exec("cmake --build ./build/Win64 --target INSTALL --config Debug --clean-first")
            os.exec("cmake -S . -B ./build/Win64 -DENGINE_DLL=YES -DBUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=../../LearningComputerGraphics/Vendor/DiligentEngine -D DILIGENT_NO_FORMAT_VALIDATION=YES -A x64")
            os.exec("cmake --build ./build/Win64 --target INSTALL --config Release --clean-first")
            os.cd(oldir)
        end
        target:add("deps", "Foundation")
    end)

rule("LearningComputerGraphics.Copy")
    after_build(function (target)
        local resourceDir = path.join(target:targetdir(), "Resource")
        if os.exists(resourceDir) then
            os.rm(resourceDir)
        end
        os.cp("Resource", target:targetdir())
        os.cp("Vendor/DiligentEngine/bin/DiligentCore/Release/*.dll", target:targetdir())
        os.cp("Vendor/DiligentEngine/bin/DiligentCore/Debug/*.dll", target:targetdir())
        local assimp_pkg = target:pkgs()["assimp"]
        if assimp_pkg then
            local libfiles = assimp_pkg:get("libfiles")
            for index, file in pairs(libfiles) do
                if path.extension(file) == ".dll" then
                    os.cp(file, path.join(target:targetdir(), path.filename(file)))
                end
            end        
        end
    end)

target("LearningComputerGraphics")
    set_kind("binary")
    set_languages("c++17")

    add_headerfiles("Resource/**.*")
    add_headerfiles("Resource/**.*")

    add_files("Src/main.cpp")
    add_files("Src/Scene.cpp")
    add_files("Src/BridgingModels/*.cpp")
    add_files("Src/GUI/*.cpp")
    add_files("Src/GUI/Main/*.cpp")
    add_files("Src/Object/*.cpp")
    add_files("Src/Util/*.cpp")
    add_files("Src/ImageIO/*.cpp")
    add_files("Src/Renderer/*.cpp")

    add_includedirs("Src")
    add_includedirs("Src/BridgingModels")
    add_includedirs("Src/GUI")
    add_includedirs("Src/GUI/Main")
    add_includedirs("Src/Interface")
    add_includedirs("Src/Object")
    add_includedirs("Src/Renderer")
    add_includedirs("Src/Util")
    add_includedirs("Src/ImageIO")

    add_headerfiles("Src/BridgingModels/*.hpp")
    add_headerfiles("Src/GUI/*.hpp")
    add_headerfiles("Src/GUI/Main/*.hpp")
    add_headerfiles("Src/Interface/*.hpp")
    add_headerfiles("Src/Object/*.hpp")
    add_headerfiles("Src/Renderer/*.hpp")
    add_headerfiles("Src/Scene.hpp")
    add_headerfiles("Src/Util/*.hpp")
    add_headerfiles("Src/ImageIO/*.hpp")

    add_rules("mode.debug", "mode.release")
    add_rules("LearningComputerGraphics.Copy")
    add_rules("LearningComputerGraphics.deps")

    add_packages("glm")
    add_packages("spdlog")
    add_packages("glfw")
    add_packages("assimp")
    add_packages("openimageio")

    add_deps("Foundation")
    add_deps("ImGui")

    local suffix = ""
    local suffix1 = ""
    local mode = ""
    if is_mode("debug") then
        suffix = "d"
        suffix1 = "d"
        mode = "Debug"
    else 
        suffix = "r"
        suffix1 = ""
        mode = "Release"
    end
    add_links("dxgi") 
    add_links("d3d11") 
    add_links("d3d12")
    add_links("d3dcompiler") 
    add_links("opengl32")
    add_links("DiligentCore") 
    add_links("glslang" .. suffix1) 
    add_links("HLSL" .. suffix1) 
    add_links("OGLCompiler" .. suffix1)
    add_links("OSDependent" .. suffix1) 
    add_links("GenericCodeGen" .. suffix1) 
    add_links("MachineIndependent" .. suffix1) 
    add_links("Archiver_64" .. suffix)
    add_links("GraphicsEngineD3D11_64" .. suffix)
    add_links("GraphicsEngineD3D12_64" .. suffix)
    add_links("GraphicsEngineOpenGL_64" .. suffix)
    add_links("GraphicsEngineVk_64" .. suffix)

    add_linkdirs("Vendor/DiligentEngine/lib/DiligentCore/" .. mode)
    add_includedirs("Vendor/DiligentEngine/include")
    add_headerfiles("Vendor/DiligentEngine/include/DiligentCore/**.hpp")

    add_defines("GLM_FORCE_LEFT_HANDED=1")
    add_defines("DILIGENT_DEBUG=1")
    add_defines("PLATFORM_WIN32=1")
    add_defines("ENGINE_DLL=1")
    add_defines("D3D11_SUPPORTED=1")
    add_defines("D3D12_SUPPORTED=1")
    add_defines("VULKAN_SUPPORTED=1")
    add_defines("GL_SUPPORTED=1")
    add_defines("NOMINMAX=1")
    add_defines("GLFW_EXPOSE_NATIVE_WIN32=1")
    add_defines("GLM_FORCE_XYZW_ONLY=1")

target("ImGui")
    set_kind("static")
    set_languages("c++17")
    add_rules("mode.debug", "mode.release")
    add_includedirs("Vendor/imgui", { public = true })
    add_headerfiles("Vendor/imgui/*.h")
    add_headerfiles("Vendor/imgui/backends/imgui_impl_glfw.h")
    add_headerfiles("Vendor/imgui/backends/imgui_impl_win32.h")
    add_files("Vendor/imgui/*.cpp")
    add_files("Vendor/imgui/backends/imgui_impl_glfw.cpp")
    add_files("Vendor/imgui/backends/imgui_impl_win32.cpp")
    add_packages("glfw")

    add_includedirs("../Vendor/DiligentEngine/DiligentTools/Imgui/interface", { public = true })
    add_includedirs("../Vendor/DiligentEngine/DiligentCore/Graphics/GraphicsEngine/interface")
    add_includedirs("../Vendor/DiligentEngine/DiligentCore/Graphics/GraphicsTools/interface")
    add_includedirs("../Vendor/DiligentEngine/DiligentCore/Platforms/Basic/interface")
    add_includedirs("../Vendor/DiligentEngine/DiligentCore/Platforms/Win32/interface")
    add_includedirs("../Vendor/DiligentEngine/DiligentCore/Primitives/interface")
    add_includedirs("../Vendor/DiligentEngine/DiligentCore/Common/interface")

    add_headerfiles("../Vendor/DiligentEngine/DiligentTools/Imgui/interface/ImGuiImplWin32.hpp")
    add_headerfiles("../Vendor/DiligentEngine/DiligentTools/Imgui/interface/ImGuiDiligentRenderer.hpp")
    add_headerfiles("../Vendor/DiligentEngine/DiligentTools/Imgui/interface/ImGuiImplDiligent.hpp")
    add_headerfiles("../Vendor/DiligentEngine/DiligentTools/Imgui/interface/ImGuiUtils.hpp")

    add_files("../Vendor/DiligentEngine/DiligentTools/Imgui/src/ImGuiImplWin32.cpp")
    add_files("../Vendor/DiligentEngine/DiligentTools/Imgui/src/ImGuiDiligentRenderer.cpp")
    add_files("../Vendor/DiligentEngine/DiligentTools/Imgui/src/ImGuiImplDiligent.cpp")
    add_files("../Vendor/DiligentEngine/DiligentTools/Imgui/src/ImGuiUtils.cpp")

    add_defines("PLATFORM_WIN32=1")
    add_defines("ENGINE_DLL=1")
    add_defines("D3D11_SUPPORTED=1")
    add_defines("D3D12_SUPPORTED=1")
    add_defines("VULKAN_SUPPORTED=1")
    add_defines("GL_SUPPORTED=1")
    add_defines("NOMINMAX=1")

target("Foundation")