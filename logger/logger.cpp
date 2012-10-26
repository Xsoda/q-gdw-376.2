// -*- encoding: utf-8-unix; -*-
// File-name:    <logger.cpp>
// Author:       <Ð¡ËÕ´ò>
// Create:       <Thu Oct 25 16:40:12 2012>
// Time-stamp:   <Thursday October 25, 16:40:14 2012>

#include "logger.h"

#ifndef LOG_ONCE
Logger Logger_create()
{
    Logger l;
#else
static Logger l;
void Logger_create()
{

#endif
    l = (Logger) malloc(sizeof(Logger));
    l->datetime_format = "%Y-%m-%d %H:%M:%S";
    l->level = LOG_DEBUG;
#ifndef LOG_ONCE
    return(l);
#endif
}

void log_add(Logger la, int level, const char *msg)
{
    if (level < la->level) return;
    FILE *fp;
    fopen_s(&fp, "log.txt", "a");
    time_t meow = time(NULL);

    char buf[64];

    strftime(buf, sizeof(buf), la->datetime_format, localtime(&meow));
    fprintf(fp, "[%d] %c, %s : %s\n", (int)getpid(), LOG_LEVEL_CHARS[level], buf, msg);
    fclose(fp);
}

#ifndef LOG_ONCE
void log_debug(Logger l, const char *fmt, ...)
#else
void log_debug(const char *fmt, ...)
#endif
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf_s(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_DEBUG, msg);
    va_end(ap);
}

#ifndef LOG_ONCE
void log_info(Logger l, const char *fmt, ...)
#else
void log_info(const char *fmt, ...)
#endif
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf_s(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_INFO, msg);
    va_end(ap);
}

#ifndef LOG_ONCE
void log_warn(Logger l, const char *fmt, ...)
#else
void log_warn(const char *fmt, ...)
#endif
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf_s(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_WARN, msg);
    va_end(ap);
}

#ifndef LOG_ONCE
void log_error(Logger l, const char *fmt, ...)
#else
void log_error(const char *fmt, ...)
#endif
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf_s(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_ERROR, msg);
    va_end(ap);
}

#ifndef LOG_ONCE
void Logger_release(Logger l)
#else
void Logger_release()
#endif
{
    free(l);
}