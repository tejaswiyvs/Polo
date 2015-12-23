#ifndef __log_h
#define __log_h

#define loge(message) log("ERR", message);
#define logv(message) log("VRB", message);
#define logi(message) log("INFO", message);

void log(char *level, char *message);

#endif
