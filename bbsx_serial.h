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
    unsigned char manufacturer[5];
    unsigned char model[5];
    unsigned char hardware_version[4];
    unsigned char firmware_version[8];
    unsigned char max_current;
    unsigned char voltage;
};
struct General_Read_Response {
    unsigned char low_voltage_cutoff;
    unsigned char current_limit;
    unsigned char assist_0_current;
    unsigned char assist_1_current;
    unsigned char assist_2_current;
    unsigned char assist_3_current;
    unsigned char assist_4_current;
    unsigned char assist_5_current;
    unsigned char assist_6_current;
    unsigned char assist_7_current;
    unsigned char assist_8_current;
    unsigned char assist_9_current;
    unsigned char assist_0_speed;
    unsigned char assist_1_speed;
    unsigned char assist_2_speed;
    unsigned char assist_3_speed;
    unsigned char assist_4_speed;
    unsigned char assist_5_speed;
    unsigned char assist_6_speed;
    unsigned char assist_7_speed;
    unsigned char assist_8_speed;
    unsigned char assist_9_speed;
    unsigned char wheel_diameter_inches;
    unsigned char speedmeter_byte;
};
struct Pedal_Read_Response {
    unsigned char pedal_type;
    unsigned char assist_level;
    unsigned char speed_limit;
    unsigned char start_current;
    unsigned char slow_start_mode;
    unsigned char start_degree;
    unsigned char work_mode;
    unsigned char stop_delay;
    unsigned char current_decay;
    unsigned char stop_decay;
    unsigned char keep_current;
};
struct Throttle_Read_Response {
    unsigned char start_voltage;
    unsigned char end_voltage;
    unsigned char mode;
    unsigned char assist_level;
    unsigned char speed_limit;
    unsigned char start_current;
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
extern void render_pedal_info(char * render_buffer, Pedal_Read_Response *response_ptr);
extern void render_throttle_info(char * render_buffer, Throttle_Read_Response *response_ptr);

#endif