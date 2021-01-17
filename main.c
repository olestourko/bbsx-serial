#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libconfig.h>
#include "bbsx_serial.h"

int fd;
struct termios terminal_config;

int config_lookup_uchar(config_t *cfg, char *key, unsigned char *address) {
    /*  Wrapper for libconfig's config_lookup_int. Stores the value in an unsigned char address.
    *   Prevents compiler warnings.
    */
    int v;
    int r = config_lookup_int(cfg, key, &v);
    if (r) {
        *address = v;
    }
    return r;
}

void read_config_file() {
    /* Open config file */
    config_t cfg;
    config_init(&cfg);
    if (!config_read_file(&cfg, "config.cfg")) {
        fprintf(stdout, "%s:%d - %s\n", config_error_file(&cfg),
        config_error_line(&cfg), config_error_text(&cfg));
    }

    int r;
    int int_value;
    const char *str_value;
    char render_buffer[1024];

    /* Read Basic Config */
    General_Read_Response general_data;
    config_lookup_uchar(&cfg, "general.low_voltage_cutoff", &general_data.low_voltage_cutoff);
    config_lookup_uchar(&cfg, "general.current_limit", &general_data.current_limit);
    config_lookup_uchar(&cfg, "general.assist_levels.level_0.speed_limit", &general_data.assist_0_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_0.current_limit", &general_data.assist_0_current);
    config_lookup_uchar(&cfg, "general.assist_levels.level_1.speed_limit", &general_data.assist_1_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_1.current_limit", &general_data.assist_1_current);
    config_lookup_uchar(&cfg, "general.assist_levels.level_2.speed_limit", &general_data.assist_2_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_2.current_limit", &general_data.assist_2_current);
    config_lookup_uchar(&cfg, "general.assist_levels.level_3.speed_limit", &general_data.assist_3_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_3.current_limit", &general_data.assist_3_current);
    config_lookup_uchar(&cfg, "general.assist_levels.level_4.speed_limit", &general_data.assist_4_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_4.current_limit", &general_data.assist_4_current);
    config_lookup_uchar(&cfg, "general.assist_levels.level_5.speed_limit", &general_data.assist_5_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_5.current_limit", &general_data.assist_5_current);
    config_lookup_uchar(&cfg, "general.assist_levels.level_6.speed_limit", &general_data.assist_6_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_6.current_limit", &general_data.assist_6_current);
    config_lookup_uchar(&cfg, "general.assist_levels.level_7.speed_limit", &general_data.assist_7_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_7.current_limit", &general_data.assist_7_current);
    config_lookup_uchar(&cfg, "general.assist_levels.level_8.speed_limit", &general_data.assist_8_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_8.current_limit", &general_data.assist_8_current);
    config_lookup_uchar(&cfg, "general.assist_levels.level_9.speed_limit", &general_data.assist_9_speed);
    config_lookup_uchar(&cfg, "general.assist_levels.level_9.current_limit", &general_data.assist_9_current);
    if (r = config_lookup_int(&cfg, "general.wheel_diameter", &int_value)) {
        general_data.wheel_diameter_inches = int_value + 0x19;
    } else if (r = config_lookup_string(&cfg, "general.wheel_diameter", &str_value)) {
        if (strcmp(str_value, "700C") == 0) {
            general_data.wheel_diameter_inches = 0x37;
        }
    }
    if (r = config_lookup_string(&cfg, "general.speedmeter", &str_value)) {
        if (strcmp(str_value, "external") == 0) {
            general_data.speedmeter_byte = 0b00;
        } else if (strcmp(str_value, "internal") == 0) {
            general_data.speedmeter_byte = 0b01;
        } else if (strcmp(str_value, "motorphase") == 0) {
            general_data.speedmeter_byte = 0b10;
        }
    }
    // TODO: Speedmeter signals
    render_general_info(render_buffer, &general_data);
    printf("\n--- General Settings (Config File) ---------\n%s", render_buffer);

    /* Read Pedal Config */
    Pedal_Read_Response pedal_data;
    if (r = config_lookup_string(&cfg, "pedal.pedal_type", &str_value)) {
        if (strcmp(str_value, "None")) {
            pedal_data.pedal_type = 0x00;
        } else if (strcmp(str_value, "DH-Sensor-12")) {
            pedal_data.pedal_type = 0x01;
        } else if (strcmp(str_value, "BB-Sensor-32")) {
            pedal_data.pedal_type = 0x02;
        } else if (strcmp(str_value, "DoubleSignal-24")) {
            pedal_data.pedal_type = 0x03;
        }
    }
    if (!config_lookup_uchar(&cfg, "pedal.assist_level", &pedal_data.assist_level)
    && config_lookup_string(&cfg, "pedal.assist_level", &str_value)) {
        if (strcmp(str_value, "by_display") == 0) {
            pedal_data.assist_level = 0xff;
        }
    }
    if (!config_lookup_uchar(&cfg, "pedal.speed_limit", &pedal_data.speed_limit)
    && config_lookup_string(&cfg, "pedal.speed_limit", &str_value)) {
        if (strcmp(str_value, "by_display") == 0) {
            pedal_data.speed_limit = 0xff;
        }
    }
    config_lookup_uchar(&cfg, "pedal.start_current", &pedal_data.start_current);
    config_lookup_uchar(&cfg, "pedal.slow_start_mode", &pedal_data.slow_start_mode);
    config_lookup_uchar(&cfg, "pedal.start_degree", &pedal_data.start_degree);
    config_lookup_uchar(&cfg, "pedal.work_mode", &pedal_data.work_mode);
    config_lookup_uchar(&cfg, "pedal.stop_delay", &pedal_data.stop_delay);
    config_lookup_uchar(&cfg, "pedal.current_decay", &pedal_data.current_decay);
    config_lookup_uchar(&cfg, "pedal.stop_decay", &pedal_data.stop_decay);
    config_lookup_uchar(&cfg, "pedal.keep_current", &pedal_data.keep_current);
    render_pedal_info(render_buffer, &pedal_data);
    printf("\n--- Pedal Settings (Config File) ---------\n%s", render_buffer);

    /* Read Throttle Config */
    Throttle_Read_Response throttle_data;
    config_lookup_uchar(&cfg, "throttle.start_voltage", &throttle_data.start_voltage);
    config_lookup_uchar(&cfg, "throttle.end_voltage", &throttle_data.end_voltage);
    if (r = config_lookup_string(&cfg, "throttle.mode", &str_value)) {
        if (strcmp(str_value, "speed") == 0) {
            throttle_data.mode = 0x00;
        } else if (strcmp(str_value, "current") == 0) {
            throttle_data.mode = 0x01;
        }
    }
    config_lookup_uchar(&cfg, "throttle.assist_level", &throttle_data.assist_level);
    if (r = config_lookup_int(&cfg, "throttle.speed_limit", &int_value)) {
        throttle_data.speed_limit = int_value;
    } else if (r = config_lookup_string(&cfg, "throttle.speed_limit", &str_value)) {
        if (strcmp(str_value, "by_display") == 0) {
            throttle_data.speed_limit = 0xff;
        }
    }
    config_lookup_uchar(&cfg, "throttle.start_current", &throttle_data.start_current);
    render_throttle_info(render_buffer, &throttle_data);
    printf("\n--- Throttle Settings (Config File) ---------\n%s", render_buffer);
}

void print_usage() {
    printf("Usage: bbsx-serial -d <device>\n");
    exit(2);
}

int main(int argc, char * argv[]) {
    /* Read terminal args */
    int option;
    char * device = NULL;
    while ((option = getopt(argc, argv, "d:")) !=-1) {
        switch (option) {
            case 'd':
                device = optarg;
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                exit(2);
        }
    }

    if (device == NULL) {
        print_usage();
    }

    read_config_file();

    // https://www.cmrr.umn.edu/~strupp/serial.html
    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        printf("Error %d opening %s\n", errno, device);
        exit(2);
    }

    fcntl(fd, F_SETFL, 0);

    tcgetattr(fd, &terminal_config);
    cfsetospeed(&terminal_config, B1200); // TODO: Error handling
    cfsetispeed(&terminal_config, B1200); // TODO: Error handling
    cfmakeraw(&terminal_config); // Set to pass raw bytes
    tcsetattr(fd, TCSANOW, &terminal_config);

    char * read_buffer = NULL;
    char render_buffer[1024];

    /* Basic Read */
    read_buffer = send_basic_read_request(read_buffer);
    Basic_Read_Response *basic_read_response = parse_basic_read_response(read_buffer);
    render_basic_info(render_buffer, basic_read_response);
    free(basic_read_response);
    printf("--- Controller Info ---------\n%s", render_buffer);

    /* General Read */
    read_buffer = send_general_read_request(read_buffer);
    General_Read_Response *general_read_response = parse_general_read_response(read_buffer);
    render_general_info(render_buffer, general_read_response);
    free(general_read_response);
    printf("\n--- General Settings ---------\n%s", render_buffer);

    /* Pedal Read */
    read_buffer = send_pedal_read_request(read_buffer);
    Pedal_Read_Response *pedal_read_response = parse_pedal_read_response(read_buffer);
    render_pedal_info(render_buffer, pedal_read_response);
    free(pedal_read_response);
    printf("\n--- Pedal Settings ---------\n%s", render_buffer);

    /* Throttle Read */
    read_buffer = send_throttle_read_request(read_buffer);
    Throttle_Read_Response *throttle_read_response = parse_throttle_read_response(read_buffer);
    render_throttle_info(render_buffer, throttle_read_response);
    free(throttle_read_response);
    printf("\n--- Throttle Settings ---------\n%s", render_buffer);

    free(read_buffer);
    close(fd);
}