#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include "bbsx_serial.h"

int fd;
struct termios terminal_config;

int main(int argc, char * argv[]) {
    // https://www.cmrr.umn.edu/~strupp/serial.html

    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    fcntl(fd, F_SETFL, 0);

    tcgetattr(fd, &terminal_config);
    cfsetospeed(&terminal_config, B1200); // TODO: Error handling
    cfsetispeed(&terminal_config, B1200); // TODO: Error handling
    cfmakeraw(&terminal_config); // Set to pass raw bytes

    char * read_buffer;
    read_buffer = send_basic_read_request(read_buffer);

    Connect_Response *connect_response = parse_connect_response(read_buffer);
    char render_buffer[256];
    render_general_info(render_buffer, connect_response);

    // Print General
    printf("%s", render_buffer);
    
    free(read_buffer);
    close(fd);
}