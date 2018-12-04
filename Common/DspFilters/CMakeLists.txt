cmake_minimum_required(VERSION 3.11)

project(DspFilters VERSION 0.0.0)

set(src
	${CMAKE_CURRENT_SOURCE_DIR}/ChebyshevFiltre.h
	${CMAKE_CURRENT_SOURCE_DIR}/ChebyshevFiltre.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/Filter.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/Filter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/Common.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/Params.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/Param.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/Cascade.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/Cascade.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/Biquad.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/Biquad.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/ChebyshevII.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/ChebyshevII.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/PoleFilter.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/PoleFilter.cpp
	)
	
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	
add_library(${PROJECT_NAME} ${src})