# ---- Options ----
option(COPY_BUILD "Copy the build output to the Fallout directory." TRUE)
option(BUILD_FALLOUTVR "Build for Fallout VR" OFF)

macro(find_commonlib_path)
	if (CommonLibName AND NOT ${CommonLibName} STREQUAL "")
		# Check extern
		find_path(CommonLibPath
		include/REL/Relocation.h
		PATHS ${ROOT_DIR}/external/${CommonLibName}/${CommonLibName}/)
		if (${CommonLibPath} STREQUAL "CommonLibPath-NOTFOUND")
			#Check path
			set_from_environment(${CommonLibName}Path)
			set(CommonLibPath ${${CommonLibName}Path})
		endif()
	endif()
endmacro()
set(CommonLibName "CommonLibF4")
set_root_directory()

if(BUILD_FALLOUTVR)
	add_compile_definitions(FALLOUTVR)
	set_from_environment(FalloutVRPath)
	set(FalloutPath ${FalloutVRPath})
	set(FalloutVersion "Fallout VR")
else()
	set_from_environment(Fallout4Path)
	set(FalloutPath ${Fallout4Path})
	set(FalloutVersion "Fallout 4")
endif()
find_commonlib_path()
message(
	STATUS
	"Building ${PROJECT_NAME} ${PROJECT_VERSION} for ${FalloutVersion} at ${FalloutPath} with ${CommonLibName} at ${CommonLibPath}."
)

list(APPEND CMAKE_MODULE_PATH "${ROOT_DIR}/cmake")

set(SOURCE_DIR "${ROOT_DIR}/src")
set(SOURCE_FILES
	"${SOURCE_DIR}/dirent.h"
	"${SOURCE_DIR}/main.cpp"
	"${SOURCE_DIR}/Utility.h"
	"${SOURCE_DIR}/Menu.h"
	"${SOURCE_DIR}/Menu.cpp"
	"${SOURCE_DIR}/VRFpsStabilizer.h"
	"${SOURCE_DIR}/VRFpsStabilizer.cpp"
	"${SOURCE_DIR}/PCH.h"
)

source_group(TREE "${ROOT_DIR}" FILES ${SOURCE_FILES})

set(VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/Plugin.h")
configure_file(
	"${ROOT_DIR}/cmake/Plugin.h.in"
	"${VERSION_HEADER}"
	@ONLY
)

source_group("src" FILES "${VERSION_HEADER}")

configure_file(
	"${ROOT_DIR}/cmake/version.rc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	@ONLY
)

add_library(
	"${PROJECT_NAME}"
	SHARED
	${SOURCE_FILES}
	"${VERSION_HEADER}"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	"${ROOT_DIR}/.clang-format"
	#"${ROOT_DIR}/.editorconfig"
)

target_compile_features(
	"${PROJECT_NAME}"
	PRIVATE
		cxx_std_23
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(
		"${PROJECT_NAME}"
		PRIVATE
			"/EHa"	# Exception handling model
			"/sdl"	# Enable Additional Security Checks
			"/utf-8"	# Set Source and Executable character sets to UTF-8
			"/Zi"	# Debug Information Format

			"/permissive-"	# Standards conformance
			"/Zc:preprocessor"	# Enable preprocessor conformance mode

			"/wd4324" # 'struct_name' : structure was padded due to __declspec(align())

			"$<$<CONFIG:DEBUG>:>"
			"$<$<CONFIG:RELEASE>:/Zc:inline;/JMC-;/Ob3>"
	)

	target_link_options(
		"${PROJECT_NAME}"
		PRIVATE
			"$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOREF;/OPT:NOICF>"
			"$<$<CONFIG:RELEASE>:/INCREMENTAL:NO;/OPT:REF;/OPT:ICF;/DEBUG:FULL;/LTCG>"
	)
endif()

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_BINARY_DIR}/src"
		"${SOURCE_DIR}"
)

if (DEFINED CommonLibPath AND NOT ${CommonLibPath} STREQUAL "" AND IS_DIRECTORY ${CommonLibPath})
	add_subdirectory(${CommonLibPath} ${CommonLibName})
else ()
	message(
		FATAL_ERROR
		"Variable ${CommonLibName}Path is not set or in external/."
	)
endif()

find_package(xbyak REQUIRED CONFIG)
find_package(OpenVR REQUIRED)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS) # silence warnings for mbstowcs

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		CommonLibF4::CommonLibF4
		xbyak::xbyak
		OpenVR::OpenVR
)

target_compile_definitions(
	"${PROJECT_NAME}"
	PRIVATE
)

target_compile_definitions(
	CommonLibF4
	PUBLIC
		F4SE_SUPPORT_XBYAK
)

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		"${SOURCE_DIR}/PCH.h"
)

handle_data_files(
	FILES
		"$<TARGET_FILE:${PROJECT_NAME}>"
#		"${ROOT_DIR}/data/Data/F4SE/Plugins/VRFPSStabilizerFallout.toml"
	DESTINATION "F4SE/Plugins"
)

install(
	FILES
		"$<TARGET_PDB_FILE:${PROJECT_NAME}>"
	DESTINATION "/"
	COMPONENT "pdbs"
)

if (COPY_BUILD)
	if (DEFINED FalloutPath)
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${FalloutPath}/Data/f4se/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${FalloutPath}/Data/f4se/Plugins/
		)
	else ()
		message(
			WARNING
			"Variable ${FalloutPath} is not defined. Skipping post-build copy command."
		)
	endif ()
endif ()
