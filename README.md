# REST API
![Build status](https://github.com/wrazik/rest_api/actions/workflows/build.yml/badge.svg)

## Description
TODO

## Installation
### Requirements
* vcpkg
 
This package requires vcpkg to be installed. 
To install vcpkg, run the following commands:
```bash
$ git clone git@github.com:microsoft/vcpkg.git
$ cd vcpkg
$ ./bootstrap-vcpkg.sh
```
* ninja
 
By default, vcpkg uses ninja as the build system.
 
## Usage
To build the project, run the following commands:
```bash
cmake --preset=default
cmake --build build
```

