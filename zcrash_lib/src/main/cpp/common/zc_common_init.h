//
// Created by zhaoyu on 2023/6/6.
//

#ifndef ZC_COMMON_INIT_H
#define ZC_COMMON_INIT_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdint>
#include <sys/types.h>
#include <jni.h>

// log filename format:
// tombstone_01234567890123456789_appversion__processname.native.xcrash
// tombstone_01234567890123456789_appversion__processname.trace.xcrash
// placeholder_01234567890123456789.clean.xcrash
#define ZC_COMMON_LOG_PREFIX           "tombstone"
#define ZC_COMMON_LOG_PREFIX_LEN       9
#define ZC_COMMON_LOG_SUFFIX_CRASH     ".native.zcrash"
#define ZC_COMMON_LOG_SUFFIX_TRACE     ".trace.zcrash"
#define ZC_COMMON_LOG_SUFFIX_TRACE_LEN 13
#define ZC_COMMON_LOG_NAME_MIN_TRACE   (9 + 1 + 20 + 1 + 2 + 13)
#define ZC_COMMON_PLACEHOLDER_PREFIX   "placeholder"
#define ZC_COMMON_PLACEHOLDER_SUFFIX   ".clean.zcrash"

int zc_common_init(int         api_level,
                   const char *os_version,
                   const char *abi_list,
                   const char *manufacturer,
                   const char *brand,
                   const char *model,
                   const char *build_fingerprint,
                   const char *app_id,
                   const char *app_version,
                   const char *app_lib_dir,
                   const char *log_dir);

#ifdef __cplusplus
}
#endif

#endif //ZC_COMMON_INIT_H