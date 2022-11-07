cmake_minimum_required(VERSION 3.23)

project(
	VRFPSStabilizerFallout
	VERSION 1.0.1
	LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "in-source builds are not allowed")
endif()

include(cmake/common.cmake)

add_subdirectory(src)
include(cmake/packaging.cmake)
