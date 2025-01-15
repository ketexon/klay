FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	GIT_SHALLOW 1
)
FetchContent_MakeAvailable(raylib)
FetchContent_Declare(
	raylib_cpp
	GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
)
FetchContent_MakeAvailable(raylib_cpp)

add_executable(
	KlayUnit
	Main.cpp
)

target_link_libraries(
	KlayUnit
	PRIVATE
	raylib
	raylib_cpp
)

set_target_properties(
	KlayUnit
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)