#include <log.h>
#include <serial.h>

void print_mem(void const *vp, size_t n)
{
    unsigned char const *p = vp;
    for (size_t i=0; i<n; i++)
        write_string_serial("%02x\n", p[i]);
    putchar('\n');
}

void log(char *level, char *file, int line, char *message) {
  write_string_serial(level);
  write_string_serial("\t");
  write_string_serial(file);
  write_string_serial("\t");
  write_string_serial(itoa(line));
  write_string_serial("\t");
  write_string_serial(message);
  write_string_serial("\n");
}

void log_asm() {
  write_string_serial("[ASM]\t");
  write_string_serial("\n");
}
