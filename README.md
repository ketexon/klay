# Klay

Ketexon's Layout Engine for C++20. 

This is a customizable layout engine designed to mimic web layout modes in C++ for use in games.

Right now, it's in early development, with 2 layout modes: flex and grid.

## Usage

See the `unit` folder for an example.

## Building Tests

```bash
cmake -B build && cmake --build build
./build/unit/KLayTest
./build/unit/KLayUnit
```

## Using with CMake

To use with FetchContent:

```cmake
include(FetchContent)
FetchContent_Declare(
	KLay
	GIT_REPOSITORY https://github.com/ketexon/klay
	GIT_TAG main
)

FetchContent_MakeAvailable(KLay)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  KLay
)
```

Otherwise, you can use `add_subdirectory`:

```cmake
# make sure ./klay/CMakeLists.txt exists
add_subdirectory(klay)
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  KLay
)
```

## Current features

- Minimum size
- Flex layout
  - Justify content and align content/self options

## Todo

- [x] Padding, margin, border
- [ ] Layout abstraction
- [ ] Mark dirty
- [ ] User data
- [ ] Grid
  - [ ] Dense packing
  - [ ] Non-dense packing
  - [ ] Implicit grid sizing
- [ ] Custom layouts
- [ ] Absolute positioning
- [ ] De/serialization of layouts
