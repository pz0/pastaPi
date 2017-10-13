cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(Puttanesca VERSION ${Spaghetti_VERSION} LANGUAGES C CXX)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(PUTTANESCA_SOURCES main.cc)

add_executable(Puttanesca ${PUTTANESCA_SOURCES})
target_link_libraries(Puttanesca Spaghetti)
