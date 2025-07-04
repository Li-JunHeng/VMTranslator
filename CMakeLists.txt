cmake_minimum_required(VERSION 3.31)
project(VMTranslator VERSION 1.0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

# 源文件列表
set(VM_SRCS
    main.cpp
    Command/Parser.cpp
    Command/Parser.h
    Command/CodeWriter.cpp
    Command/CodeWriter.h
    Helper/Usage.cpp
    Helper/Usage.h
    IO_Helper/InputHelper.cpp
    IO_Helper/InputHelper.h
    IO_Helper/OutputHelper.cpp
    IO_Helper/OutputHelper.h
    IO_Helper/Helper.cpp
    IO_Helper/Helper.h
    Logger/ErrorLogger.cpp
    Logger/ErrorLogger.h
    Logger/Logger.h
)

add_executable(VMTranslator ${VM_SRCS})

target_compile_definitions(VMTranslator
    PRIVATE
        PROJECT_VERSION="${PROJECT_VERSION}"
)

target_compile_options(VMTranslator
    PRIVATE
        $<$<CONFIG:Release>:-O3>
)

target_link_options(VMTranslator
    PRIVATE
        $<$<CONFIG:Release>:-s>
)