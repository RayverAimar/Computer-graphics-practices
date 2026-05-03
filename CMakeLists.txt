cmake_minimum_required(VERSION 3.24)
project(cg-practices LANGUAGES CXX C)

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

# ── GLM (header-only math) ────────────────────────────────────────────────────
FetchContent_Declare(
    glm
    GIT_REPOSITORY  https://github.com/g-truc/glm.git
    GIT_TAG         1.0.1
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(glm)

# ── Common helper macro ───────────────────────────────────────────────────────
macro(cg_executable TARGET SOURCE)
    add_executable(${TARGET} ${SOURCE})
    target_include_directories(${TARGET} PRIVATE include)
    target_link_libraries(${TARGET} PRIVATE glfw glad_gl glm::glm)
    if(APPLE)
        target_compile_definitions(${TARGET} PRIVATE GL_SILENCE_DEPRECATION)
        find_library(OPENGL_LIB OpenGL REQUIRED)
        target_link_libraries(${TARGET} PRIVATE ${OPENGL_LIB})
    endif()
    # Copy all shaders next to the executable at build time
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/utils"
                "$<TARGET_FILE_DIR:${TARGET}>/utils"
        COMMENT "Copying shaders for ${TARGET}"
    )
endmacro()

# ── Exercise 1: triangle motions ──────────────────────────────────────────────
cg_executable(triangle-motions   main.cpp)

# ── Exercise 2: GLM transformations ───────────────────────────────────────────
cg_executable(house-transforms   transformations.cpp)
