#include "Log.hpp"

#include <stdarg.h>
#include <android/log.h>


    void Log::info(const char* pMessage, ...) {
        va_list  lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_INFO, "Packman", pMessage, lVarArgs);
        __android_log_print( ANDROID_LOG_INFO, "Packman", "\n"); va_end(lVarArgs);
    }

    void Log::error(const char* pMessage, ...) {
        va_list  lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_ERROR, "Packman", pMessage, lVarArgs);
        __android_log_print( ANDROID_LOG_ERROR, "Packman", "\n"); va_end(lVarArgs);
    }

    void Log::warn(const char* pMessage, ...) {
        va_list  lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_WARN, "Packman", pMessage, lVarArgs);
        __android_log_print( ANDROID_LOG_WARN, "Packman", "\n"); va_end(lVarArgs);
    }

    void Log::debug(const char* pMessage, ...) {
        va_list  lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_DEBUG, "Packman", pMessage, lVarArgs);
        __android_log_print( ANDROID_LOG_DEBUG, "Packman", "\n"); va_end(lVarArgs);
    }

