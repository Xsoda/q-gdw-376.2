// -*- encoding: utf-8-unix; -*-
// File-name:    <logger.cpp>
// Author:       <Ð¡ËÕ´ò>
// Create:       <Thu Oct 25 16:40:12 2012>
// Time-stamp:   <Thursday October 25, 16:40:14 2012>

#include "logger.h"


Logger Logger_create(const char *device_path)
{
    Logger l;
    l = (Logger) malloc(sizeof(Logger));
	if (device_path)
	{
		l->device_path = strdup(device_path);
	}
    l->datetime_format = "%Y-%m-%d %H:%M:%S";
    l->level = LOG_DEBUG;
    return(l);

}

void log_add(Logger l, int level, const char *msg)
{
    if (level < l->level) return;
    FILE *fp;
    fopen_s(&fp, l->device_path, "a");
    time_t meow = time(NULL);

    char buf[64];

    strftime(buf, sizeof(buf), l->datetime_format, localtime(&meow));
    fprintf(fp, "[%d %s] %s : %s\n", (int)_getpid(), LOG_LEVEL_CHARS[level], buf, msg);
    fclose(fp);
}

void log_debug(Logger l, const char *fmt, ...)
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf_s(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_DEBUG, msg);
    va_end(ap);
}


void log_info(Logger l, const char *fmt, ...)
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf_s(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_INFO, msg);
    va_end(ap);
}

void log_warn(Logger l, const char *fmt, ...)
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf_s(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_WARN, msg);
    va_end(ap);
}

void log_error(Logger l, const char *fmt, ...)
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf_s(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_ERROR, msg);
    va_end(ap);
}

void Logger_release(Logger l)
{
	free(l->device_path);
    free(l);
}