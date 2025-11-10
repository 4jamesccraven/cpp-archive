cmake_minimum_required(VERSION 3.16)
project(cpp_archive LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Link compile_commands.json to the root dir for language servers
if(UNIX)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_custom_target(link_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
                ${CMAKE_BINARY_DIR}/compile_commands.json
                ${CMAKE_SOURCE_DIR}/compile_commands.json
        DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    )
endif()

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(proj ${PROJECT_SOURCES})

target_include_directories(proj PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
