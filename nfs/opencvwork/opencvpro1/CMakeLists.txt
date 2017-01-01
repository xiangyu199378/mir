project(opencv002)
find_package(OpenCV REQUIRED)
add_executable(opencv002 opencv002)
target_link_libraries(opencv002 ${OpenCV_LIBS})
