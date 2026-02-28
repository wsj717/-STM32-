#ifndef __LOG_H
#define __LOG_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* 配置宏定义 */
#define CONFIG_ULOG_BUF_SIZE    256u
#define CONFIG_ULOG_DEF_LEVEL   LOG_LEVEL_TRACE
#define ULOG_NEWLINE_SIGN       "\r\n"

/* 日志级别枚举 */
typedef enum {
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_MAX
} log_level_t;

/* 格式控制宏 */
#define LOG_FMT_RAW           (0u)
#define LOG_FMT_LEVEL_STR     (1u << 0)
#define LOG_FMT_TIME_STAMP    (1u << 1)
#define LOG_FMT_FUNC_LINE     (1u << 2)
#define LOG_FMT_FILE_LINE     (1u << 3)

/* 启用日志级别开关 */
#define LOG_TRACE_EN  1
#define LOG_DEBUG_EN  1
#define LOG_INFO_EN   1
#define LOG_WARN_EN   1
#define LOG_ERROR_EN  1
#define LOG_FATAL_EN  1

/* 日志输出函数类型 */
typedef void (*UlogOutputFunc)(uint8_t *data, uint16_t size);

/* 接口函数 */
void Ulog_RegisterOutput(UlogOutputFunc func);
void Ulog_SetFmt(uint32_t fmt);
void Ulog_SetLevel(uint32_t level);

void Ulog(uint32_t level,
          const char *file,
          const char *func,
          uint32_t line,
          const char *fmt, ...);

/* 日志宏定义 */

#if LOG_TRACE_EN
#define LOG_TRACE(fmt, ...) \
    Ulog(LOG_LEVEL_TRACE, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_TRACE(fmt, ...)
#endif

#if LOG_DEBUG_EN
#define LOG_DEBUG(fmt, ...) \
    Ulog(LOG_LEVEL_DEBUG, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#endif

#if LOG_INFO_EN
#define LOG_INFO(fmt, ...) \
    Ulog(LOG_LEVEL_INFO, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...)
#endif

#if LOG_WARN_EN
#define LOG_WARN(fmt, ...) \
    Ulog(LOG_LEVEL_WARN, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_WARN(fmt, ...)
#endif

#if LOG_ERROR_EN
#define LOG_ERROR(fmt, ...) \
    Ulog(LOG_LEVEL_ERROR, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...)
#endif

#if LOG_FATAL_EN
#define LOG_FATAL(fmt, ...) \
    Ulog(LOG_LEVEL_FATAL, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define LOG_FATAL(fmt, ...)
#endif

#endif /* __LOG_H */