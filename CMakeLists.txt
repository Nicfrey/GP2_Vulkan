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
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        v1.90.6
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

find_package(Vulkan REQUIRED)
find_package(Doxygen REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(VulkanProject)
