[![Build status](https://github.com/Psyhich/mathxlib/actions/workflows/cmake.yml/badge.svg)](https://github.com/Psyhich/mathxlib/actions/workflows/cmake.yml)
![Coverage](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/Psyhich/b7156fe3db753fc241ad820cfe0a3566/raw/dev_badge.json)

# NOTICE: Currently project is very volatile 

# MathxLib - an easy math library
Library that I wrote on a first term as single header file and continued developing it.

## Usage
In CMake you can just use it as subfolder:
```cmake
add_subfolder(<path_to_directory>)
```

And after that link it to your project:
```cmake
target_link_libraries(target_name PUBLIC mathxlib)
```
