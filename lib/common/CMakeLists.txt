cmake_minimum_required( VERSION 3.1)
project(common VERSION 1.0.0 DESCRIPTION "Common Library for Service Stock Market")

message("Lib for Service Stock Market")
message( "Project  name:" ${PROJECT_NAME})

set(CMAKE_DEFAULT_BUILD_TYPE  "Release")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS -pthread)

cmake_policy(SET CMP0071 NEW)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-function")
endif()


#optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")
endif()


add_compile_options(-Wall -Wextra -Wno-unused-function)

add_library(${PROJECT_NAME}  SHARED dca.cpp grid.h logger.h future.h futurescalculator.cpp  investmentcalculator.h storage.h)
add_library(lib::common ALIAS common)

target_include_directories( ${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}
        )

target_link_libraries(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER grid.h logger.h )
#install(TARGETS ${PROJECT_NAME} DESTINATION bin)