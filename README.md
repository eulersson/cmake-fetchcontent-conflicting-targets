# CMake FetchContent Conflicting Targets

**A possible workaround is found on branch `workaround` https://github.com/eulersson/cmake-fetchcontent-conflicting-targets/tree/workaround**.

Hello, I have problems building a project with two external dependencies that define the same target.

My goal is to be able to compile the project’s tests along with the project’s dependencies
tests too. For that I was thinking of building the unit tests when compiling from source
with FetchContent. If you included `include(CTest)` any subsequent calls to add_test would
include that test so afterwards all the tests (project + dependencies’) can be run with
`ctest` on on the build dir (`ctest --test-dir build`):

```
$ ctest --test-dir build
Internal ctest changing into directory: /home/eulersson/cmake-fetchcontent-conflicting-targets/build
Test project /home/eulersson/cmake-fetchcontent-conflicting-targets/build
(FIRST IT RUNS THE TESTS FROM THIS PROJECT (TEST_FOO))
      Start  1: test-foo
 1/62 Test  #1: test-foo ..........................   Passed    0.02 sec
 (NOW IT WOULD RUN THE TESTS FROM CJSON (DEPENDENCY))
      Start  2: cJSON_test
 2/62 Test  #2: cJSON_test ........................   Passed    0.02 sec
      Start  3: parse_examples
[...]
 (NOW IT WOULD RUN THE TESTS FROM LIBZMQ (DEPENDENCY))
[...]
```

Does this make sense? Perhaps there’s another way that conform to best practices?

When adding various dependencies into a CMake project, if you want to also build the
unit tests along them, I run into this problem where the target (in this case the unit
testing library “unity”) is being added twice by the various dependencies you want to
build tests for (add_library(unity STATIC [...])):

- add_library(unity [...]) in libzmq: https://github.com/zeromq/libzmq/blob/959a133520dfc80d29e83aa7ef762e1d0327f63b/tests/CMakeLists.txt#L206-L209
- add_library(unity [...]) in cJSON: https://github.com/DaveGamble/cJSON/blob/19ff92da79ee37c81a4cdf1f50a8dd1cbb02e84f/tests/CMakeLists.txt#L2

Reproducible exampe: GitHub - eulersson/cmake-fetchcontent-conflicting-targets: Problem illustrating using FetchContent on projects defining the same un-namespaced target.

```
$ git clone https://github.com/eulersson/cmake-conflicting-targets
$ cd cmake-conflicting-targets
$ cmake -S . -B build
[...]
CMake Error at build/_deps/cjson-src/tests/CMakeLists.txt:2 (add_library):
  add_library cannot create target "unity" because another target with the
  same name already exists.  The existing target is a static library created
  in source directory
  "/Users/ramon/Devel/conflicting-targets/build/_deps/libzmq-src/tests".  See
  documentation for policy CMP0002 for more details.
```

This would not be an issue if those targets were namespaced.

What would be a good workaround?

Thanks in advance.
