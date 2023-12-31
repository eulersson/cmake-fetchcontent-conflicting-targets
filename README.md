# CMake FetchContent Conflicting Targets

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

When adding various dependencies into a CMake project, if you want to also build the
unit tests along them, I run into this problem where the target (in this case the unit
testing library "unity") is being added twice by the various dependencies you want to
build tests for (`add_library(unity STATIC [...])`):

- `add_library(unity [...])` in libzmq: https://github.com/zeromq/libzmq/blob/959a133520dfc80d29e83aa7ef762e1d0327f63b/tests/CMakeLists.txt#L206-L209
- `add_library(unity [...])` in cJSON: https://github.com/DaveGamble/cJSON/blob/19ff92da79ee37c81a4cdf1f50a8dd1cbb02e84f/tests/CMakeLists.txt#L2

This would not be an issue if those targets were namespaced.

What would be a good workaround?
