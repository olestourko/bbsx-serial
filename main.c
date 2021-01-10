#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <errno.h>
#include "bbsx_serial.h"

int fd;
struct termios terminal_config;

void print_usage() {
    printf("Usage: bbsx-serial -d <device>\n");
    exit(2);
}

int main(int argc, char * argv[]) {
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