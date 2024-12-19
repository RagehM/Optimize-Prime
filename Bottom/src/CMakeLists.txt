add_library(pico_pdm_microphone STATIC)

# Generate PIO header
pico_generate_pio_header(pico_pdm_microphone ${CMAKE_CURRENT_LIST_DIR}/mic/pdm_microphone.pio)



add_executable(${ProjectName}
    main.cpp
    servo/Servo.cpp
    dcMotor/DcMotor.cpp
    i2c/i2c.cpp
    ServoCluster/ServoCluster.cpp
    communication/communication.cpp
)



# Add PDM microphone sources - remove INTERFACE keyword
target_sources(pico_pdm_microphone
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/mic/pdm_microphone.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mic/OpenPDM2PCM/OpenPDMFilter.cpp
)


add_definitions(-DEIDSP_QUANTIZE_FILTERBANK=0
                -DEI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN=0
                -DARM_MATH_LOOPUNROLL
                )
include(edge-impulse-sdk/cmake/utils.cmake)
add_subdirectory(edge-impulse-sdk/cmake/zephyr)
                RECURSIVE_FIND_FILE_APPEND(MODEL_SOURCE "tflite-model" "*.cpp")
                RECURSIVE_FIND_FILE(SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/edge-impulse-sdk" "*.cpp")
                RECURSIVE_FIND_FILE(MODEL_FILES "${CMAKE_CURRENT_LIST_DIR}/tflite-model" "*.cpp")
                RECURSIVE_FIND_FILE(CC_FILES "${CMAKE_CURRENT_LIST_DIR}/edge-impulse-sdk" "*.cc")
                RECURSIVE_FIND_FILE(C_FILES "${CMAKE_CURRENT_LIST_DIR}/edge-impulse-sdk" "*.c")
                
                list(APPEND SOURCE_FILES ${S_FILES})
                list(APPEND SOURCE_FILES ${C_FILES})
                list(APPEND SOURCE_FILES ${CC_FILES})
                list(APPEND SOURCE_FILES ${MODEL_FILES})
                





# Add include directories - change to PUBLIC visibility
target_include_directories(pico_pdm_microphone 
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/mic/include
    ${CMAKE_CURRENT_LIST_DIR}/mic/OpenPDM2PCM
    ${CMAKE_CURRENT_LIST_DIR}/model-parameters
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/mic/include
    ${CMAKE_CURRENT_LIST_DIR}/mic/OpenPDM2PCM
    ${CMAKE_CURRENT_BINARY_DIR}  
    ${CMAKE_CURRENT_LIST_DIR}/tflite-model
)



# Link dependencies - change to PUBLIC visibility
target_link_libraries(pico_pdm_microphone
    PUBLIC
    pico_stdlib
    hardware_dma 
    hardware_pio
)


pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_LIST_DIR}/mic/pdm_microphone.pio)
target_sources(${ProjectName} PRIVATE ${SOURCE_FILES}     ${CMAKE_CURRENT_LIST_DIR}/mic/pdm_microphone.cpp
${CMAKE_CURRENT_LIST_DIR}/mic/OpenPDM2PCM/OpenPDMFilter.cpp
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    servo
    dcMotor
    i2c
    ServoCluster
    communication
    ${CMAKE_CURRENT_LIST_DIR}/mic/include
    ${CMAKE_CURRENT_LIST_DIR}/mic/OpenPDM2PCM

)

target_link_libraries(${ProjectName} 
    pico_stdlib
    hardware_adc
    #FreeRTOS-Kernel-Heap4
    hardware_pwm
    hardware_i2c
    hardware_irq
    hardware_pio
    hardware_dma
    hardware_clocks
)

pico_add_extra_outputs(${ProjectName})

pico_enable_stdio_usb(${ProjectName} TRUE)
pico_enable_stdio_uart(${ProjectName} FALSE)