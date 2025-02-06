# REST API
![Build status](https://github.com/wrazik/rest_api/actions/workflows/build.yml/badge.svg)

## Description
This is example boost::beast server that serves REST API. It was based on boost::beast async server example.

## Installation
### Requirements
* vcpkg
 
This package requires vcpkg to be set up. Remember to check out code with submodules. Then, go into vcpkg directory and run the following commands: 
```bash
$ cd vcpkg
$ ./bootstrap-vcpkg.sh
```
* ninja
 
By default, vcpkg uses ninja as the build system.
 
## Usage
To build the project, run the following commands:
```bash
 cmake --build --preset ninja-vcpkg-release 
```
Binary files will be located in the `./builds/ninja-multi-vcpkg/Release` directory.
Binary takes few arguments as input:
```bash
./rest_api <host>  <port> <thread_count>
```
Example:
```bash
 ./builds/ninja-multi-vcpkg/Release/rest_api 0.0.0.0 5000 3
 ```

Code is compiled on CI, take a look at [Workflow](./.github/workflows/build.yml) file.
  
## Unit tests
To run unit tests, run the following commands:
```bash
ctest --preset test-release
```
## Integration tests
To run integration tests (in python), it is recommended to use a virtual environment. To set it up, run the following commands:
```bash
cd integration_tests
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```
Then, to run the tests, run the following commands:
```bash
pytest
```