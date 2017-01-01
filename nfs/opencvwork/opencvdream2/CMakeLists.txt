
project(opencvdream1)

set(SRC_user user/main.cpp cardrive/cardrive.cpp cardrive/cardrive.hpp)
set(SRC_func_f1 line_detection/line_detection.cpp line_detection/line_detection.h)
set(SRC_func_f2 tracking/featuretracking.cpp tracking/featuretracking.hpp)
set(SRC_func_f3  videotransmission/SocketVideoserver.cpp  videotransmission/SocketVideoserver.hpp)
set(SRC_func  ${SRC_func_f1} ${SRC_func_f2} ${SRC_func_f3} )

set(SRC_lib_h1   lib/includes/libfahw-gpio.h    lib/includes/libfahw-i2c.h        lib/includes/libfahw-iio.h     lib/includes/libfahw-pcf8591.h) 
set(SRC_lib_h2   lib/includes/libfahw-adxl34x.h lib/includes/libfahw-GPIOSensor.h lib/includes/libfahw-hmc5883.h lib/includes/libfahw-w1.h)
set(SRC_lib_h3 	 lib/includes/libfahw-pwm.h      lib/includes/libfahw-bmp180.h  lib/includes/libfahw-encoder.h)
set(SRC_lib_h4	 lib/includes/libfahw-spi.h     lib/includes/libfahw-oled.h       lib/includes/libfahw-filectl.h )
set(SRC_lib_h ${SRC_lib_h1} ${SRC_lib_h2}  ${SRC_lib_h3}  ${SRC_lib_h4})

set(SRC_lib_src1  lib/adxl34x.cpp lib/config.h lib/i2c.cpp  lib/oled.cpp lib/spi.cpp lib/bmp180.cpp )  
set(SRC_lib_src2  lib/filectl.cpp lib/iio.cpp  lib/w1.cpp lib/common.cpp )
set(SRC_lib_src3  lib/gpio.cpp lib/pcf8591.cpp lib/common.h lib/gpio_sensor.cpp  lib/led.cpp  lib/pwm.cpp)
set(SRC_lib_src4  lib/config.cpp   lib/hmc5883.cpp  lib/rotary_encoder.cpp)
set(SRC_lib_src   ${SRC_lib_src1}  ${SRC_lib_src2}  ${SRC_lib_src3}  ${SRC_lib_src4})

set(SRC_LIST  ${SRC_user} ${SRC_func} ${SRC_lib_h} ${SRC_lib_src})

find_package(OpenCV REQUIRED)
add_executable(main ${SRC_LIST})
target_link_libraries(main ${OpenCV_LIBS})



