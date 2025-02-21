FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	GIT_SHALLOW 1
	GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)
FetchContent_Declare(
	raylib_cpp
	GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
	GIT_TAG v5.5.0
)
FetchContent_MakeAvailable(raylib_cpp)

add_executable(
	KLayUnit
	Unit.hpp
	Main.cpp
	Flex.cpp
)

target_link_libraries(
	KLayUnit
	PRIVATE
	raylib
	raylib_cpp
	KLay
)

set_target_properties(
	KLayUnit
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	EXPORT_COMPILE_COMMANDS ON
)
