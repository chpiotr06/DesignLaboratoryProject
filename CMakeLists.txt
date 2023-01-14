cmake_minimum_required(VERSION 3.12)

include(build/pico_sdk_import.cmake)
set(PICO_SDK_FETCH_FROM_GIT on)

project(GPS_Tracker C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME}
        main.c
        )

add_subdirectory(lib)
target_link_directories(${PROJECT_NAME} PRIVATE lib)


# pull in common dependencies
target_link_libraries(${PROJECT_NAME} 
        pico_stdlib
        bluetoothHandler
        gpsHandler      
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

# Disable USB Serial communication, use UARTS only for GPS and bluetooth
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)


