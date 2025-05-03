cmake_minimum_required(VERSION 3.28)
project(Taskify)

set(CMAKE_CXX_STANDARD 20)

find_package(wxWidgets REQUIRED COMPONENTS core base xml)
include(${wxWidgets_USE_FILE})

add_executable(Taskify src/main.cpp)

add_library(core
        src/View/MainFrame.cpp src/View/MainFrame.h
        src/View/TaskPanel.cpp src/View/TaskPanel.h
        src/Model/XMLParser.h
        src/View/ThemeManager.cpp
        src/View/ThemeManager.h
        src/Model/XMLParser.cpp
        src/Model/Task.cpp
        src/Model/Task.h
        src/Controller/Controller.cpp
        src/Controller/Controller.h
)

target_link_libraries(Taskify PRIVATE core ${wxWidgets_LIBRARIES})

add_subdirectory(test)

add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)