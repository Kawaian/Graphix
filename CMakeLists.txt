###########################################################################
########################### DO NOT EDIT ME !!! ############################
###########################################################################

# Copyright Matyas Yves Constans

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

# http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#####################
## CMAKE RULES ######
#####################

CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT(SpikyEngine)

# On Visual 2005 and above, this module can set the debug working directory
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")

if ( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
	message ( FATAL_ERROR "Please select another build directory (Like VS_Build) !" )
endif()
if ( CMAKE_SOURCE_DIR MATCHES " " )
	message ("Warning : Your source directory contains spaces ! This make cause problems on some platforms..")
endif()
if ( CMAKE_BINARY_DIR MATCHES " " )
	message( "Warning : Your build directory contains spaces ! This make cause problems on some platforms.." )
endif()


if ( CMAKE_BUILD_TYPE STREQUAL "" )
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(
		CMAKE_BUILD_TYPE
			"Debug"
		CACHE STRING
			"Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\"."
		FORCE
	)
endif ( CMAKE_BUILD_TYPE STREQUAL "" )

if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
add_definitions( -O2 )
endif ( CMAKE_BUILD_TYPE STREQUAL "Release" )



#fetch source files
FILE 	(GLOB_RECURSE SRC_DIR
	${SpikyEngine_SOURCE_DIR}/lib
	${SpikyEngine_SOURCE_DIR}/tests
	)
FILE  (GLOB_RECURSE HEADER_DIR
	${SpikyEngine_SOURCE_DIR}/include/Core
	${SpikyEngine_SOURCE_DIR}/include/GL
	${SpikyEngine_SOURCE_DIR}/include/Graphics
	${SpikyEngine_SOURCE_DIR}/include/Physics
	 )

FILE(GLOB SRC_FILES 
			${SpikyEngine_SOURCE_DIR}/lib/*.cpp
			${SpikyEngine_SOURCE_DIR}/tests/*.cpp
			)
FILE(GLOB HEADER_FILES
			${SpikyEngine_SOURCE_DIR}/include/Core/*.h
			${SpikyEngine_SOURCE_DIR}/include/Core/*.hpp
			${SpikyEngine_SOURCE_DIR}/include/Physics/*.h
			${SpikyEngine_SOURCE_DIR}/include/Physics/*.hpp
			${SpikyEngine_SOURCE_DIR}/include/Graphics/*.h
			${SpikyEngine_SOURCE_DIR}/include/Graphics/*.hpp
			${SpikyEngine_SOURCE_DIR}/include/GL/*.h
			${SpikyEngine_SOURCE_DIR}/include/GL/*.hpp
		)

#executable
ADD_EXECUTABLE( ${PROJECT_NAME}
 				${SRC_FILES}
				${HEADER_FILES}
     		  )

		
#Opengl
FIND_PACKAGE(OpenGL REQUIRED)

# Glew
INCLUDE("cmake/FindGlew.cmake")
# SDL2
INCLUDE("cmake/FindSDL2.cmake")
# Assimp
INCLUDE("cmake/FindAssimp.cmake")


INCLUDE_DIRECTORIES(
				${SRC_DIR}
                ${HEADER_DIR}
				${GLEW_INCLUDE_DIRS}     	#Glew
				${SDL2_INCLUDE_DIRS}	 	#SDL2
				${ASSIMP_INCLUDE_DIRS}   	#Assimp
				"./external/Glm/include"    #GLM
				"./external/boost/include"    #boost
			 )

TARGET_LINK_LIBRARIES(
                ${PROJECT_NAME}
				${OPENGL_LIBRARIES}
				${GLEW_LIBRARIES}
				${SDL2_LIBRARIES}
				${ASSIMP_LIBRARIES}
				"${CMAKE_SOURCE_DIR}/external/boost/lib/x86/libboost_log-vc141-mt-gd-1_65_1.lib"
				"${CMAKE_SOURCE_DIR}/external/boost/lib/x86/libboost_system-vc141-mt-gd-1_65_1.lib"
				"${CMAKE_SOURCE_DIR}/external/boost/lib/x86/libboost_filesystem-vc141-mt-gd-1_65_1.lib"
				"${CMAKE_SOURCE_DIR}/external/boost/lib/x86/libboost_date_time-vc141-mt-gd-1_65_1.lib"
				"${CMAKE_SOURCE_DIR}/external/boost/lib/x86/libboost_thread-vc141-mt-gd-1_65_1.lib"
			   )