message("Running makefsdata python script")
execute_process(COMMAND
    py makefsdata.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_executable(${ProjectName}
    main.cpp
    servo/Servo.cpp
    dcMotor/DcMotor.cpp
    i2c/i2c.cpp
    camera/camera.cpp
    ServoCluster/ServoCluster.cpp
    communication/communication.cpp
    reset/reset.cpp
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    wifi
    servo
    dcMotor
    i2c
    camera
    ServoCluster
    communication
    reset
)

target_link_libraries(${ProjectName} 
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
    pico_stdlib
    hardware_adc
    FreeRTOS-Kernel-Heap4
    hardware_pwm
    hardware_i2c
    hardware_irq
)

pico_add_extra_outputs(${ProjectName})

pico_enable_stdio_usb(${ProjectName} TRUE)
pico_enable_stdio_uart(${ProjectName} FALSE)