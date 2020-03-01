#ifndef _XNet_LOG_H
#define _XNet_LOG_H
#include <cstring>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DBG_LOG(format,  ...) XNet_debug_log("DEBUG", __FILENAME__, __LINE__, format, ##__VA_ARGS__)
#define STAT_LOG(format, ...) XNet_stat_log("STAT", __FILENAME__, __LINE__, format, ##__VA_ARGS__)
#define WAR_LOG(format,  ...) XNet_war_log("WARN", __FILENAME__, __LINE__, format, ##__VA_ARGS__)
#define ERR_LOG(format,  ...) XNet_err_log("ERROR", __FILENAME__, __LINE__, format, ##__VA_ARGS__)

int XNet_debug_log(const char *tag, const char *file, int line, const char *format, ...);
int XNet_stat_log(const char *tag, const char *file, int line, const char *format, ...);
int XNet_war_log(const char *tag, const char *file, int line, const char *format, ...);
int XNet_err_log(const char *tag, const char *file, int line, const char *format, ...);

#endif /* _XNet_LOG_H */
