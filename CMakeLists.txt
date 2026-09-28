cmake_minimum_required(VERSION 3.15)
project(R-Type VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # Errors and warnings for MSVC (Windows)
    add_compile_options(/W4 /WX)
else()
    # Configuration for GCC or Clang (Linux / Mac)
    add_compile_options(-Wall -Wextra -Werror)
endif()

# We use Vcpkg in Manifest Mode to manage dependencies. The toolchain file is automatically detected by CMake when using the `CMAKE_TOOLCHAIN_FILE` environment variable.
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(r-type_server
    server/src/main.cpp
    # Add the other source files for the server here
)
target_link_libraries(r-type_server PRIVATE asio)

add_executable(r-type_client
    client/src/main.cpp
    # Add the other source files for the client here
)

# The client needs SFML for rendering, audio, and potentially Asio for networking
target_link_libraries(r-type_client PRIVATE 
    SFML::graphics 
    SFML::window 
    SFML::system 
    SFML::audio 
    SFML::network 
    asio
)