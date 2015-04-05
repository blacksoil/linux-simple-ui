#include <android/log.h>
#define TAG "blacksoil"
#define DEBUG 1
#define PRINT_ON_LOGCAT 0

#define ALOGD(...) \
do { \
    if (PRINT_ON_LOGCAT){\
        __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__);\
    }\
    else {\
        printf(__VA_ARGS__);\
    }\
} while(0)

#define ALOGE(...) \
do { \
    if (PRINT_ON_LOGCAT){\
        __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__);\
    }\
    else {\
        printf(__VA_ARGS__);\
    }\
} while(0)


#define DEBUG_PRINT(...) \
do { \
    if (DEBUG) \
        if (PRINT_ON_LOGCAT) \
            __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__);\
        else \
            printf(__VA_ARGS__);\
    } while(0)
