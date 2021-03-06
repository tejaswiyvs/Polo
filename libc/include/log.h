#ifndef __log_h
#define __log_h

#include <stdint.h>

#define loge(message) log("ERR", __FILE__, __LINE__, message);
#define logv(message) log("VRB", __FILE__, __LINE__, message);
#define logi(message) log("INFO", __FILE__, __LINE__, message);

#define LOG_OPAQUE_STRUCT(p)  print_mem((p), sizeof(*(p)))

void log(char *level, char *file, int line, char *message);
void log_asm();
void print_mem(void const *vp, size_t n);

#endif
