cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package(Zinc 	REQUIRED)
find_package( OpenCV REQUIRED )

#add a new varialbe for the zinc library, change depending on version
SET(ZINC_LIB zinc.r11306.3.0.1)

#include directories
include_directories(${Zinc_DIR}/../include)
include_directories(${CMAKE_SOURCE_DIR}/zinc_wrapper/include/)
include_directories(${CMAKE_SOURCE_DIR}/include/)

#link libraries
link_directories(${Zinc_DIR}/../lib)



#add zinc wrapper
add_subdirectory(zinc_wrapper)

#display some variables
message(${Zinc_DIR})
message(${ZINC_LIB})

add_executable( Zinc_Augment source/new_cmiss.cpp )
target_link_libraries( Zinc_Augment zinc_wrapper ${OpenCV_LIBS} ${ZINC_LIB})