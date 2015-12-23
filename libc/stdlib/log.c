#include <log.h>
#include <serial.h>

void log(char *level, char *message) {
  write_string_serial(level);
  write_string_serial("\t");
  write_string_serial(message);
  write_string_serial("\n");
}
