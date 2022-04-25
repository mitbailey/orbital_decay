/**
 * @file meb_print.h
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Contains debug-related macros and function-like macros.
 * @version See Git tags for version information.
 * @date 2021.07.26
 *
 * With revisions by Sunip K. Mukherjee (sunipkmukherjee@gmail.com).
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef MEB_PRINT_H
#define MEB_PRINT_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define MEB_DBG_BPRINT 1
#define MEB_DBG_TPRINT 2
#define MEB_DBG_DBPRINT 4
#define MEB_DBG_ERPRINT 8

#define MEB_DBG_ALL (MEB_DBG_BPRINT | MEB_DBG_TPRINT | MEB_DBG_DBPRINT | MEB_DBG_ERPRINT)

#ifndef MEB_DBGLVL
#define MEB_DBGLVL MEB_DBG_ALL
#endif // MEB_DBGLVL

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define OS_Windows
#define TERMINATOR
#ifndef MEB_COLORS
#define MEB_COLORS
#define RESET_ALL
#define RED_FG
#define GREEN_FG
#define YELLOW_FG
#define BLUE_FG
#define MAGENTA_FG
#define CYAN_FG
#define RED_BG
#define GREEN_BG
#define YELLOW_BG
#define BLUE_BG
#define MAGENTA_BG
#define CYAN_BG
#endif // MEB_COLORS

#ifndef MEB_CODES
#define MEB_CODES
#define FATAL "(FATAL) "
#endif // MEB_CODES

#else // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#define TERMINATOR "\x1b[0m"
#ifndef MEB_COLORS
#define MEB_COLORS
#define RESET_ALL TERMINATOR
#define RED_FG "\x1b[91m"
#define GREEN_FG "\x1b[92m"
#define YELLOW_FG "\x1b[33m"
#define BLUE_FG "\x1b[94m"
#define MAGENTA_FG "\x1b[95m"
#define CYAN_FG "\x1b[96m"
#define RED_BG "\x1b[101m"
#define GREEN_BG "\x1b[102m"
#define YELLOW_BG "\x1b[43m"
#define BLUE_BG "\x1b[104m"
#define MAGENTA_BG "\x1b[105m"
#define CYAN_BG "\x1b[106m"
#endif // MEB_COLORS

#ifndef MEB_CODES
#define MEB_CODES
#define FATAL "\033[1m\x1b[107m\x1b[31m(FATAL) "
#endif // MEB_CODES
#endif // defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#ifndef dbprintlf
#define dbprintlf(format, ...)                                                                                    \
    {                                                                                                             \
        if (MEB_DBGLVL & MEB_DBG_DBPRINT)                                                                         \
        {                                                                                                         \
            fprintf(stderr, "[%s:%d | %s] " format TERMINATOR "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
            fflush(stderr);                                                                                       \
        }                                                                                                         \
    }
#endif // dbprintlf

#ifndef dbprintf
#define dbprintf(format, ...)                                                                                \
    {                                                                                                        \
        if (MEB_DBGLVL & MEB_DBG_DBPRINT)                                                                    \
        {                                                                                                    \
            fprintf(stderr, "[%s:%d | %s] " format TERMINATOR, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
            fflush(stderr);                                                                                  \
        }                                                                                                    \
    }
#endif // dbprintf

#if !defined(_MSC_VER)

#ifndef bprintf
#define bprintf(str, ...)                                        \
    (                                                            \
        {                                                        \
            int _meb_rc = 0;                                     \
            if (MEB_DBGLVL & MEB_DBG_BPRINT)                     \
            {                                                    \
                _meb_rc = printf(str TERMINATOR, ##__VA_ARGS__); \
                fflush(stdout);                                  \
            }                                                    \
            _meb_rc;                                             \
        })
#endif // bprintf

#ifndef bprintlf
#define bprintlf(str, ...)                                             \
    (                                                                  \
        {                                                              \
            int _meb_rc = 0;                                           \
            if (MEB_DBGLVL & MEB_DBG_BPRINT)                           \
            {                                                          \
                _meb_rc = printf(str TERMINATOR " \n", ##__VA_ARGS__); \
                fflush(stdout);                                        \
            }                                                          \
            _meb_rc;                                                   \
        })
#endif // bprintlf

#else

#if (MEB_DBGLVL & MEB_DBG_BPRINT)

#ifndef bprintf
#define bprintf(str, ...)                  \
    printf(str TERMINATOR, ##__VA_ARGS__);
#endif // bprintf

#ifndef bprintlf
#define bprintlf(str, ...)                       \
    printf(str TERMINATOR "\n", ##__VA_ARGS__);
#endif // bprintlf

#else

#ifndef bprintf
#define bprintf(str, ...) \
    0;
#endif

#ifndef bprintlf
#define bprintlf(str, ...) \
    0;
#endif

#endif // (MEB_DBGLVL & MEB_DBG_BPRINT)

#endif // _MSC_VER

#ifndef erprintlf
#define erprintlf(error)                                                                            \
    {                                                                                               \
        if (MEB_DBGLVL & MEB_DBG_ERPRINT)                                                           \
        {                                                                                           \
            fprintf(stderr, "[%s:%d | %s] " RED_FG "ERRNO >>> %d:" RESET_ALL " %s" TERMINATOR "\n", \
                    __FILE__, __LINE__, __func__, error, strerror(error));                          \
            fflush(stderr);                                                                         \
        }                                                                                           \
    }
#endif // erprintlf

static inline char *get_time_now()
{
    static __thread char buf[128];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buf, sizeof(buf), YELLOW_FG "[" YELLOW_FG "%02d:%02d:%02d" YELLOW_FG "] " TERMINATOR,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
    return buf;
}

#ifndef tprintf
#define tprintf(str, ...)                                               \
    {                                                                   \
        if (MEB_DBGLVL & MEB_DBG_TPRINT)                                \
        {                                                               \
            printf("%s" str TERMINATOR, get_time_now(), ##__VA_ARGS__); \
            fflush(stdout);                                             \
        }                                                               \
    }
#endif

#ifndef tprintlf
#define tprintlf(str, ...) \
    tprintf(str "\n", ##__VA_ARGS__)
#endif // tprintlf

#endif // MEB_PRINT_H
