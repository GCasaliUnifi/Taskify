cmake_minimum_required(VERSION 3.28)
project(Taskify)

set(CMAKE_CXX_STANDARD 20)

find_package(wxWidgets REQUIRED COMPONENTS core base xml)
include(${wxWidgets_USE_FILE})

add_executable(Taskify src/main.cpp)

add_library(core
        src/MainFrame.cpp src/MainFrame.h
        src/TaskPanel.cpp src/TaskPanel.h
        src/XMLParser.h
)

target_link_libraries(Taskify PRIVATE core ${wxWidgets_LIBRARIES})

add_subdirectory(test)

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)