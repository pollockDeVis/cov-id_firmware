set(SOURCES 
"app.c"
"MLX90614_SMBus_Driver.c"
"MLX90614_API.c"
"temp_driver.c"
"led.c"
"sensor_filter.c"
"custom_list_iterator.c"
"custom_list_node.c"
"custom_list.c"
"MessageQueue.c"
"sensor_manager.c"
"https_manager.c"
"wifi_driver.c"
"uart_driver.c"
"json_parser.c"
)
idf_component_register(
        SRCS ${SOURCES}
        INCLUDE_DIRS ".")
        
include_directories(../inc)
