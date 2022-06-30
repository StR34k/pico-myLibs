target_link_libraries(${PROJECT_NAME}
    hardware_gpio
)

add_subdirectory("spi/" "spi/")
add_subdirectory("i2c/" "i2c/")
