cmake_minimum_required(VERSION 3.4)
project(rubiks)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

# Configuration
message("Configuration")
set(THIRD_PARTY_LIBRARIES "${CMAKE_SOURCE_DIR}/libs" CACHE FILEPATH "Third party libraries directory")
set(ENABLE_TEST ON CACHE BOOL "Enable testing for this build")
set(TEST_OUTPUT ${CMAKE_BINARY_DIR}/tests CACHE FILEPATH "Output directory of tests")
set(TEST_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/test/resources CACHE FILEPATH "Resource folder for tests")
set(COVERAGE OFF CACHE BOOL "Enables coverage configuration")
set(PROFILING OFF CACHE BOOL "Enables profiling configuration")
add_definitions(-DTEST_RESOURCE_DIR="${TEST_RESOURCE_DIR}")
add_definitions(-DBOOST_LOG_DYN_LINK=1)
add_definitions(-DDEFAULT_CONFIG_FILE="${CMAKE_SOURCE_DIR}/config/vision.json")
message("-- Libraries at ${THIRD_PARTY_LIBRARIES}")
if (${ENABLE_TEST})
	message("-- Tests enabled")
	message("--   Tests output: ${TEST_OUTPUT}")
	message("--   Resources at: ${TEST_RESOURCE_DIR}")
else()
	message("-- Tests disabled")
endif()
if (${COVERAGE})
	message("-- Coverage enabled")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
	#set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
	#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
	set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} --coverage")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
else()
	message("-- Coverage disabled")
endif()

if (${PROFILING})
	message("-- Profiling enabled")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
	set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -pg")
else()
	message("-- Profiling disabled")
endif()
message("")

# JSONCPP
set(JSONCPP_VERSION "1.7.7"
	CACHE STRING "JSON-CPP library's version")
set(JSONCPP_URL "https://github.com/open-source-parsers/jsoncpp/archive/${JSONCPP_VERSION}.tar.gz"
	CACHE STRING "JSON-CPP library's git repository URL")
set(JSONCPP_DIR "${THIRD_PARTY_LIBRARIES}/jsoncpp-${JSONCPP_VERSION}"
	CACHE STRING "JSON-CPP destiny folder")
set(JSONCPP_MD5 "9b51c65c563463220a8cb5fa33d525f8"
	CACHE STRING "JSON-CPP md5")

# GTest
set(GTEST_VERSION "release-1.7.0"
	CACHE STRING "GTest library's version")
set(GTEST_URL "https://github.com/google/googletest/archive/${GTEST_VERSION}.tar.gz"
	CACHE STRING "GTest library's git repository URL")
set(GTEST_MD5 "4ff6353b2560df0afecfbda3b2763847"
	CACHE STRING "Google Test download MD5 hash for integrity verification")
set(GTEST_DIR "${THIRD_PARTY_LIBRARIES}/googletest-${GTEST_VERSION}"
	CACHE STRING "GTest destiny folder")

# libboost
set(BOOST_VERSION "1.62.0"
	CACHE STRING "GTest library's version")
string(REPLACE "."
	"_" BOOST_VERSION_UNDERSCORE
	${BOOST_VERSION})
set(BOOST_DIR "$ENV{HOME}/libs/boost_${BOOST_VERSION_UNDERSCORE}"
	CACHE STRING "Boost destiny folder")
set(BOOST_DOWNLOAD_MD5 "6095876341956f65f9d35939ccea1a9f"
	CACHE STRING "Boost library download MD5 hash for integrity verification")
set(BOOST_REQUIRED_COMPONENTS "system;filesystem;regex;log_setup;log;program_options"
	CACHE STRING "Boost library components")

# OpenCV
set(OPENCV_VERSION "3.1.0"
	CACHE STRING "OpenCV library's version")
set(OPENCV_DIR "${THIRD_PARTY_LIBRARIES}/opencv-${OPENCV_VERSION}"
	CACHE STRING "OpenCV destiny folder")
set(OPENCV_FILE_URL https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.tar.gz
	CACHE STRING "OpenCV library download URL")
set(OPENCV_DOWNLOAD_MD5 "70e1dd07f0aa06606f1bc0e3fa15abd3"
	CACHE STRING "OpenCV library download MD5 hash for integrity verification")

set(OPENCVCONTRIB_DIR "${THIRD_PARTY_LIBRARIES}/opencv_contrib-${OPENCV_VERSION}"
	CACHE STRING "OpenCV Contrib destiny folder")
set(OPENCVCONTRIB_DOWNLOAD_MD5 "a822839ad3ab79ff837c16785ea9dd10"
	CACHE STRING "OpenCV Contrib library download MD5 hash for integrity verification")
set(OPENCVCONTRIB_FILE_URL https://github.com/opencv/opencv_contrib/archive/${OPENCV_VERSION}.tar.gz
	CACHE STRING "OpenCV Contrib library download URL")

function(download result url md5 file)
	if (EXISTS ${file})
		file(MD5 ${file} file_md5)
		if (NOT ${file_md5} STREQUAL ${md5})
			file(REMOVE ${file})
			message("Downloading ${url}")
			file(DOWNLOAD ${url} ${file} SHOW_PROGRESS)
		endif()
	else()
		message("Downloading ${url}")
		file(DOWNLOAD ${url} ${file} SHOW_PROGRESS)
	endif()
	file(MD5 ${file} file_md5)
	if (${file_md5} STREQUAL ${md5})
		set(${result} ${file} PARENT_SCOPE)
	else()
		set(${result} "ERROR" PARENT_SCOPE)
		message(WARNING "MD5 verification failed.")
	endif()
endfunction(download)

message("JSONCPP")
if (NOT EXISTS ${JSONCPP_DIR})
	set(JSONCPP_TMP_FILE /tmp/jsoncpp.tar.gz)
	download(JSONCPP_RESULT ${JSONCPP_URL} ${JSONCPP_MD5} ${JSONCPP_TMP_FILE})
	if ("${JSONCPP_RESULT}" STREQUAL "ERROR")
		message(FATAL_ERROR "Cannot download JSON-CPP.")
	else()
		message("-- Extracting JSON-CPP to '${JSONCPP_DIR}'")
		execute_process(                                       # checkout to v2.0
			COMMAND tar xf ${JSONCPP_TMP_FILE}
			WORKING_DIRECTORY ${THIRD_PARTY_LIBRARIES}
			RESULT_VARIABLE JSON_RESULT
			ERROR_VARIABLE JSON_ERROR
			)
		if (${JSON_RESULT} EQUAL 0)
			message("-- CMaking ...")
			file(MAKE_DIRECTORY "${JSONCPP_DIR}/build")
			file(MAKE_DIRECTORY "${JSONCPP_DIR}/build/debug")
			file(MAKE_DIRECTORY "${JSONCPP_DIR}/install")
			file(MAKE_DIRECTORY "${JSONCPP_DIR}/install/debug")
			execute_process(
				COMMAND cmake -DCMAKE_INSTALL_PREFIX=../../install/debug -DBUILD_SHARED_LIBS=ON -config Debug ../..
				WORKING_DIRECTORY "${JSONCPP_DIR}/build/debug"
				OUTPUT_VARIABLE CMAKE_OUTPUT
				RESULT_VARIABLE CMAKE_RESULT
			)
			if (${CMAKE_RESULT} EQUAL 0)
				message("-- Making ...")
				execute_process(
					COMMAND make install
					WORKING_DIRECTORY "${JSONCPP_DIR}/build/debug"
					OUTPUT_VARIABLE MAKE_OUTPUT
					RESULT_VARIABLE MAKE_RESULT
				)
				if (MAKE_RESULT GREATER 0)
					file(REMOVE_RECURSE ${JSONCPP_DIR})
					message(FATAL_ERROR "Cannot make JSON-CPP.")
				else()
					message("-- Success!")
				endif()
			else()
				file(REMOVE_RECURSE ${JSONCPP_DIR})
				message(FATAL_ERROR "Cannot cmake JSON-CPP.")
			endif()
		else()
			message(FATAL_ERROR "Cannot extract JSON-CPP.")
		endif()
	endif()
endif()

find_library(jsoncpp jsoncpp
	HINTS "${JSONCPP_DIR}/install/debug/lib"
	NO_DEFAULT_PATH
	)
if (${jsoncpp} STREQUAL "jsoncpp-NOTFOUND")
	# tries to find the default installation.
	find_library(jsoncpp jsoncpp)
endif()
if (${jsoncpp} STREQUAL "jsoncpp-NOTFOUND")
	message(FATAL_ERROR "Cannot find jsoncpp at '${JSONCPP_DIR}/install/debug/lib'.")
else()
	message("-- JSONCPP found at ${jsoncpp}")
	set(JSONCPP_INCLUDE_DIR "${JSONCPP_DIR}/install/debug/include")
	message("-- Include directory: ${JSONCPP_INCLUDE_DIR}")
	include_directories(${JSONCPP_INCLUDE_DIR})
endif()
message("")

message("GTest")
if (NOT EXISTS ${GTEST_DIR})
	message("-- Downloading GTest (${GTEST_URL})")
	set(GTEST_TMP_FILE /tmp/gtest.tar.gz)
	download(GTEST_RESULT ${GTEST_URL} ${GTEST_MD5} ${GTEST_TMP_FILE})
	if (${GTEST_RESULT} EQUAL "ERROR")
		message(FATAL_ERROR "Cannot download GTest.")
	else()
		message("-- Extracting Google Test to ${GTEST_DIR} ...")
		execute_process(                                       # checkout to v2.0
			COMMAND tar xf ${GTEST_TMP_FILE}
			WORKING_DIRECTORY ${THIRD_PARTY_LIBRARIES}
			RESULT_VARIABLE GTEST_RESULT
			ERROR_VARIABLE GTEST_ERROR
			)
		if (${GTEST_RESULT} GREATER 0)                           # git checkout failed
			file(REMOVE_RECURSE ${GTEST_DIR})
			message(FATAL_ERROR "Cannot extract GTest: ${GTEST_ERROR}.")
		else()                                                 # git checkout success
			file(MAKE_DIRECTORY "${GTEST_DIR}/build")
			file(MAKE_DIRECTORY "${GTEST_DIR}/build/debug")
			message("-- CMaking ...")
			execute_process(
				COMMAND cmake -config Debug ../..
				WORKING_DIRECTORY "${GTEST_DIR}/build/debug"
				OUTPUT_VARIABLE CMAKE_OUTPUT
				RESULT_VARIABLE CMAKE_RESULT
			)
			if (${CMAKE_RESULT} EQUAL 0)
				message("-- Making ...")
				execute_process(
					COMMAND make
					WORKING_DIRECTORY "${GTEST_DIR}/build/debug"
					OUTPUT_VARIABLE MAKE_OUTPUT
					RESULT_VARIABLE MAKE_RESULT
				)
				if (MAKE_RESULT GREATER 0)
					file(REMOVE_RECURSE ${GTEST_DIR})
					message(FATAL_ERROR "Cannot make GTest.")
				else()
					message("-- Success!")
				endif()
			else()
				file(REMOVE_RECURSE ${GTEST_DIR})
				message(FATAL_ERROR "Cannot cmake GTest.")
			endif()
		endif()
	endif()
endif()
if (EXISTS libs/googletest)
	add_subdirectory(libs/googletest)
endif()
message("")

message("OpenCV")
if (NOT EXISTS ${OPENCV_DIR})
	message("-- Cannot find ${OPENCV_DIR}")
	set(OPENCV_TMP_FILE /tmp/opencv_${OPENCV_VERSION}.tar.gz)
	download(OPENCV_RESULT ${OPENCV_FILE_URL} ${OPENCV_DOWNLOAD_MD5} ${OPENCV_TMP_FILE})
	if (${OPENCV_RESULT} STREQUAL "ERROR")
		message(FATAL_ERROR "Cannot get ${OPENCV_FILE_URL}.")
	else()
		message("-- OK")
	endif()
	message("-- Extracting to ${THIRD_PARTY_LIBRARIES} ...")
	execute_process(
		COMMAND tar xf ${OPENCV_RESULT}
		WORKING_DIRECTORY ${THIRD_PARTY_LIBRARIES}
		RESULT_VARIABLE TAR_RESULT
		OUTPUT_VARIABLE TARGET_OUTPUT
	)
	if (${TAR_RESULT} EQUAL "0")
		message("-- OK")
	else()
		file(REMOVE_RECURSE ${BOOST_DIR})
		message(FATAL_ERROR "Cannot extract OpenCV.")
	endif()
endif()
if (NOT EXISTS ${OPENCVCONTRIB_DIR})
	message("-- Cannot find ${OPENCVCONTRIB_DIR}")
	set(OPENCVCONTRIB_TMP_FILE /tmp/opencv_contrib-${OPENCV_VERSION}.tar.gz)
	download(OPENCV_RESULT ${OPENCVCONTRIB_FILE_URL} ${OPENCVCONTRIB_DOWNLOAD_MD5} ${OPENCVCONTRIB_TMP_FILE})
	if (${OPENCV_RESULT} STREQUAL "ERROR")
		message(FATAL_ERROR "Cannot get ${OPENCVCONTRIB_FILE_URL}.")
	else()
		message("-- OK")
	endif()
	message("-- Extracting to ${THIRD_PARTY_LIBRARIES} ...")
	execute_process(
		COMMAND tar xf ${OPENCV_RESULT}
		WORKING_DIRECTORY ${THIRD_PARTY_LIBRARIES}
		RESULT_VARIABLE TAR_RESULT
		OUTPUT_VARIABLE TARGET_OUTPUT
	)
	if (${TAR_RESULT} EQUAL "0")
		message("-- OK")
	else()
		file(REMOVE_RECURSE ${OPENCV_DIR})
		message(FATAL_ERROR "Cannot extract OpenCV.")
	endif()
endif()
if (NOT EXISTS ${OPENCV_DIR}/build)
	message("-- CMaking ...")
	file(MAKE_DIRECTORY ${OPENCV_DIR}/build)
	execute_process(
		COMMAND cmake -DCMAKE_BUILD_TYPE=RELEASE -DWITH_VTK=OFF -DCMAKE_INSTALL_PREFIX=${OPENCV_DIR}/install -D OPENCV_EXTRA_MODULES_PATH=${OPENCVCONTRIB_DIR}/modules ${OPENCV_DIR}
		WORKING_DIRECTORY ${OPENCV_DIR}/build
		RESULT_VARIABLE OPENCV_RESULT
	)
	if (${OPENCV_RESULT} EQUAL 0)
		message("-- OK")
	else()
		file(REMOVE_RECURSE "${OPENCV_DIR}/build")
		message(FATAL_ERROR "Cannot cmake opencv.")
	endif()
endif()
if (NOT EXISTS ${OPENCV_DIR}/install)
	message("-- Compiling and installing opencv")
	file(MAKE_DIRECTORY ${OPENCV_DIR}/install)
	execute_process(
		COMMAND make install
		WORKING_DIRECTORY ${OPENCV_DIR}/build
		RESULT_VARIABLE OPENCV_RESULT
	)
	if (${OPENCV_RESULT} GREATER 0)
		file(REMOVE_RECURSE "${OPENCV_DIR}/install")
		message(FATAL_ERROR "Cannot build opencv.")
	endif()
endif()
find_package(OpenCV ${OPENCV_VERSION} REQUIRED PATHS "${OPENCV_DIR}/install/share/OpenCV/")
if (NOT ${OpenCV_FOUND})
	message(FATAL_ERROR "Cannot find OpenCv.")
endif()
message("-- Include dir: ${OpenCV_INCLUDE_DIRS}")
include_directories(${OpenCV_INCLUDE_DIRS})
message("-- Version: ${OpenCV_VERSION}")
message("-- Libs: ${OpenCV_LIBS}")
message("")

message("Boost")
find_package(Boost ${BOOST_VERSION} COMPONENTS ${BOOST_REQUIRED_COMPONENTS})
if (NOT ${Boost_FOUND})
	if (NOT EXISTS ${BOOST_DIR})
		message("-- Cannot find ${BOOST_DIR}")
		set(BOOST_FILE_URL http://downloads.sourceforge.net/project/boost/boost/${BOOST_VERSION}/boost_${BOOST_VERSION_UNDERSCORE}.tar.bz2)
		set(BOOST_TMP_FILE /tmp/libboost.tar.bz2)
		download(BOOST_RESULT ${BOOST_FILE_URL} ${BOOST_DOWNLOAD_MD5} ${BOOST_TMP_FILE})
		if (${BOOST_RESULT} STREQUAL "ERROR")
			message(FATAL_ERROR "Cannot get ${BOOST_FILE_URL}.")
		else()
			message("-- OK")
		endif()
		message("-- Extracting to ${THIRD_PARTY_LIBRARIES} ...")
		execute_process(
			COMMAND tar xf ${BOOST_TMP_FILE}
			WORKING_DIRECTORY ${THIRD_PARTY_LIBRARIES}
			RESULT_VARIABLE TAR_RESULT
			OUTPUT_VARIABLE TARGET_OUTPUT
		)
		if (${TAR_RESULT} EQUAL "0")
			message("-- OK")
		else()
			file(REMOVE_RECURSE ${BOOST_DIR})
			message(FATAL_ERROR "Cannot extract libboost.")
		endif()
	endif()

	if (NOT EXISTS ${BOOST_DIR}/b2)
		message("-- Bootstrapping libboost")
		execute_process(
			COMMAND sh bootstrap.sh
			WORKING_DIRECTORY ${BOOST_DIR}
			RESULT_VARIABLE BOOST_RESULT
		)
		if (${BOOST_RESULT} EQUAL 0)
			message("-- Ok")
		else()
			file(REMOVE_RECURSE "${BOOST_DIR}/install")
			message(FATAL_ERROR "Cannot extract libboost.")
		endif()
	endif()
	if (NOT EXISTS ${BOOST_DIR}/install)
		message("-- Compiling and installing libboost")
		file(MAKE_DIRECTORY ${BOOST_DIR}/install)
		execute_process(
			COMMAND "${BOOST_DIR}/b2" "--prefix=${BOOST_DIR}/install" install
			WORKING_DIRECTORY ${BOOST_DIR}
			RESULT_VARIABLE BOOST_RESULT
		)
		if (${BOOST_RESULT} GREATER 0)
			file(REMOVE_RECURSE "${BOOST_DIR}/install")
			message(FATAL_ERROR "Cannot build libboost. ${BOOST_OUTPUT}")
		endif()
	endif()
	set(BOOST_ROOT "${BOOST_DIR}/install/")
	find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS ${BOOST_REQUIRED_COMPONENTS})
	message("-- Include dir: ${Boost_INCLUDE_DIRS}")
	include_directories(${Boost_INCLUDE_DIRS})
	message("-- Libraries: ${Boost_LIBRARIES}")
endif()
message("")

set(SOURCE_FILES
	src/procs/framebufferiterator.cpp
	src/procs/framebufferiterator.h
	src/procs/floodfillstate.cpp
	src/procs/floodfillstate.h
	src/procs/segmentationcolourobject.cpp
	src/procs/segmentationcolourobject.h
	src/procs/segmentcolour.cpp
	src/procs/segmentcolour.h
	src/procs/gridfinder.cpp
	src/procs/gridfinder.h
	src/procs/colourmatches.cpp
	src/procs/colourmatches.h
	src/procs/visitors.cpp
	src/procs/visitors.h

	src/data/cube.cpp
	src/data/cube.h
)

add_executable(rubiks
	src/main.cpp

	${SOURCE_FILES}
)

set(rubiks_libraries
	"${OpenCV_LIBS};${jsoncpp};${Boost_LIBRARIES};pthread"
	)

target_link_libraries(rubiks
	${rubiks_libraries}
	)

if (${ENABLE_TEST})
	message("Tests")
	enable_testing()
	include_directories(${CMAKE_SOURCE_DIR}/src)
	file(GLOB_RECURSE TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*_test.cpp)

	add_executable(run_tests
		test/resources.h
		test/resources.cpp
		${TEST_SRCS}
		${SOURCE_FILES}
	)
	set_target_properties(run_tests PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT}
		)
	target_link_libraries(run_tests
		gtest gtest_main
		${rubiks_libraries}
		)
	foreach (testSrc ${TEST_SRCS})
		# get_filename_component(testName ${testSrc} NAME_WE)
		string(REPLACE "test/src/" "" testPackage "${testSrc}")
		string(REPLACE "_test.cpp" "" testPackage "${testPackage}")
		string(REPLACE "/" "_" testPackage "${testPackage}")
		message("-- ${testPackage}: ${testSrc}")
		add_test(
			NAME ${testPackage}
			WORKING_DIRECTORY ${TEST_OUTPUT}
			COMMAND run_tests --gtest_filter=${testPackage}.*
		)
	endforeach()
	message("")
endif()