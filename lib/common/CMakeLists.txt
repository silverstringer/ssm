cmake_minimum_required( VERSION 3.1)
project(common)

message("Lib for Service Stock Market")
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Wno-unused-function)

add_library(${PROJECT_NAME}  SHARED dca.cpp)
add_library(lib::common ALIAS common)

target_include_directories( ${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}
        )

target_link_libraries(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)