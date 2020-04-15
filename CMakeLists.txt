cmake_minimum_required(VERSION 3.9)

set(VCPKG_DEFAULT_TRIPLET "x86-windows-static")
set(CMAKE_TOOLCHAIN_FILE "./vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x86-windows-static")

project(mybot-demo) # 填写项目名称

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准版本为 C++17, 必须

include(extern/cqcppsdk/cqcppsdk.cmake) # 包含 SDK 的 CMake 脚本, 必须

include_directories(extern/cqcppsdk/include) # 设置 src 目录为包含目录, 可根据实际情况修改

cq_set_app_id("io.github.qijieh.saucenaosoutu") # 设置 app id, 必须

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp src/*.h)


if (CQ_CAN_BUILD_STD_MODE)

    find_package(nlohmann_json CONFIG REQUIRED)
    find_package(ZLIB REQUIRED)
    find_package(libpng CONFIG REQUIRED)
    find_package(CURL CONFIG REQUIRED)
    find_package(unofficial-nana CONFIG REQUIRED)
    cq_add_std_app(${SOURCE_FILES}) # 添加 std 模式的动态链接库构建目标
    #历史遗留原因 勿设置 PUBLIC/PRIVATE
    target_link_libraries(app CURL::libcurl)
    target_link_libraries(app png_static)
    target_link_libraries(app ZLIB::ZLIB)
    target_link_libraries(app nlohmann_json nlohmann_json::nlohmann_json)
    target_link_libraries(app unofficial::nana::nana)

    cq_install_std_app("D:/QQBot/CoolQ")

    
endif ()
