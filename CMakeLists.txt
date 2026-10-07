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
find_package(SFML 3.0.2 EXACT COMPONENTS Graphics Window System CONFIG REQUIRED)

add_library(rtype_protocol src/utils/network/Protocol.cpp)
target_include_directories(rtype_protocol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(rtype_logger src/utils/Logger.cpp)
target_include_directories(rtype_logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(rtype_engine
    src/ECS/Component/ComponentManager.cpp
    src/ECS/Coordinator/Coordinator.cpp
    src/ECS/Entity/Entity.cpp
    src/ECS/Entity/EntityManager.cpp
    src/ECS/System/SystemManager.cpp
)
target_include_directories(rtype_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ECS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ECS/Component
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ECS/Coordinator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ECS/Entity
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ECS/System
)

add_executable(r-type_server
    src/server/main.cpp
    src/server/GameServer.cpp
    src/server/Session.cpp
    src/server/network/UdpServer.cpp
)
target_link_libraries(r-type_server PRIVATE rtype_protocol rtype_logger rtype_engine asio::asio Threads::Threads)
if(WIN32)
    target_link_libraries(r-type_server PRIVATE bcrypt)
endif()

add_executable(r-type_client
    src/client/main.cpp
    src/client/app/ClientApp.cpp
    src/client/app/ClientOptions.cpp
    src/client/SFML/AssetManager.cpp
    src/client/SFML/Sfml.cpp
    src/client/network/GameClient.cpp
    src/client/network/UdpClient.cpp
)
target_link_libraries(r-type_client PRIVATE rtype_protocol rtype_logger
    asio::asio Threads::Threads SFML::Graphics SFML::Window SFML::System)

add_custom_target(rtype_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:r-type_client>/assets"
    DEPENDS r-type_client)
