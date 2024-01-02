cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0135 NEW)

project(conflicts C)

include(ExternalProject)

include(CTest)
enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ExternalProject_Add(
  zmq
  URL https://github.com/zeromq/libzmq/archive/refs/tags/v4.3.5.tar.gz
  URL_HASH MD5=cfb80c6ad694cacc2f7c99bf44e8b686
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/zmq
  CMAKE_CACHE_ARGS -DBUILD_TESTS:BOOL=FALSE
)
list(APPEND EP_ADDED_${PROJECT_NAME}_DEPENDENCIES zmq)

ExternalProject_Add(
  cJSON
  URL https://github.com/DaveGamble/cJSON/archive/refs/tags/v1.7.16.tar.gz
  URL_HASH MD5=9422f73fb9e4e524d77d9467d4b73536
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/cJSON
  CMAKE_CACHE_ARGS
    -DENABLE_CJSON_TEST:BOOL=TRUE
)
list(APPEND EP_ADDED_${PROJECT_NAME}_DEPENDENCIES cJSON)

ExternalProject_Add(
  cmocka
  URL https://cmocka.org/files/1.1/cmocka-1.1.7.tar.xz
  URL_HASH MD5=dc23f4870eb7dfa6da065f3056e815b0
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/cmocka
  CMAKE_CACHE_ARGS
    -DUNIT_TESTING:BOOL=TRUE
)
list(APPEND EP_ADDED_${PROJECT_NAME}_TEST_DEPENDENCIES cmocka)

add_executable(foo foo.c)

message("EP_ADDED_${PROJECT_NAME}_DEPENDENCIES: ${EP_ADDED_${PROJECT_NAME}_DEPENDENCIES}")
message("EP_ADDED_${PROJECT_NAME}_TESTING_DEPENDENCIES: ${EP_ADDED_${PROJECT_NAME}_TESTING_DEPENDENCIES}")

# So that if any of the external dependencies change then foo is rebuilt.
add_dependencies(foo ${EP_ADDED_${PROJECT_NAME}_DEPENDENCIES} ${EP_ADDED_${PROJECT_NAME}_TEST_DEPENDENCIES})

foreach(dep IN LISTS EP_ADDED_${PROJECT_NAME}_DEPENDENCIES EP_ADDED_${PROJECT_NAME}_TEST_DEPENDENCIES)
  message("[Dependency] [${dep}] Adding includes and libs to interface target ${dep}_lib...")
  add_library(${dep}_lib INTERFACE)
  target_include_directories(
    ${dep}_lib
    INTERFACE
    ${CMAKE_BINARY_DIR}/${dep}/include/
  )
  target_link_libraries(
    ${dep}_lib
    INTERFACE
    ${CMAKE_BINARY_DIR}/${dep}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${dep}${CMAKE_SHARED_LIBRARY_SUFFIX}
  )

  # Link against the interface lib, that way instead of having to run:
  #
  #   `target_include_directories(foo ${CMAKE_BINARY_DIR}/${dep}/include/)`
  #   `target_link_libraries(foo ${CMAKE_BINARY_DIR}/${dep}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}${dep}${CMAKE_SHARED_LIBRARY_SUFFIX})`
  #
  # we can simply link to the interface library which has this information (includes and
  # libs) associated:
  #
  #   `target_link_libraries(foo ${dep}_lib)`
  #
  # That makes it cleaner when we have to link to these deps on other executables or unit tests.
  target_link_libraries(foo PRIVATE ${dep}_lib)
endforeach()

set(
  ${PROJECT_NAME}_unit_tests
  test_foo
)
foreach(${PROJECT_NAME}_unit_test ${${PROJECT_NAME}_unit_tests})
  add_executable(${${PROJECT_NAME}_unit_test} tests/${${PROJECT_NAME}_unit_test}.c)
  target_link_libraries(${${PROJECT_NAME}_unit_test} cJSON_lib cmocka_lib)
  add_test(${${PROJECT_NAME}_unit_test} ${${PROJECT_NAME}_unit_test})
endforeach()

# Run tests with:
# ctest --test-dir build
# ctest --test-dir build/zmq-prefix/src/zmq-build
# ctest --test-dir build/cmocka-prefix/src/cmocka-build
# ctest --test-dir build/cJSON-prefix/src/cJSON-build
