cmake_minimum_required(VERSION 3.14)

project(Server LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_SQL_DRIVERS_PATH "C:/Qt/Tools/QtCreator/bin/plugins/sqldrivers")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS Sql REQUIRED)

add_executable(Server
  main.cpp
  server.h
  server.cpp
  clienthandler.h
  clienthandler.cpp
  connect_to_data_base.h
  connect_to_data_base.cpp
)

target_link_libraries(Server PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Server PRIVATE Qt6::Network)
target_link_libraries(Server PRIVATE Qt6::Sql)

install(TARGETS Server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
