cmake_minimum_required(VERSION 3.28)
project(Taskify)

set(CMAKE_CXX_STANDARD 20)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(Taskify main.cpp
        MainFrame.h
        MainFrame.cpp
        TaskPanel.cpp
        TaskPanel.h)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)