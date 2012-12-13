/* -*- encoding: utf-8-unix; -*- */
/* File-name:    <logger.h> */
/* Author:       <Ð¡ËÕ´ò> */
/* Create:       <Thu Oct 25 16:40:02 2012> */
/* Time-stamp:   <Thursday October 25, 16:40:3 2012> */
/* Mail:         <Xsoda@Live.com> */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <process.h>

#ifndef _LOGGER_H_
#define _LOGGER_H_

#if defined __cplusplus
extern "C" {
#endif

#define LOG_DEBUG 0
#define LOG_INFO  1
#define LOG_WARN  2
#define LOG_ERROR 3

static char * LOG_LEVEL_CHARS[] = {"DEBUG", " INFO", " WARN", "ERROR", "FAULT"};
#define LOG_MAX_MSG_LEN 1024

typedef struct LOGGER {
    int level;
    char *datetime_format;
    char *device_path;
} LOGGER;

typedef LOGGER* Logger;

Logger Logger_create(const char *device_path);
void Logger_release(Logger l);
void log_add(Logger l, int level, const char *msg);
void log_debug(Logger l, const char *fmt, ...);
void log_info(Logger l, const char *fmt, ...);
void log_warn(Logger l, const char *fmt, ...);
void log_error(Logger l, const char *fmt, ...);

#if defined __cplusplus
}
#endif

#endif
