cmake_minimum_required(VERSION 3.30)

project(KLay)

add_library(
	KLay
	include/klay/ToString.hpp src/ToString.cpp
	include/klay/Klay.hpp src/Klay.cpp
	include/klay/Flex.hpp src/Flex.cpp
	include/klay/Unit.hpp
	include/klay/Geometry.hpp
	include/klay/Element.hpp src/Element.cpp
	include/klay/ElementBuilder.hpp
	include/klay/Layout.hpp src/Layout.cpp
	include/klay/Grid.hpp src/Grid.cpp
)

set_target_properties(
	KLay
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
	set_target_properties(
		KLay
		PROPERTIES
		EXPORT_COMPILE_COMMANDS ON
	)
endif()

include(FetchContent)

FetchContent_Declare(
	KInd
	GIT_REPOSITORY https://github.com/ketexon/kind
	GIT_TAG main
)

FetchContent_MakeAvailable(KInd)

target_include_directories(KLay PUBLIC include)
target_link_libraries(KLay PUBLIC KInd)

add_subdirectory(test)
add_subdirectory(unit)
