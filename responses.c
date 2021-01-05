#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bbsx_serial.h"

Connect_Response * parse_connect_response(char * response_body) {
    Connect_Response *response = (Connect_Response *) malloc(sizeof(Connect_Response));
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