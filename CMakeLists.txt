cmake_minimum_required(VERSION 3.24)
project(triangle-motions LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ── GLFW 3.4 ──────────────────────────────────────────────────────────────────
FetchContent_Declare(
    glfw
    GIT_REPOSITORY  https://github.com/glfw/glfw.git
    GIT_TAG         3.4
    GIT_SHALLOW     TRUE
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# ── GLAD 2 (OpenGL 3.3 core) ──────────────────────────────────────────────────
FetchContent_Declare(
    glad_cmake
    GIT_REPOSITORY  https://github.com/Dav1dde/glad.git
    GIT_TAG         v2.0.8
    GIT_SHALLOW     TRUE
    SOURCE_SUBDIR   cmake
)
FetchContent_MakeAvailable(glad_cmake)
glad_add_library(glad_gl STATIC REPRODUCIBLE API gl:core=3.3)

# ── Executable ────────────────────────────────────────────────────────────────
add_executable(triangle-motions main.cpp)

target_include_directories(triangle-motions PRIVATE include)
target_link_libraries(triangle-motions PRIVATE glfw glad_gl)

# macOS: suppress the deprecated OpenGL warning introduced in 10.14
if(APPLE)
    target_compile_definitions(triangle-motions PRIVATE GL_SILENCE_DEPRECATION)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(triangle-motions PRIVATE ${OPENGL_LIB})
endif()

# Copy shader files next to the executable so they are found at runtime
add_custom_command(TARGET triangle-motions POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/utils"
            "$<TARGET_FILE_DIR:triangle-motions>/utils"
    COMMENT "Copying shaders to build output directory"
)
