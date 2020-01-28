cmake_minimum_required(VERSION 3.10)
project(mtfind)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_COMPILER g++-6)

option(WITH_TESTS "Build test (over gtest)" ON)

# If ccache is available, then use it.
find_program(CCACHE ccache)
if(CCACHE)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE})
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE})
endif(CCACHE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZERS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZERS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZERS}")

include_directories(src)

file(GLOB_RECURSE CPP_FILES src/*.cpp src/*.c)
file(GLOB_RECURSE TEST_FILES src/*_test.cpp)
file(GLOB_RECURSE AUX_FILES main.cpp test.cpp)

message(STATUS "Aux files:")
foreach(F ${AUX_FILES})
	message(${F})
	list(REMOVE_ITEM CPP_FILES ${F})
	list(REMOVE_ITEM TEST_FILES ${F})
endforeach()

message(STATUS "Test files:")
foreach(F ${TEST_FILES})
	message(${F})
	list(REMOVE_ITEM CPP_FILES ${F})
endforeach()

message(STATUS "Source files:")
foreach(F ${CPP_FILES})
	message(${F})
endforeach()

add_library(core ${CPP_FILES})
target_link_libraries(core)

if (WITH_TESTS)
	message(STATUS "Testing is enabled")

	find_package(GTest REQUIRED)

	# If find_package reports about libraries not found, just use follow command:
	#  cd /usr/src/gtest
	#  sudo cmake CMakeLists.txt
	#  sudo make
	#  sudo ln -s /usr/src/gtest/*.a /usr/lib/
	enable_testing()

	include_directories(${GTEST_INCLUDE_DIRS})

	add_executable(tests ${TEST_FILES})
	target_link_libraries(tests core ${GTEST_BOTH_LIBRARIES} pthread)
endif()

add_executable(mtfind src/main.cpp)
target_link_libraries(mtfind core pthread)
