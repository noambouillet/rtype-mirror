cmake_minimum_required(VERSION 3.15)
project(R-Type VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

find_package(asio CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_executable(r-type_server
    src/server/main.cpp
    src/server/GameServer.cpp
    src/server/Session.cpp
    src/server/network/Protocol.cpp
    src/server/network/UdpServer.cpp
)
target_link_libraries(r-type_server PRIVATE asio::asio Threads::Threads)

# The graphical client will become a target when its source tree is added.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/client/main.cpp")
    find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
    add_executable(r-type_client src/client/main.cpp)
    target_link_libraries(r-type_client PRIVATE SFML::graphics SFML::window
        SFML::system SFML::audio SFML::network asio::asio)
endif()


