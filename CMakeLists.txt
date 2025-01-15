cmake_minimum_required(VERSION 3.30)

project(KLay)

add_library(
	KLay
	src/Klay.cpp
	src/ToString.cpp
	include/klay/Klay.hpp
	include/klay/Geometry.hpp
)

set_target_properties(
	KLay
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

target_include_directories(KLay PUBLIC include)

add_subdirectory(test)
add_subdirectory(unit)