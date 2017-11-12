cmake_minimum_required(VERSION 2.8)
project( ZincExamples)
find_package(Zinc 	REQUIRED)


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

#add cmgui examples
add_executable( cmgui_example_ai source/cmgui_example_ai.cpp )
add_executable( cmgui_example_mesh_integration source/cmgui_example_mesh_integration.cpp )




target_link_libraries( cmgui_example_ai zinc_wrapper  ${ZINC_LIB})
target_link_libraries( cmgui_example_mesh_integration zinc_wrapper  ${ZINC_LIB})




file(GLOB exnode_list 
	${CMAKE_SOURCE_DIR}/geometry/examples/a/ai/cmiss_input/*.exnode
	${CMAKE_SOURCE_DIR}/geometry/examples/a/mesh_integration/cmiss_input/*.exnode
)
file(GLOB exelem_list 
	${CMAKE_SOURCE_DIR}/geometry/examples/a/ai/cmiss_input/*.exelem
	${CMAKE_SOURCE_DIR}/geometry/examples/a/mesh_integration/cmiss_input/*.exelem
)
#file(GLOB exfile_list ${CMAKE_SOURCE_DIR}/geometry/examples/a/ai/cmiss_input/*.exfile)


add_custom_target(add_geometry_dlls

	#ADD ZINC .ex...
	COMMAND ${CMAKE_COMMAND} -E copy ${exnode_list}           ${CMAKE_BINARY_DIR}/Debug/
	COMMAND ${CMAKE_COMMAND} -E copy ${exnode_list}           ${CMAKE_BINARY_DIR}/

	COMMAND ${CMAKE_COMMAND} -E copy ${exelem_list}           ${CMAKE_BINARY_DIR}/Debug/
	COMMAND ${CMAKE_COMMAND} -E copy ${exelem_list}           ${CMAKE_BINARY_DIR}/


	#ADD ZINC .dll
	COMMAND ${CMAKE_COMMAND} -E copy ${Zinc_DIR}/../bin/${ZINC_LIB}.dll          ${CMAKE_BINARY_DIR}/Debug/
	
)




add_dependencies(cmgui_example_ai  add_geometry_dlls)
add_dependencies(cmgui_example_mesh_integration  add_geometry_dlls)