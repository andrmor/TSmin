cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(TSmin)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(TSmin TSmin.cc ${sources} ${headers})
target_link_libraries(TSmin ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

install(TARGETS TSmin DESTINATION bin)

