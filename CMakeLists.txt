cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
project( ZincExamples)
#find_package(Zinc 	REQUIRED)

set(OpenCMISSLibs_DIR /home/david/Documents/opencmiss_sdk)
find_package(OpenCMISSLibs  REQUIRED COMPONENTS [ZINC|Iron] CONFIG)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

#add a new varialbe for the zinc library, change depending on version
set(ZINC_LIB ${OpenCMISSLibs_DIR}/lib/libzinc.so)

#include directories
include_directories(${OpenCMISSLibs_DIR}/include)
include_directories(${OPENGL_INCLUDE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/zinc_wrapper/include/)
include_directories(${CMAKE_SOURCE_DIR}/include/)

#link libraries
link_directories(${OpenCMISSLib_DIR}/lib)

#add zinc wrapper
add_subdirectory(zinc_wrapper)

#add cmgui examples
add_executable( cmgui_example_ai source/cmgui_example_ai.cpp )
add_executable( cmgui_example_mesh_integration source/cmgui_example_mesh_integration.cpp )
#add_executable( cmgui_example_optimisation_fitting-1D-linear  source/cmgui_example_optimisation_fitting-1D-linear.cpp )





target_link_libraries( cmgui_example_ai zinc_wrapper ${ZINC_LIB} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}) 
target_link_libraries( cmgui_example_mesh_integration zinc_wrapper ${ZINC_LIB} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}) 
#target_link_libraries( cmgui_example_mesh_integration zinc_wrapper opencmiss) 
#target_link_libraries(cmgui_example_optimisation_fitting-1D-linear zinc_wrapper opencmiss)
#add_opencmiss_environment(cmgui_example_ai)


file(GLOB exnode_list 
	${CMAKE_SOURCE_DIR}/geometry/examples/a/ai/cmiss_input/*.exnode
	${CMAKE_SOURCE_DIR}/geometry/examples/a/mesh_integration/cmiss_input/*.exnode
	${CMAKE_SOURCE_DIR}/geometry/examples/a/optimisation/fitting-1D-linear/cmiss_input/*.exnode
)
file(GLOB exelem_list 
	${CMAKE_SOURCE_DIR}/geometry/examples/a/ai/cmiss_input/*.exelem
	${CMAKE_SOURCE_DIR}/geometry/examples/a/mesh_integration/cmiss_input/*.exelem
	${CMAKE_SOURCE_DIR}/geometry/examples/a/optimisation/fitting-1D-linear/cmiss_input/*.exelem
)
#file(GLOB exfile_list ${CMAKE_SOURCE_DIR}/geometry/examples/a/ai/cmiss_input/*.exfile)


add_custom_target(copy_exfiles

	#ADD ZINC .ex...
#	COMMAND ${CMAKE_COMMAND} -E copy ${exnode_list}           ${CMAKE_BINARY_DIR}/Debug/
	COMMAND ${CMAKE_COMMAND} -E copy ${exnode_list}           ${CMAKE_BINARY_DIR}/

#	COMMAND ${CMAKE_COMMAND} -E copy ${exelem_list}           ${CMAKE_BINARY_DIR}/Debug/
	COMMAND ${CMAKE_COMMAND} -E copy ${exelem_list}           ${CMAKE_BINARY_DIR}/


	
)




add_dependencies(cmgui_example_ai  copy_exfiles)
#add_dependencies(cmgui_example_mesh_integration  add_geometry_dlls)
#add_dependencies(cmgui_example_optimisation_fitting-1D-linear add_geometry_dlls)
