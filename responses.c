#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bbsx_serial.h"

Basic_Read_Response * parse_basic_read_response(char * response_body) {
    Basic_Read_Response *response = (Basic_Read_Response *) malloc(sizeof(Basic_Read_Response));
    strncpy(response->manufacturer, response_body, 4);
    response->manufacturer[4] = '\0';
    strncpy(response->model, response_body + 4, 4);
    response->model[4] = '\0';
    sprintf(response->hardware_version, "%c.%c", response_body[8], response_body[9]);
    sprintf(response->firmware_version, "%c.%c.%c.%c", response_body[10], response_body[11], response_body[12], response_body[13]);
    response->voltage = response_body[14];
    response->max_current = response_body[15];

    return response;
}

General_Read_Response * parse_general_read_response(char * response_body) {
    General_Read_Response *response = (General_Read_Response *) malloc(sizeof(General_Read_Response));
    response->low_voltage_cutoff = response_body[0];
    response->current_limit = response_body[1];
    response->assist_0_current = response_body[2];
    response->assist_1_current = response_body[3];
    response->assist_2_current = response_body[4];
    response->assist_3_current = response_body[5];
    response->assist_4_current = response_body[6];
    response->assist_5_current = response_body[7];
    response->assist_6_current = response_body[8];
    response->assist_7_current = response_body[9];
    response->assist_8_current = response_body[10];
    response->assist_9_current = response_body[11];
    response->assist_0_speed = response_body[12];
    response->assist_1_speed = response_body[13];
    response->assist_2_speed = response_body[14];
    response->assist_3_speed = response_body[15];
    response->assist_4_speed = response_body[16];
    response->assist_5_speed = response_body[17];
    response->assist_6_speed = response_body[18];
    response->assist_7_speed = response_body[19];
    response->assist_8_speed = response_body[20];
    response->assist_9_speed = response_body[21];
    response->wheel_diameter_inches = response_body[22];
    response->speedmeter_byte = response_body[23];

    return response;
}

Pedal_Read_Response * parse_pedal_read_response(char * response_body) {
    Pedal_Read_Response *response = (Pedal_Read_Response *) malloc(sizeof(Pedal_Read_Response));
    response->pedal_type = response_body[0];
    response->assist_level = response_body[1];
    response->speed_limit = response_body[2];
    response->start_current = response_body[3];
    response->slow_start_mode = response_body[4];
    response->start_degree = response_body[5];
    response->work_mode = response_body[6];
    response->stop_delay = response_body[7];
    response->stop_decay = response_body[8];
    response->keep_current = response_body[9];
}

Throttle_Read_Response * parse_throttle_read_response(char * response_body) {
    Throttle_Read_Response *response = (Throttle_Read_Response *) malloc(sizeof(Throttle_Read_Response));
    response->start_voltage = response_body[0];
    response->end_voltage = response_body[1];
    response->mode = response_body[2];
    response->assist_level = response_body[3];
    response->speed_limit = response_body[4];
    response->start_current = response_body[5];
}