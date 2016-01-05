#ifndef __log_h
#define __log_h

#define loge(message) log("ERR", __FILE__, __LINE__, message);
#define logv(message) log("VRB", __FILE__, __LINE__, message);
#define logi(message) log("INFO", __FILE__, __LINE__, message);

void log(char *level, char *file, int line, char *message);

#endif
