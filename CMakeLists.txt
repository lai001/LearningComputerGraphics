CMAKE_MINIMUM_REQUIRED( VERSION 3.16 )
PROJECT(LearningComputerGraphics VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

include_directories("Src")
include_directories("Src/GLRenderer")
include_directories("Src/GUI")
include_directories("Src/LightingSystem")
include_directories("Src/FileManager")
include_directories("Src/BridgingModels")

include_directories("Vendor")
include_directories("Vendor/glad/include")
include_directories("Vendor/glfw/include")
include_directories("Vendor/imgui")

aux_source_directory("Src" SRC)
aux_source_directory("Src/ThirdParty" SRC_ThirdParty)
aux_source_directory("Src/GLRenderer" SRC_GLRenderer)
aux_source_directory("Src/GUI" SRC_GUI)
aux_source_directory("Src/LightingSystem" SRC_LightingSystem)
aux_source_directory("Src/FileManager" SRC_FileManager)
aux_source_directory("Src/BridgingModels" SRC_BridgingModels)

file(GLOB HEADERS Src/*.h)
file(GLOB ThirdParty_HEADERS Src/ThirdParty/*.h)
file(GLOB GLRenderer_HEADERS Src/GLRenderer/*.h)
file(GLOB GUI_HEADERS Src/GUI/*.h)
file(GLOB LightingSystem_HEADERS Src/LightingSystem/*.h)
file(GLOB FileManager_HEADERS Src/FileManager/*.h)
file(GLOB BridgingModels_HEADERS Src/BridgingModels/*.h)
file(GLOB GLShader Src/GLRenderer/GLShaders/*.frag Src/GLRenderer/GLShaders/*.vert Src/GLRenderer/GLShaders/Library/*.glsl)

add_executable(${PROJECT_NAME} 
${FileManager_HEADERS} ${SRC} ${ThirdParty_HEADERS}
${SRC_ThirdParty} ${SRC_GLRenderer} ${SRC_FileManager}
${SRC_GUI} ${SRC_LightingSystem} 
${HEADERS} ${GLRenderer_HEADERS}
${GUI_HEADERS} ${LightingSystem_HEADERS}
${SRC_BridgingModels} ${BridgingModels_HEADERS}
${GLShader}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
${SRC} ${SRC_Vendor} ${SRC_GLRenderer}
${SRC_GUI} ${SRC_LightingSystem} ${HEADERS}
${GLRenderer_HEADERS} ${GUI_HEADERS} ${LightingSystem_HEADERS}
${ThirdParty_HEADERS} ${FileManager_HEADERS} ${SRC_FileManager}
${SRC_BridgingModels} ${BridgingModels_HEADERS}
${GLShader}
)

file(GLOB stb_image_Src Vendor/stb_image/*.cpp )
file(GLOB stb_image_Header Vendor/stb_image/*.h)
add_library(stb_image STATIC ${stb_image_Src} ${stb_image_Header})
target_link_libraries(${PROJECT_NAME} PRIVATE stb_image)

file(GLOB_RECURSE  glad_Src Vendor/glad/**.c)
file(GLOB glad_Header Vendor/glad/*.h)
add_library(glad STATIC ${glad_Src} ${glad_Header})
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(Vendor/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_include_directories(${PROJECT_NAME} PRIVATE Vendor/glfw/include)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

file(GLOB imgui_Src Vendor/imgui/*.cpp)
file(GLOB imgui_Header Vendor/imgui/*.h)
file(GLOB imgui_Src2 Vendor/imgui/examples/opengl3_example/imgui_impl_glfw_gl3.cpp)
file(GLOB imgui_Header2 Vendor/imgui/examples/opengl3_example/imgui_impl_glfw_gl3.h)
add_library(imgui STATIC ${imgui_Src} ${imgui_Src2} ${imgui_Header} ${imgui_Header2})
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

add_subdirectory(Vendor/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

add_subdirectory(Vendor/spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_include_directories(${PROJECT_NAME} PRIVATE Vendor/spdlog/include)

add_subdirectory(Vendor/assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_include_directories(${PROJECT_NAME} PRIVATE Vendor/assimp/include)
set(ASSIMP_LIBRARY_SUFFIX -vc141-mt)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${Assimp_BINARY_DIR}/code/$<CONFIG>/assimp${ASSIMP_LIBRARY_SUFFIX}$<$<CONFIG:Debug>:d>.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${spdlog_BINARY_DIR}/$<CONFIG>/spdlog$<$<CONFIG:Debug>:d>.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${GLFW_BINARY_DIR}/src/$<CONFIG>/glfw3$<$<CONFIG:Debug>:d>.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Resource ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Resource)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Src/GLRenderer/GLShaders ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Resource/GLShaders)

add_custom_target(CopyResource)
add_custom_command(TARGET CopyResource POST_BUILD  COMMAND ${CMAKE_COMMAND} -E copy ${Assimp_BINARY_DIR}/code/$<CONFIG>/assimp${ASSIMP_LIBRARY_SUFFIX}$<$<CONFIG:Debug>:d>.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
add_custom_command(TARGET CopyResource POST_BUILD  COMMAND ${CMAKE_COMMAND} -E copy ${spdlog_BINARY_DIR}/$<CONFIG>/spdlog$<$<CONFIG:Debug>:d>.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
add_custom_command(TARGET CopyResource POST_BUILD  COMMAND ${CMAKE_COMMAND} -E copy ${GLFW_BINARY_DIR}/src/$<CONFIG>/glfw3$<$<CONFIG:Debug>:d>.dll ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
add_custom_command(TARGET CopyResource POST_BUILD  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Resource ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Resource)
add_custom_command(TARGET CopyResource POST_BUILD  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Src/GLRenderer/GLShaders ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Resource/GLShaders)

message(STATUS "--------------------------------------------------")
get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()