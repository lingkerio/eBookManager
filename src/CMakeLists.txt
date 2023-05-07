# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.25)

# 项目信息
project (toy)

set(CMAKE_TOOLCHAIN_FILE D:\\Scoop\\apps\\vcpkg\\current\\scripts\\buildsystems\\vcpkg.cmake)



find_package(nlohmann_json REQUIRED)
find_package(unofficial-curlpp CONFIG REQUIRED)
find_package(oatpp CONFIG REQUIRED)

# 指定生成目标
add_executable(toy main.cpp Book.cpp Manage.cpp Server.cpp Download.cpp)

# 链接nlohmann-json库
target_link_libraries(toy PRIVATE nlohmann_json::nlohmann_json) #nlohmann 为 header only library，因此这一行链接代码并没有实际意义

# 链接curl++库
target_link_libraries(toy PRIVATE unofficial::curlpp::curlpp)

# 链接oatpp库
target_link_libraries(toy PRIVATE oatpp::oatpp oatpp::oatpp-test)