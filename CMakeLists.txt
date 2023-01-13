cmake_minimum_required(VERSION 3.0.0)

# 项目名称
project(CMake_GCCv7.3.0 VERSION 0.1.0)
include(CTest)
enable_testing()

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER "g++")#设置c++编译器

# h 文件
include_directories(include)
# 根目录/代码路径
aux_source_directory(src SRC_MAIN)
#aux_source_directory(source/2212 SOURCE_DIR)
# 动态库文件
#add_library(cpps_lib_shared SHARED "E:\\C++\\VScode-workspace\\source\\2212\\*.cpp")
# 链接hellolib库，注意下面子目录src的CMakeLists
# link_directories("${PROJECT_SOURCE_DIR}/lib")
# target_link_libraries(hello1 hellolib)

#dubug 模式 ---- 否则无法启动 debug
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")

# 编译可执行文件
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin)
add_executable(CMake_GCCv7.3.0 ${SRC_MAIN})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# aux_source_directory(. DIR_TOOT_SRCS)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# add_executable(${PROJECT_NAME} ${DIR_TOOT_SRCS})

# 设置编码格式
# C++ 的解决办法
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK")