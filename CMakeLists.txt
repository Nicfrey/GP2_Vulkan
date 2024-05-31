cmake_minimum_required(VERSION 3.10.0)
project(Vulkan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8 # Specify the version you need
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.4.zip
)

FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e 
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG master
)

FetchContent_MakeAvailable(stb)

FetchContent_Declare(
	tinyobjloader
	GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
	GIT_TAG v1.0.6
)

FetchContent_MakeAvailable(tinyobjloader)

FetchContent_Declare(
	json
	URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(json)

find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(VulkanProject)
