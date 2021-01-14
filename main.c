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

void read_config_file() {
    /* Open config file */
    config_t cfg;
    config_setting_t *setting;
    config_init(&cfg);
    if (!config_read_file(&cfg, "config.cfg")) {
        fprintf(stdout, "%s:%d - %s\n", config_error_file(&cfg),
        config_error_line(&cfg), config_error_text(&cfg));
    }

    int r;
    int int_value;
    const char *str_value;
    const char * key;
    /* Read throttle config */
    Throttle_Read_Response throttle_data;
    key = "throttle.start_voltage";
    if (r = config_lookup_int(&cfg, key, &int_value)) {
        throttle_data.start_voltage = int_value;
    }
    key = "throttle.end_voltage";
    if (r = config_lookup_int(&cfg, key, &int_value)) {
        throttle_data.end_voltage = int_value;
    }
    key = "throttle.mode";
    if (r = config_lookup_string(&cfg, key, &str_value)) {
        if (strcmp(str_value, "speed") == 0) {
            throttle_data.mode = 0x00;
        } else if (strcmp(str_value, "current") == 0) {
            throttle_data.mode = 0x01;
        }
    }
    key = "throttle.assist_level";
    if (r = config_lookup_int(&cfg, key, &int_value)) {
        throttle_data.assist_level = int_value;
    }
    key = "throttle.speed_limit";
    if (r = config_lookup_int(&cfg, key, &int_value)) {
        throttle_data.speed_limit = int_value;
    } else if (r = config_lookup_string(&cfg, key, &str_value)) {
        if (strcmp(str_value, "by_display") == 0) {
            throttle_data.speed_limit = 0xff;
        }
    }

    key = "throttle.start_current";
    if (r = config_lookup_int(&cfg, key, &int_value)) {
        throttle_data.start_current = int_value;
    }
    char render_buffer[1024];
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