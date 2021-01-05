#include <stdio.h>
#include <string.h>
#include "bbsx_serial.h"

void render_general_info(char * render_buffer, Connect_Response *response_ptr) {
    char voltage_string[8];
    switch(response_ptr->voltage) {
        case VOLTAGE_24V:
            strcpy(voltage_string, "24V");
            break;
        case VOLTAGE_36V:
            strcpy(voltage_string, "36V");
            break;
        case VOLTAGE_48V:
            strcpy(voltage_string, "48V");
            break;
        case VOLTAGE_60V:
            strcpy(voltage_string, "60V");
            break;
        case VOLTAGE_24Vto48V:
            strcpy(voltage_string, "24-48V");
            break;
        default:
            strcpy(voltage_string, "24-60V");
            break;
    }

    sprintf(render_buffer,
        "Manufacturer: %s\n"
        "Model: %s\n"
        "Hardware Version: %s\n"
        "Firmware Version: %s\n"
        "Voltage: %s\n"
        "Maximum Current: %dA\n",
        response_ptr->manufacturer,
        response_ptr->model,
        response_ptr->hardware_version,
        response_ptr->firmware_version,
        voltage_string,
        response_ptr->max_current
    );
}