cmake_minimum_required(VERSION 2.8)
project(audiotest CXX)

include_directories(SYSTEM /usr/local/include)

set(CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS " -L/usr/local/lib")
file(GLOB sauce "./src/*.cpp" "./src/*.hpp")
add_executable(audiotest ${sauce})
target_link_libraries(audiotest portaudio ogg vorbis vorbisfile lua.5.2)





