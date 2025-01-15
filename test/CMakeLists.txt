include(FetchContent)

FetchContent_Declare(
	KTest
	GIT_REPOSITORY https://github.com/ketexon/KTest
	GIT_TAG 7236f6c68adfaf9886cf8acab209452df20c7d76
)

FetchContent_MakeAvailable(KTest)

add_executable(
	KLayTest
	Layout.cpp
)

set_target_properties(
	KLayTest
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

target_link_libraries(
	KLayTest
	KLay
	KTestWithMain
)