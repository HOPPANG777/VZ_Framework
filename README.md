# VZ_Framework

This project previously relied on Visual Studio project files and post-build scripts.  
A cross-IDE build system using CMake is now provided.

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The build outputs and copied resources will be placed in `build/bin`.