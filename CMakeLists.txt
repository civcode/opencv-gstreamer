cmake_minimum_required(VERSION 2.9)

project(gst)

find_package(OpenCV REQUIRED)

# add include directories
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src    
)

# add source files
file(GLOB gst_SRC
    ${CMAKE_SOURCE_DIR/src}    
)

#add_library(my_lib LowPassFilter.cpp)
add_executable(tx-gst src/Sender.cpp ${gst_SRC} src/webcam.cpp)
add_executable(rx-gst src/Receiver.cpp ${gst_SRC})

target_link_libraries(tx-gst ${OpenCV_LIBS})
target_link_libraries(rx-gst ${OpenCV_LIBS})

