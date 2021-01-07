#include <stdio.h>
#include <string.h>
#include "bbsx_serial.h"

void render_basic_info(char * render_buffer, Basic_Read_Response *response_ptr) {
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

void render_general_info(char * render_buffer, General_Read_Response *response_ptr) {
    char * wheel_diameter;
    switch(response_ptr->wheel_diameter_inches) {
        case 0x0f:
        case 0x20:
            wheel_diameter = "16\"";
            break;
        case 0x21:
        case 0x22:
            wheel_diameter = "17\"";
            break;
        case 0x23:
        case 0x24:
            wheel_diameter = "18\"";
            break;
        case 0x25:
        case 0x26:
            wheel_diameter = "19\"";
            break;
        case 0x27:
        case 0x28:
            wheel_diameter = "20\"";
            break;
        case 0x29:
        case 0x2A:
            wheel_diameter = "21\"";
            break;
        case 0x2B:
        case 0x2C:
            wheel_diameter = "22\"";
            break;
        case 0x2D:
        case 0x2E:
            wheel_diameter = "23\"";
            break;
        case 0x2F:
        case 0x30:
            wheel_diameter = "24\"";
            break;
        case 0x31:
        case 0x32:
            wheel_diameter = "25\"";
            break;
        case 0x33:
        case 0x34:
            wheel_diameter = "26\"";
            break;
        case 0x35:
        case 0x36:
            wheel_diameter = "27\"";
            break;
        case 0x37:
            wheel_diameter = "700C";
            break;
        case 0x38:
            wheel_diameter = "28\"";
            break;
        case 0x39:
        case 0x3A:
            wheel_diameter = "29\"";
            break;
        case 0x3B:
        case 0x3C:
            wheel_diameter = "30\"";
            break;
        default:
            wheel_diameter = "Unknown";
    }

    char * speedmeter_model;
    // TODO: Take endianness into account
    switch(response_ptr->speedmeter_byte & 0b1100000) {
        case 0b00:
            speedmeter_model = "External";
            break;
        case 0b01:
            speedmeter_model = "Internal";
            break;
        case 0b10:
            speedmeter_model = "Monophase";
            break;
        default:
            speedmeter_model = "Unknown";
    }
    
    sprintf(render_buffer,
        "Low Voltage Cutoff: %dV\n"
        "Current Limit: %dA\n"
        "Assist Level 0 - Current Limit: %d%%\n"
        "Assist Level 1 - Current Limit: %d%%\n"
        "Assist Level 2 - Current Limit: %d%%\n"
        "Assist Level 3 - Current Limit: %d%%\n"
        "Assist Level 4 - Current Limit: %d%%\n"
        "Assist Level 5 - Current Limit: %d%%\n"
        "Assist Level 6 - Current Limit: %d%%\n"
        "Assist Level 7 - Current Limit: %d%%\n"
        "Assist Level 8 - Current Limit: %d%%\n"
        "Assist Level 9 - Current Limit: %d%%\n"
        "Assist Level 0 - Speed Limit: %d%%\n"
        "Assist Level 1 - Speed Limit: %d%%\n"
        "Assist Level 2 - Speed Limit: %d%%\n"
        "Assist Level 3 - Speed Limit: %d%%\n"
        "Assist Level 4 - Speed Limit: %d%%\n"
        "Assist Level 5 - Speed Limit: %d%%\n"
        "Assist Level 6 - Speed Limit: %d%%\n"
        "Assist Level 7 - Speed Limit: %d%%\n"
        "Assist Level 8 - Speed Limit: %d%%\n"
        "Assist Level 9 - Speed Limit: %d%%\n"
        "Wheel Diamater: %s\n"
        "Speedmeter Model: %s\n"
        "Speedmeter Signals: %02X\n",
        response_ptr->low_voltage_cutoff,
        response_ptr->current_limit,
        response_ptr->assist_0_current,
        response_ptr->assist_1_current,
        response_ptr->assist_2_current,
        response_ptr->assist_3_current,
        response_ptr->assist_4_current,
        response_ptr->assist_5_current,
        response_ptr->assist_6_current,
        response_ptr->assist_7_current,
        response_ptr->assist_8_current,
        response_ptr->assist_9_current,
        response_ptr->assist_0_speed,
        response_ptr->assist_1_speed,
        response_ptr->assist_2_speed,
        response_ptr->assist_3_speed,
        response_ptr->assist_4_speed,
        response_ptr->assist_5_speed,
        response_ptr->assist_6_speed,
        response_ptr->assist_7_speed,
        response_ptr->assist_8_speed,
        response_ptr->assist_9_speed,
        wheel_diameter,
        speedmeter_model,
        response_ptr->speedmeter_byte & 0b00111111 // Not sure if this is correct
    );
};

void render_throttle_info(char * render_buffer, Throttle_Read_Response *response_ptr) {
    char * mode;
    switch(response_ptr->mode) {
        case 0x00:
            mode = "Speed";
            break;
        case 0x01:
            mode = "current";
            break;
    }

    char speed_limit[20];
    if (response_ptr->speed_limit == 0xFF) {
        sprintf(speed_limit, "Use Display Setting");
    } else {
        sprintf(speed_limit, "%dkm/h", response_ptr->speed_limit);
    }

    sprintf(
        render_buffer,
        "Start Voltage: %dmV\n"
        "End Voltage: %dmV\n"
        "Mode: %s\n"
        "Assist Level: %d\n"
        "Speed Limit: %s\n"
        "Start Current: %d%%\n",
        response_ptr->start_voltage * 100,
        response_ptr->end_voltage * 100,
        mode,
        response_ptr->assist_level,
        speed_limit,
        response_ptr->start_current
    );
}