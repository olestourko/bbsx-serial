#ifndef BBSX_SERIAL_HEADER
#define BBSX_SERIAL_HEADER

#define VOLTAGE_24V 0x00
#define VOLTAGE_36V 0x01
#define VOLTAGE_48V 0x02
#define VOLTAGE_60V 0x03
#define VOLTAGE_24Vto48V 0x04

// Requests
extern char * send_basic_read_request();
extern char * send_general_read_request();
extern char * send_pedal_read_request();
extern char * send_throttle_read_request();

// Responses
struct Connect_Response {
    char manufacturer[5];
    char model[5];
    char hardware_version[4];
    char firmware_version[8];
    int max_current;
    char voltage;
};
typedef struct Connect_Response Connect_Response;

extern Connect_Response * parse_connect_response(char * response_body);

// Rendering
extern void render_general_info(char * render_buffer, Connect_Response *response_ptr);

#endif