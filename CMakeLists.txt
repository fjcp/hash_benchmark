cmake_minimum_required(VERSION 3.2.1)
project(test_hash_modelling)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
  SET(BOOST_ROOT "c:/workcopy/extsw/windows_amd64/boost_1_71_0")
endif()

find_package(Boost)

include_directories(SYSTEM
  ${Boost_INCLUDE_DIRS}
  )

file(GLOB SOURCES
  "*.[ch]*"
)

add_executable(test_hash_modelling ${SOURCES})

