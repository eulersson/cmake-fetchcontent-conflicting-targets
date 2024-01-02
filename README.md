# CMake FetchContent Conflicting Targets

```
cmake -S . -B build
cmake --build B

# Test the project.
ctest --test-dir build

# Test the project's dependencies.
ctest --test-dir build/zmq-prefix/src/zmq-build
ctest --test-dir build/cmocka-prefix/src/cmocka-build
ctest --test-dir build/cJSON-prefix/src/cJSON-build

# Run the project binary.
./build/foo
```

