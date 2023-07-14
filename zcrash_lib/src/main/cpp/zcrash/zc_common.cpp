//
// Created by zhaoyu on 2023/6/6.
//

#include "zc_common.h"
#include <cstdint>
#include <sys/types.h>
#include <jni.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-statement-expression"
#pragma clang diagnostic ignored "-Wcast-align"

//system info
int           zx_common_api_level         = 0;
char         *zx_common_os_version        = nullptr;
char         *zx_common_abi_list          = nullptr;
char         *zx_common_manufacturer      = nullptr;
char         *zx_common_brand             = nullptr;
char         *zx_common_model             = nullptr;
char         *zx_common_build_fingerprint = nullptr;
char         *zx_common_kernel_version    = nullptr;
long          zx_common_time_zone         = 0;

//app info
char         *zc_common_app_id            = nullptr;
char         *zc_common_app_version       = nullptr;
char         *zc_common_app_lib_dir       = nullptr;
char         *zc_common_log_dir           = nullptr;

//process info
pid_t         zc_common_process_id        = 0;
char         *zc_common_process_name      = nullptr;
uint64_t      zc_common_start_time        = 0;
JavaVM       *zc_common_vm                = nullptr;
jclass        zc_common_cb_class          = nullptr;
int           zc_common_fd_null           = -1;

//process statue
//sig_atomic_t  zx_common_native_crashed    = 0;
//sig_atomic_t  zx_common_java_crashed      = 0;

static int    zx_common_crash_prepared_fd = -1;
static int    zx_common_trace_prepared_fd = -1;
