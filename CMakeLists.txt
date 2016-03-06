cmake_minimum_required (VERSION 2.8.7 FATAL_ERROR)
project (Audiologger CXX)

# Set Version Number
set (Audiologger_VERSION_MAJOR 0)
set (Audiologger_VERSION_MINOR 1)

# Pass some of the CMake settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/headers/AudiologgerConfig.hpp.in"
	"${PROJECT_BINARY_DIR}/headers/AudiologgerConfig.hpp"
	)

include_directories("${PROJECT_BINARY_DIR}/headers")
include_directories("${PROJECT_SOURCE_DIR}/headers")

# Libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
## Portaudio
find_package (Portaudio REQUIRED)
include_directories(${PORTAUDIO_INCLUDE_DIRS})
set(LIBS ${LIBS} ${PORTAUDIO_LIBRARIES})

## SndFile
find_package (SndFile REQUIRED)
include_directories(${SNDFILE_INCLUDE_DIRS})
set(LIBS ${LIBS} ${SNDFILE_LIBRARIES})

## TagLib
find_package (Taglib REQUIRED)
include_directories(${TAGLIB_INCLUDES})
set(LIBS ${LIBS} ${TAGLIB_LIBRARIES})

add_executable(Audiologger src/main.cpp src/Audiolog.cpp src/LinkedList.cpp src/logging.cpp src/LogHandler.cpp src/Player.cpp src/Recorder.cpp)
target_link_libraries(Audiologger ${LIBS})


# add the install targets
install (TARGETS Audiologger DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/AudiologgerConfig.hpp"        
         DESTINATION include)

set(EXECUTABLE_OUTPUT_PATH bin)

target_compile_features(Audiologger PRIVATE cxx_range_for)
