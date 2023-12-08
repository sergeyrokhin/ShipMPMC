cmake_minimum_required(VERSION 3.12)


project(producer_consumer CXX)

set(SRC_FILES_CPP main.cpp ship.cpp)
set(SRC_FILES_HPP ship.h ship_generator.h preloader.h tunnel.h)

add_executable(${PROJECT_NAME} ${SRC_FILES_CPP} ${SRC_FILES_HPP})

list(APPEND TARGETS ${PROJECT_NAME})

set_target_properties(${TARGETS} PROPERTIES  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

foreach(target ${TARGETS})
  target_compile_features(${target} PUBLIC cxx_std_17)
  set_target_properties(${target} PROPERTIES CXX_STANDARD_REQUIRED ON)

  target_link_libraries(${target} PRIVATE ${CMAKE_THREAD_LIBS_INIT})

  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${target}
      PRIVATE -g -Wall -Wextra -Werror -pedantic)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${target}
      PRIVATE /W4 /WX)
  endif()
endforeach()


