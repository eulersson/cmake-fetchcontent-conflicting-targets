cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0135 NEW)

project(conflicts)

include(CTest)
include(FetchContent)

add_executable(foo foo.c)

# https://github.com/zeromq/libzmq/blob/959a133520dfc80d29e83aa7ef762e1d0327f63b/tests/CMakeLists.txt#L206-L209
#
#   add_library(unity STATIC [...])
#
FetchContent_Declare(
  libzmq
  URL https://github.com/zeromq/libzmq/archive/refs/tags/v4.3.4.tar.gz
  URL_HASH MD5=cc20b769ac10afa352e5ed2769bb23b3
)
set(BUILD_TESTS ON CACHE BOOL "libzmq: Build unit tests" FORCE)
FetchContent_MakeAvailable(libzmq)
target_link_libraries(foo PUBLIC libzmq)

# https://github.com/DaveGamble/cJSON/blob/19ff92da79ee37c81a4cdf1f50a8dd1cbb02e84f/tests/CMakeLists.txt#L2
#
#  add_library(unity STATIC [...])
#
FetchContent_Declare(
  cJSON
  URL https://github.com/DaveGamble/cJSON/archive/refs/tags/v1.7.15.tar.gz
  URL_HASH MD5=921b4bcb401aa604dc632fdb1c8dbdea
)
set(ENABLE_CJSON_TEST ON CACHE BOOL "cJSON: Build unit tests" FORCE)
FetchContent_MakeAvailable(cJSON)
# FAILS cannot create target "unity" because another target with the same name already
# exists. The existing target is a static library created in source directory
# "/Users/eulersson/Devel/conflicting-targets/build/_deps/libzmq-src/tests".
# See documentation for policy CMP0002 for more details.
configure_file(${cJSON_SOURCE_DIR}/cJSON.h ${CMAKE_BINARY_DIR}/include/cJSON/cJSON.h)
target_include_directories(foo PUBLIC ${CMAKE_BINARY_DIR}/include)
target_link_libraries(foo PUBLIC cjson)

FetchContent_Declare(
  cmocka
  URL https://cmocka.org/files/1.1/cmocka-1.1.7.tar.xz
  URL_HASH MD5=dc23f4870eb7dfa6da065f3056e815b0
)
set(WITH_EXAMPLES OFF CACHE BOOL "CMocka: Build examples" FORCE)
set(UNIT_TESTING ON CACHE BOOL "CMocka: Build unit tests" FORCE)
FetchContent_MakeAvailable(cmocka)
add_executable(suite-foo tests/test_foo.c)
target_link_libraries(suite-foo cmocka::cmocka)
add_test(test-foo suite-foo)
