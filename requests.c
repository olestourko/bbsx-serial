#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include "bbsx_serial.h"

extern int fd;
extern struct termios terminal_config;
const int RESPONSE_HEADER_BYTESIZE = 2;
const char connect_command[] = {0x11, 0x51, 0x04, 0xB0, 0x05};

char * send_basic_read_request(char * read_buffer) {
    terminal_config.c_cc[VMIN] = 2;
    tcsetattr(fd, TCSANOW, &terminal_config);

    write(fd, connect_command, sizeof(connect_command));
    read_buffer = realloc(read_buffer, sizeof(char) * 2);

    // Read response header
    terminal_config.c_cc[VMIN] = RESPONSE_HEADER_BYTESIZE;
    tcsetattr(fd, TCSANOW, &terminal_config);
    read(fd, read_buffer, sizeof(char) * 2);
    int response_length = read_buffer[1] + 1; // +1 for end checksum bit not included in length count?

    // Read response body
    terminal_config.c_cc[VMIN] = response_length;
    tcsetattr(fd, TCSANOW, &terminal_config);
    read_buffer = realloc(read_buffer, sizeof(char) * response_length);
    read(fd, read_buffer, response_length * sizeof(char));

    return read_buffer;
}