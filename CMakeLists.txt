project(neglectedfraction)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

message(${CMAKE_PREFIX_PATH})

find_package(ROOT REQUIRED COMPONENTS Hist Gpad Graf MathCore) 

include(${ROOT_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(simulatedCompton SHARED simulatedCompton.cpp)
add_library(experimentalTOT SHARED experimentalTOT.cpp)
#add_library(experimentalAmplitude SHARED experimentalAmplitude.cpp)

target_link_libraries(simulatedCompton ${ROOT_LIBRARIES})
target_link_libraries(experimentalTOT ${ROOT_LIBRARIES})
#target_link_libraries(experimentalAmplitude ${ROOT_LIBRARIES})

add_executable(neglectedfraction main.cpp)

target_link_libraries(neglectedfraction simulatedCompton experimentalTOT )