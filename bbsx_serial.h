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
struct Basic_Read_Response {
    char manufacturer[5];
    char model[5];
    char hardware_version[4];
    char firmware_version[8];
    char max_current;
    char voltage;
};
struct General_Read_Response {
    char low_voltage_cutoff;
    char current_limit;
    char assist_0_current;
    char assist_1_current;
    char assist_2_current;
    char assist_3_current;
    char assist_4_current;
    char assist_5_current;
    char assist_6_current;
    char assist_7_current;
    char assist_8_current;
    char assist_9_current;
    char assist_0_speed;
    char assist_1_speed;
    char assist_2_speed;
    char assist_3_speed;
    char assist_4_speed;
    char assist_5_speed;
    char assist_6_speed;
    char assist_7_speed;
    char assist_8_speed;
    char assist_9_speed;
    char wheel_diameter_inches;
    char speedmeter_byte;
};
struct Pedal_Read_Response {
    char pedal_type;
    char assist_level;
    char speed_limit;
    char start_current;
    char slow_start_mode;
    char start_degree;
    char work_mode;
    char stop_delay;
    char current_decay;
    char stop_decay;
    char keep_current;
};
struct Throttle_Read_Response {
    char start_voltage;
    char end_voltage;
    char mode;
    char assist_level;
    char speed_limit;
    char start_current;
};

typedef struct Basic_Read_Response Basic_Read_Response;
typedef struct General_Read_Response General_Read_Response;
typedef struct Pedal_Read_Response Pedal_Read_Response;
typedef struct Throttle_Read_Response Throttle_Read_Response;
extern Basic_Read_Response * parse_basic_read_response(char * response_body);
extern General_Read_Response * parse_general_read_response(char * response_body);
extern Pedal_Read_Response * parse_pedal_read_response(char * response_body);
extern Throttle_Read_Response * parse_throttle_read_response(char * response_body);

// Rendering
extern void render_basic_info(char * render_buffer, Basic_Read_Response *response_ptr);
extern void render_general_info(char * render_buffer, General_Read_Response *response_ptr);
extern void render_throttle_info(char * render_buffer, Throttle_Read_Response *response_ptr);

#endif