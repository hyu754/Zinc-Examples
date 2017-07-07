
ADD_LIBRARY(zinc_wrapper source/zinc_wrapper.cpp)

target_link_libraries(zinc_wrapper ${ZINC_LIB} ${OpenCV_LIBS})