#include "XLog.h"
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <sys/time.h>
 
#ifdef XY_BUILD_FOR_ANDROID

#include <android/log.h>
#define LOG_TAG "XNetLog"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#endif

/* 开发者是否打开日志 */
volatile int develop_print_log;

/* 日志掩码 */
#ifndef XNET_DISABLE_LOGS
int LOG_MASK = 1 /*^ LOG_MASK_STATE*/;
#else
int LOG_MASK = 0;
#endif

static char buf[1024 * 512];

int TIME_STR(char *buf, int size)   // todo : use which buf
{
    timeval tv{};
    time_t ttt;

    time(&ttt);
    gettimeofday(&tv, nullptr);
    struct tm *tt = localtime(&ttt);
    return snprintf(buf, size, "[%04d-%02d-%02d %02d:%02d:%02d.%03d]",
                    tt->tm_year + 1900,
                    tt->tm_mon + 1,
                    tt->tm_mday,
                    tt->tm_hour, tt->tm_min, tt->tm_sec, tv.tv_usec / 1000);
}

int XNet_debug_log(const char *tag, const char */*file*/, int /*line*/, const char *format, ...)
{
    if (develop_print_log == 0 || develop_print_log > 1) {//when develop_print_log=1, start debug/stat/war/err log
        return 0;
    }

    va_list  args;
    
    int len = TIME_STR(buf, sizeof(buf));
    len += snprintf(buf + len, sizeof(buf) - len, "[%s]", tag);

    va_start(args, format);
    len += vsnprintf(buf + len, sizeof(buf) - len, format, args);
    va_end(args);

    //len += snprintf(buf + len, sizeof(buf) - len, " <%s:%d>\n", file, line);
    printf("%s\n", buf);

#ifdef XY_BUILD_FOR_WINDOWS
    snprintf(buf + len, sizeof(buf) - len, "\n");
    OutputDebugStringA(buf);
#endif

#ifdef XY_BUILD_FOR_ANDROID
    LOGI("%s", buf);
#endif

    return 0;
}

int XNet_stat_log(const char *tag, const char */*file*/, int /*line*/, const char *format, ...)
{
    if (develop_print_log == 0 || develop_print_log > 2) {//when develop_print_log=2, start stat/war/err log
        return 0;
    }

    va_list  args;

    int len = TIME_STR(buf, sizeof(buf));
    len += snprintf(buf + len, sizeof(buf) - len, "[%s]", tag);

    va_start(args, format);
    len += vsnprintf(buf + len, sizeof(buf) - len, format, args);
    va_end(args);

    //len += snprintf(buf + len, sizeof(buf) - len, " <%s:%d>\n", file, line);
    printf("%s\n", buf);

#ifdef XY_BUILD_FOR_WINDOWS
    snprintf(buf + len, sizeof(buf) - len, "\n");
    OutputDebugStringA(buf);
#endif

#ifdef XY_BUILD_FOR_ANDROID
    LOGI("%s", buf);
#endif

    return 0;
}

int XNet_war_log(const char *tag,const char */*file*/, int /*line*/, const char *format, ...)
{
    if (develop_print_log == 0 || develop_print_log > 3) {//when develop_print_log=3, start war/err log
        return 0;
    }

    va_list  args;

    int len = TIME_STR(buf, sizeof(buf));
    len += snprintf(buf + len, sizeof(buf) - len, "[%s]", tag);

    va_start(args, format);
    len += vsnprintf(buf + len, sizeof(buf) - len, format, args);
    va_end(args);

    //len += snprintf(buf + len, sizeof(buf) - len, " <%s:%d>\n", file, line);
    printf("%s\n", buf);

#ifdef XY_BUILD_FOR_WINDOWS
    snprintf(buf + len, sizeof(buf) - len, "\n");
    OutputDebugStringA(buf);
#endif

#ifdef XY_BUILD_FOR_ANDROID
    LOGI("%s", buf);
#endif

    return 0;
}

int XNet_err_log(const char *tag,const char */*file*/, int /*line*/, const char *format, ...)
{
    if (develop_print_log == 0 || develop_print_log > 4) {//when develop_print_log=4, start err log
        return 0;
    }

    va_list  args;

    int len = TIME_STR(buf, sizeof(buf));
    len += snprintf(buf + len, sizeof(buf) - len, "[%s]", tag);

    va_start(args, format);
    len += vsnprintf(buf + len, sizeof(buf) - len, format, args);
    va_end(args);

    //len += snprintf(buf + len, sizeof(buf) - len, " <%s:%d>\n", file, line);
    printf("%s\n", buf);

#ifdef XNET_BUILD_FOR_WINDOWS
    snprintf(buf + len, sizeof(buf) - len, "\n");
    OutputDebugStringA(buf);
#endif

#ifdef XNET_BUILD_FOR_ANDROID
    LOGI("%s", buf);
#endif

    return 0;
}


