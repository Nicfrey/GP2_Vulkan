# Source files
add_custom_target(copy_resources ALL)
add_custom_command(
    TARGET copy_resources PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

add_custom_target(copy_textures ALL)
add_custom_command(
    TARGET copy_textures PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/textures
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures)


set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${SHADER_SOURCE_DIR}/*.frag"
    "${SHADER_SOURCE_DIR}/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)

set(SOURCES main.cpp 
    VulkanProject.cpp
    Helper/Vertex.h
    Helper/VertexBuffer.h
    Helper/VertexBuffer.cpp
    Helper/Mesh.h
    Helper/Mesh.cpp
    Helper/RectangleMesh2D.h
    Helper/RectangleMesh2D.cpp
    Helper/CircleMesh2D.h
    Helper/CircleMesh2D.cpp
    Helper/Scene.h
    Helper/Scene.cpp
    Helper/Camera.h
    Helper/Camera.cpp
    Helper/Pipeline.h
    Helper/Pipeline.cpp
    Helper/MathHelpers.h
    Helper/Mesh2D.h
    Helper/Mesh2D.cpp
    Helper/Mesh3D.h
    Helper/Mesh3D.cpp
    Helper/TextureImage.h
    Helper/TextureImage.cpp
    Helper/Helper.h
    Helper/Descriptor.h
    Helper/Descriptor.cpp
    Helper/Shader.h
    Helper/Shader.cpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${GLSL_SOURCE_FILES} )
add_dependencies(${PROJECT_NAME} Shaders)
# Link libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${stb_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
