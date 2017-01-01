
project(opencvdream1)

set(SRC_LIST1 user/main.cpp cardrive/cardrive.cpp cardrive/cardrive.hpp)
set(SRC_LIST2 usart/usart.cpp usart/usart.h sonar/sonar.cpp sonar/sonar.h)
set(SRC_LIST3 line_detection/line_detection.cpp line_detection/line_detection.h)
set(SRC_LIST4  nanopi/GPIO.cpp nanopi/GPIO.h nanopi/S5P4418.cpp  nanopi/S5P4418.h)
set(SRC_LIST5  lib/pwm.cpp lib/includes/libfahw-pwm.h  lib/common.h lib/common.cpp lib/includes/libfahw-filectl.h)
set(SRC_LIST  ${SRC_LIST1} ${SRC_LIST2} ${SRC_LIST3} ${SRC_LIST4} ${SRC_LIST5} )


find_package(OpenCV REQUIRED)
add_executable(main ${SRC_LIST}  )
target_link_libraries(main ${OpenCV_LIBS})



