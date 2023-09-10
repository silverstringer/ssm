cmake_minimum_required( VERSION 3.1)
project(common VERSION 1.0.0 DESCRIPTION "Common Library for Service Stock Market")

message("Lib for Service Stock Market")
message( "Project  name:" ${PROJECT_NAME})

set(CMAKE_DEFAULT_BUILD_TYPE  "Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wno-unused-function)

add_library(${PROJECT_NAME}  SHARED dca.cpp grid.h logger.h future.h)
add_library(lib::common ALIAS common)

target_include_directories( ${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}
        )

target_link_libraries(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER grid.h logger.h )
#install(TARGETS ${PROJECT_NAME} DESTINATION bin)