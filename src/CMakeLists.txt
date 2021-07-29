# add compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
      -Wall -Wextra -pedantic -pedantic-errors -Wconversion -Wsign-conversion
    >
    $<$<CXX_COMPILER_ID:MSVC>:
      /W4
    >
)
endif()

add_subdirectory(computationalMesh)
add_subdirectory(parameterFileReading)
add_subdirectory(utilities)
