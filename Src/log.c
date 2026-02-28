#include "log.h"
#include "usart.h"

/* 全局配置 */
static uint32_t s_ulog_fmt =
    LOG_FMT_LEVEL_STR |
    LOG_FMT_TIME_STAMP |
    LOG_FMT_FILE_LINE |
    LOG_FMT_FUNC_LINE;

static uint32_t s_ulog_level = CONFIG_ULOG_DEF_LEVEL;
static UlogOutputFunc ulog_output = NULL;

/* 获取系统时间 */
static void Get_SystemTime(char *time_buf, uint16_t buf_size, uint16_t *ms)
{
    uint32_t tick = HAL_GetTick();
    *ms = tick % 1000;

    uint32_t sec = tick / 1000;
    uint32_t hour = sec / 3600;
    uint32_t min = (sec % 3600) / 60;
    sec = sec % 60;

    snprintf(time_buf, buf_size, "%02d:%02d:%02d",
             (int)(hour % 24),
             (int)min,
             (int)sec);
}

/* 获取短文件名（去路径） */
static const char* GetShortFileName(const char* path)
{
    const char *p = strrchr(path, '/');
    if (p) return p + 1;

    p = strrchr(path, '\\');
    if (p) return p + 1;

    return path;
}

/* 注册输出函数 */
void Ulog_RegisterOutput(UlogOutputFunc func)
{
    ulog_output = func ? func : Uart_SendData;
}

/* 设置日志格式 */
void Ulog_SetFmt(uint32_t fmt)
{
    s_ulog_fmt = fmt;
}

/* 设置日志等级 */
void Ulog_SetLevel(uint32_t level)
{
    if (level < LOG_LEVEL_MAX)
        s_ulog_level = level;
}

/* 核心日志函数 */
void Ulog(uint32_t level,
          const char *file,
          const char *func,
          uint32_t line,
          const char *fmt, ...)
{
    if (level >= LOG_LEVEL_MAX || level < s_ulog_level)
        return;

    if (ulog_output == NULL)
        Ulog_RegisterOutput(NULL);

    char log_buf[CONFIG_ULOG_BUF_SIZE] = {0};
    int idx = 0;
    va_list args;

    /* 日志级别 */
    if (s_ulog_fmt & LOG_FMT_LEVEL_STR) {
        static const char *level_str[] = {
            "TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL"
        };
        idx += snprintf(log_buf + idx,
                        sizeof(log_buf) - idx,
                        "[%s] ",
                        level_str[level]);
    }

    /* 时间戳 */
    if (s_ulog_fmt & LOG_FMT_TIME_STAMP) {
        char time_buf[32];
        uint16_t ms = 0;
        Get_SystemTime(time_buf, sizeof(time_buf), &ms);

        idx += snprintf(log_buf + idx,
                        sizeof(log_buf) - idx,
                        "[%s.%03d] ",
                        time_buf,
                        ms);
    }

    /* 文件名 + 行号 */
    if (s_ulog_fmt & LOG_FMT_FILE_LINE) {
        const char *short_file = GetShortFileName(file);
        idx += snprintf(log_buf + idx,
                        sizeof(log_buf) - idx,
                        "[%s:%d] ",
                        short_file,
                        (int)line);
    }

    /* 函数名 */
    if (s_ulog_fmt & LOG_FMT_FUNC_LINE) {
        idx += snprintf(log_buf + idx,
                        sizeof(log_buf) - idx,
                        "[%s] ",
                        func);
    }

    /* 日志内容 */
    va_start(args, fmt);
    int len = vsnprintf(log_buf + idx,
                        sizeof(log_buf) - idx,
                        fmt,
                        args);
    va_end(args);

    if (len > 0)
        idx += len;

    /* 添加换行 */
    if (idx < CONFIG_ULOG_BUF_SIZE - 2) {
        snprintf(log_buf + idx,
                 sizeof(log_buf) - idx,
                 "%s",
                 ULOG_NEWLINE_SIGN);
        idx += strlen(ULOG_NEWLINE_SIGN);
    }

    /* 输出 */
    ulog_output((uint8_t *)log_buf, (uint16_t)idx);
}